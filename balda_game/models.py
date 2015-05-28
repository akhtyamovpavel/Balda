from django.contrib.auth.models import User, UserManager,AbstractBaseUser
from django.contrib.auth import get_user_model
from django.db import models
from django.core.cache import cache

import datetime
from django.conf import settings

# Create your models here.

class UserPlayer(models.Model):

    user = models.OneToOneField(User)
    wins = models.IntegerField()
    draws = models.IntegerField()
    loses = models.IntegerField()

    rating = models.IntegerField()


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

