from django.contrib.auth.models import User, UserManager, AbstractBaseUser
from django.contrib.auth import get_user_model
from django.db import models
from django.core.cache import cache

import datetime
from django.conf import settings

# Create your models here.
from balda_game.lib.JSONlib import serialize_game_log_to_json, deserialize_game_log_from_json
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
        print(cache.get('seen_%d_%s' % (game_id, self.user.username)))
        return cache.get('seen_%d_%s' % (game_id, self.user.username))

    def last_waited(self):
        return cache.get('wait_%s' % (self.user.username))

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


class GameModel(models.Model):
    # TODO Make status for this model
    # TODO Show game history if game ended
    first_user = models.ForeignKey(User, related_name="first_user")
    second_user = models.ForeignKey(User, related_name="second_user")

    first_score = models.IntegerField(null=True)
    second_score = models.IntegerField(null=True)

    is_extra_won = models.NullBooleanField(null=True)
    extra_winner = models.ForeignKey(User, related_name="extra_winner", null=True)

    field_size = models.IntegerField()
    first_word = models.TextField(null=True)
    game_log = models.TextField(null=True)

    STATUSES = (
        ('wait', 'WAITED'),
        ('play', 'PLAYING'),
        ('finish', 'EXPECTED FINISHING'),
        ('end', 'ENDED')
    )

    status = models.TextField(choices=STATUSES)

    def __str__(self):
        return str(self.id)

    def set_game_log(self, list_moves):
        self.game_log = serialize_game_log_to_json(list_moves)

    def set_first_word(self, first_word):
        self.first_word = first_word

    def get_game_log(self):
        return deserialize_game_log_from_json(self.game_log)
