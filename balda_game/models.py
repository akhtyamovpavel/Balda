from django.contrib.auth.models import User, UserManager,AbstractBaseUser
from django.contrib.auth import get_user_model
from django.db import models
from django.core.cache import cache

import datetime
from django.conf import settings

# Create your models here.
from balda_game.lib.field.Letter import CellLetter


class UserPlayer(models.Model):

    user = models.OneToOneField(User)
    wins = models.IntegerField()
    draws = models.IntegerField()
    loses = models.IntegerField()

    rating = models.IntegerField()

    was_online = models.DateField()

    def __str__(self):
        return self.user.username + " Wins: " + str(self.wins) + " Draws: " + str(self.draws) \
               + " Loses: " + str(self.loses) + " Rating: " + str(self.rating)

    def last_seen(self):
        return cache.get('seen_%s' % self.user.username)

    def last_seen_in_game(self, game_id):
        print(cache.get('seen_%d_%s' %(game_id, self.user.username)))
        return cache.get('seen_%d_%s' %(game_id, self.user.username))

    def last_waited(self):
        return cache.get('wait_%s' %(self.user.username))

    def online(self):
        if self.last_seen():
            now = datetime.datetime.now()
            if now > self.last_seen() + datetime.timedelta(seconds=settings.USER_ONLINE_TIMEOUT):
                return False
            else:
                return True
        else:
            return False

    def online_in_game(self, game_id):
        if self.last_seen_in_game(game_id) or self.last_waited():
            now = datetime.datetime.now()
            if self.last_seen_in_game(game_id):
                in_game = self.last_seen_in_game(game_id)
                if now <= in_game + datetime.timedelta(seconds=settings.USER_ONLINE_GAME_TIMEOUT):
                    return True
            if self.last_waited():
                waited = self.last_waited()
                if now <= waited + datetime.timedelta(seconds=settings.USER_ONLINE_GAME_TIMEOUT):
                    return True
            else:
                return False
        else:
            return False

class ListMoveModel(models.Model):

    x_cell = models.IntegerField()
    y_cell = models.IntegerField()
    letter = models.IntegerField()

    is_added = models.BooleanField()

    def __init__(self):
        super(ListMoveModel, self).__init__()
        self.is_added = False

    def set_letter(self, cell_letter: CellLetter):
        self.x_cell = cell_letter.x
        self.y_cell = cell_letter.y
        self.letter = cell_letter.letter

    def get_letter(self):
        return CellLetter(self.x_cell, self.y_cell, self.letter)

class GameModel(models.Model):

    first_user = models.ForeignKey(User, related_name="first_user")
    second_user = models.OneToOneField(User, related_name="second_user")

    first_score = models.IntegerField()
    second_score = models.IntegerField()

    is_extra_won = models.NullBooleanField()
    extra_winner = models.IntegerField()

    move_table = models.ForeignKey(ListMoveModel)

    def __str__(self):
        return self.id


