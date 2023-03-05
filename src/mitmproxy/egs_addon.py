import http
import json
import re
import urllib
from urllib import request
from typing import TypedDict
from urllib.parse import urlparse
from http import client

from mitmproxy import ctx
from mitmproxy.http import HTTPFlow

from config import ScreamApiConfigV3
from logger import log, init_file_logger
from paths import get_config_path, get_log_path


class Item(TypedDict):
    itemId: str
    namespace: str
    owned: bool


class Entitlement(TypedDict):
    active: bool
    catalogItemId: str
    consumable: bool
    entitlementName: str
    entitlementSource: str
    entitlementType: str
    grantDate: str
    id: str
    namespace: str
    status: str
    useCount: int


api_host = r"api\.epicgames\.dev"
ecom_host = r"ecommerceintegration.*\.epicgames\.com"

ownership_suffix = r"platforms/EPIC/identities/\w+/ownership"
modern_ownership_path = rf"^/epic/ecom/v1/{ownership_suffix}$"
legacy_ownership_path = rf"^/ecommerceintegration/api/public/{ownership_suffix}$"

modern_entitlement_path = r"^/epic/ecom/v1/identities/\w+/entitlements"
legacy_entitlement_path = r"^/ecommerceintegration/api/public/v2/identities/\w+/entitlements$"


