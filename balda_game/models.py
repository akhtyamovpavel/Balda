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
               + " Loses: " + str(self.loses)

    def last_seen(self):
        return cache.get('seen_%s' % self.user.username)

    def online(self):
        if self.last_seen():
            now = datetime.datetime.now()
            if now > self.last_seen() + datetime.timedelta(seconds=settings.USER_ONLINE_TIMEOUT):
                return False
            else:
                return True
        else:
            return False