from enum import Enum
from django.contrib.auth.models import User

__author__ = 'akhtyamovpavel'

class Level(Enum):
    EASY = 1
    MEDIUM = 2
    HARD = 3
    HARDEST = 100500


def get_bot_by_level(level: Level):
    if level == Level.EASY:
        return User.objects.get(username='EASYBOT')
    if level == Level.MEDIUM:
        return User.objects.get(username='MEDIUMBOT')
    if level == Level.HARD:
        return User.objects.get(username='HARDBOT')
    if level == Level.HARDEST:
        return User.objects.get(username='HARDESTBOT')


def is_bot(user):
    if user.username == 'EASYBOT':
        return True
    if user.username == 'MEDIUMBOT':
        return True
    if user.username == 'HARDBOT':
        return True
    if user.username == 'HARDESTBOT':
        return True
    return False