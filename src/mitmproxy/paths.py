from pathlib import Path


def get_self_path() -> Path:
    return Path(__file__).parent


def get_config_path() -> Path:
    return get_self_path() / "ScreamAPI.config.json"


def get_log_path() -> Path:
    return get_self_path() / "ScreamAPI.log.log"
