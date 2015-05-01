import json
from django.dispatch import dispatcher
from balda_game.FieldState import FieldState
from balda_game.SingletonDictionary import dictionary

__author__ = 'akhtyamovpavel'


FIRST_PLAYER = 0
SECOND_PLAYER = 1

class GameManagerProcess:

    list_waiting_players = list()

    mapped_players = dict()

    list_games = dict()

    mapped_games = dict()
    list_first_words = dict()

    field_states = dict()
    current_moves = dict()
    first_players = dict()
    second_players = dict()

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

    def start_game(self, game_id):
        if self.field_states.get(game_id) is None:
            word = self.list_first_words[game_id]
            self.field_states[game_id] = FieldState(5, 5, word)
            self.current_moves[game_id] = FIRST_PLAYER

    def get_first_word_for_game(self, game_id):
        return self.list_first_words[game_id]

    def get_players(self, game_id):

        return self.list_games[game_id]

    def get_current_player(self, game_id):
        return self.current_moves.get(game_id)

    def get_field(self, game_id):
        field_state = self.field_states.get(game_id)
        list_fields = []
        for i in range(5):
            for j in range(5):
                state, letter = field_state.get_letter_state(i, j)
                list_fields.append({"height_level": i, "width_level": j, "letter": letter, "cell_state": state})
        return json.dumps(list_fields)






GameProcessor = GameManagerProcess()