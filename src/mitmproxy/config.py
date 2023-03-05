from __future__ import annotations

import inspect
import json
from enum import StrEnum
from pathlib import Path
from typing import TypedDict


class Game(TypedDict):
    entitlements: dict[str, str]  # dlc_id => name


class ItemStatus(StrEnum):
    ORIGINAL = "original",
    UNLOCKED = "unlocked",
    LOCKED = "locked",


class MitmProxy:
    listen_port: int = 9999
    extra_args: str = ""
    upstream_proxies: dict = {}


class ScreamApiConfigV3:
    logging: bool = False
    eos_logging: bool = False
    default_game_status: ItemStatus = ItemStatus.UNLOCKED
    override_game_status: dict[str, ItemStatus] = {}
    override_dlc_status: dict[str, ItemStatus] = {}
    extra_entitlements: dict[str, Game] = {}
    mitmproxy: MitmProxy = MitmProxy()

    def is_dlc_unlocked(self, game_id: str, dlc_id: str, original_unlocked: bool):
        status = self.default_game_status

        if game_id.lower() in self.override_game_status:
            status = self.override_game_status[game_id]

        if dlc_id.lower() in self.override_dlc_status:
            status = self.override_dlc_status[dlc_id]

        match status:
            case ItemStatus.UNLOCKED:
                is_unlocked = True
            case ItemStatus.LOCKED:
                is_unlocked = False
            case _:
                is_unlocked = original_unlocked

        # log.debug(f"Game ID: {game_id}, DLC ID: {dlc_id}, Status: {status}, "
        #           f"Original: {original_unlocked}, Unlocked: {is_unlocked}")

        return is_unlocked

    def __str__(self):
        d = {}
        for key, value in inspect.getmembers(self):
            if not callable(value) and not key.startswith("_"):
                d[key] = value

        return str(d)

    @staticmethod
    def parse(config_path: Path) -> ScreamApiConfigV3:
        instance = ScreamApiConfigV3()

        try:
            with open(config_path) as file:
                raw_json: dict = json.load(file)

                instance.logging = raw_json["logging"]
                instance.eos_logging = raw_json["eos_logging"]
                instance.default_game_status = raw_json["default_game_status"]
                instance.override_game_status = raw_json["override_game_status"]
                instance.override_dlc_status = raw_json["override_dlc_status"]
                instance.extra_entitlements = raw_json["extra_entitlements"]
                instance.mitmproxy.__dict__.update(raw_json["mitmproxy"])

        finally:
            return instance
