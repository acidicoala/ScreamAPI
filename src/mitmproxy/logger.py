from logging import *
from pathlib import Path

log = getLogger('EpicAddon')


def init_file_logger(log_path: Path):
    formatter = Formatter(
        '%(levelname)s│ %(asctime)s.%(msecs)03d │ %(lineno)3d:%(filename)-24s ┃ %(message)s'
    )

    formatter.datefmt = '%H:%M:%S'
    addLevelName(DEBUG, '⬛')
    addLevelName(INFO, '🟩')
    addLevelName(WARN, '🟨')
    addLevelName(ERROR, '🟥')
    addLevelName(CRITICAL, '💥')

    # Open with append mode since we want the ScreamAPI to manage log reset
    file_handler = FileHandler(filename=log_path.absolute(), mode='a', encoding='utf-8')
    file_handler.setFormatter(formatter)
    file_handler.setLevel(DEBUG)

    log.setLevel(DEBUG)
    log.addHandler(file_handler)
