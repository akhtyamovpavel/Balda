from django.dispatch import dispatcher
from balda_game.SingletonDictionary import dictionary

__author__ = 'akhtyamovpavel'

class GameManagerProcess:

    list_waiting_players = list()

    mapped_players = dict()

    list_games = dict()

    mapped_games = dict()
    list_first_words = dict()
    cnt = 0

    def add_player(self, user):
        self.list_waiting_players.append(user)

    def add_waiting_player(self, user):
        if not self.mapped_players.get(user) is None:
            return self.mapped_games[user]
        else:
            for player in self.list_waiting_players:
                if user.username != player.username and self.mapped_players.get(player) is None:
                    self.mapped_players[player] = user
                    self.mapped_players[user] = player
                    self.cnt += 1
                    self.list_games[self.cnt] = (user, player)
                    self.mapped_games[user] = self.cnt
                    self.mapped_games[player] = self.cnt
                    word = dictionary.get_first_word(5)
                    self.list_first_words[self.cnt] = word
                    return self.cnt
        return -1


GameProcessor = GameManagerProcess()