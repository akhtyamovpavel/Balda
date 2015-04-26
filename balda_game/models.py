from django.contrib.auth.models import User, UserManager
from django.db import models

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