class EgsAddon:
    config = ScreamApiConfigV3()
    api_cache: dict[str, dict] = {}

    def __init__(self) -> None:
        self.__update_config()

        if self.config.logging:
            init_file_logger(get_log_path())
        else:
            log.handlers.clear()

        ctx.options.set(f'allow_hosts={api_host}', f'allow_hosts={ecom_host}')

        log.info(f"EGS addon for mitmproxy initialized with config: {self.config}")

    async def response(self, flow: HTTPFlow):
        # log.debug(f'EpicAddon. Path: {flow.request.path}')
        try:
            self.__update_config()

            self.__intercept_ownership(flow)
            self.__intercept_entitlements(flow)
        except Exception as e:
            log.exception(e)

    def __update_config(self):
        self.config = ScreamApiConfigV3.parse(get_config_path())

    @staticmethod
    def __is_response_ok(flow: HTTPFlow):
        req = flow.request
        res = flow.response

        if res.status_code == 200:
            return True

        log.warn(
            f'Original response error. '
            f'URL: {req.url}. '
            f'Status code: ${res.status_code}. '
            f'Body: {res.text}'
        )

        return False

    def __intercept_ownership(self, flow: HTTPFlow):
        if not self.__host_and_path_match(flow, api_host, modern_ownership_path) and \
                not self.__host_and_path_match(flow, ecom_host, legacy_ownership_path):
            return

        if not self.__is_response_ok(flow):
            return

        log.info(f"Intercepted ownership request. Query: {flow.request.query}")
        log.info(f"Original ownership response:\n{self.get_pretty_response(flow)}")

        items: list[Item] = flow.response.json()

        for item in items:
            item['owned'] = self.config.is_dlc_unlocked(
                game_id=item['namespace'],
                dlc_id=item['itemId'],
                original_unlocked=item['owned'],
            )

        flow.response.text = json.dumps(items)

        log.info(f"Modified ownership response:\n{self.get_pretty_response(flow)}")

    def __intercept_entitlements(self, flow: HTTPFlow):
        if not self.__host_and_path_match(flow, api_host, modern_entitlement_path) and \
                not self.__host_and_path_match(flow, ecom_host, legacy_entitlement_path):
            return

        if not self.__is_response_ok(flow):
            return

        log.info(f'Intercepted entitlements request. Query: {flow.request.query}')
        log.info(f'Original entitlements response:\n{self.get_pretty_response(flow)}')

        original_entitlements = flow.response.json()

        entitlements: list[Entitlement] = []

        if (game_id := flow.request.query.get('sandboxId')) is None:
            log.warn(f"Missing sandboxId in query params: {flow.request.query}")
            return

        entitlement_names: list[str]
        if entitlement_names := flow.request.query.get_all('entitlementName'):
            # We have the entitlement IDs already in request parameters.
            for dlc_id in entitlement_names:
                dlc_name = f"DLC {dlc_id}"
                original_unlocked = any(
                    e['catalogItemId'].lower() == dlc_id.lower() for e in original_entitlements
                )
                active = self.config.is_dlc_unlocked(game_id, dlc_id, original_unlocked)
                entitlement = self.new_entitlement("query", game_id, dlc_id, dlc_name, active)
                entitlements.append(entitlement)
        else:
            # We need to fetch entitlement IDs from the Epic Games API
            api_data = self.get_api_data(game_id)

            if api_data is not None:
                for element in api_data["data"]["Catalog"]["catalogOffers"]["elements"]:
                    for item in element["items"]:
                        dlc_id = item["id"]
                        dlc_name = item["title"]
                        active = self.config.is_dlc_unlocked(game_id, dlc_id, False)
                        entitlements.append(
                            self.new_entitlement("api", game_id, dlc_id, dlc_name, active)
                        )

        # Add extra entitlements if necessary
        if (game := self.config.extra_entitlements.get(game_id)) is not None:
            for dlc_id, dlc_name in game["entitlements"].items():
                if any(e['catalogItemId'].lower() == dlc_id.lower() for e in entitlements):
                    continue  # Skip existing entitlements

                active = self.config.is_dlc_unlocked(game_id, dlc_id, True)
                entitlement = self.new_entitlement("extra", game_id, dlc_id, dlc_name, active)
                entitlements.append(entitlement)

        # Filter entitlements that are not unlocked
        entitlements = [e for e in entitlements if e['active']]

        # active was used to store original unlock status
        for entitlement in entitlements:
            entitlement['active'] = True

        flow.response.text = json.dumps(entitlements)

        log.info(f"Modified entitlements response:\n{self.get_pretty_response(flow)}")

    def get_api_data(self, game_id: str):
        if game_id.lower() in self.api_cache:
            return self.api_cache[game_id]

        payload = {
            "query": "query($namespace: String!) {\
                      Catalog {\
                          catalogOffers(\
                              namespace: $namespace\
                              params: { count: 1000}\
                          ) {\
                              elements {\
                                  items { \
                                      id\
                                      title\
                                  }\
                              }\
                          }\
                      }\
                  }",
            "variables": {
                "namespace": game_id
            }
        }

        log.info(f"Making DLC data request to Epic Games API for game id: {game_id}")

        res = self.post_request("https://graphql.epicgames.com/graphql", payload)

        if res is not None:
            log.debug(f"Epic Games GraphQL API response: {json.dumps(res, indent=2)}")

            self.api_cache[game_id.lower()] = res
        else:
            log.error("Error fetching entitlements from the Epic Games GraphQL API")

        return res

    # Because we don't want the headache of managing 3rd part libs as an addon script...
    def post_request(self, url: str, data: dict | list):
        try:
            req = urllib.request.Request(
                url=url,
                data=json.dumps(data).encode('utf-8'),
                headers={'content-type': 'application/json'}
            )

            proxy_support = urllib.request.ProxyHandler(self.config.mitmproxy.upstream_proxies)
            opener = urllib.request.build_opener(proxy_support)
            urllib.request.install_opener(opener)

            res: http.client.HTTPResponse = urllib.request.urlopen(req)

            if res.status == 200:
                return json.loads(res.read().decode("utf-8"))
            else:
                log.error(f"HTTP POST request error: {res.status} - {res.name}")
                return None
        except Exception as e:
            log.exception(e)
            return None

    @staticmethod
    def get_pretty_response(flow: HTTPFlow):
        return json.dumps(flow.response.json(), indent=2)

    @staticmethod
    def new_entitlement(
            tag: str,
            game_id: str,
            dlc_id: str,
            dlc_name: str,
            active: bool
    ) -> Entitlement:
        log.info(f"Making {tag} entitlement {game_id}:{dlc_id}")

        return Entitlement(
            active=active,  # Temporary storage of unlock status
            catalogItemId=dlc_id,
            consumable=False,
            entitlementName=dlc_name,
            entitlementSource="eos",
            entitlementType="AUDIENCE",
            grantDate="2023-01-01T00:00:00.000Z",
            id=dlc_id,
            namespace=game_id,
            status="ACTIVE",
            useCount=0,
        )

    @staticmethod
    def __host_and_path_match(flow: HTTPFlow, host: str, path: str) -> bool:
        req_host = flow.request.pretty_host
        req_path = urlparse(flow.request.url).path

        return bool(re.match(host, req_host)) and bool(re.match(path, req_path))


addons = [EgsAddon()]
