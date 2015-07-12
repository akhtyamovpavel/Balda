import json

from balda_game.lib.field.CellState import FIXED, PINNED, SPARE
from balda_game.lib.field.FieldState import FieldState
from balda_game.lib.dictionary.SingletonDictionary import dictionary
from balda_game.models import UserPlayer

__author__ = 'akhtyamovpavel'


FIRST_PLAYER = 0
SECOND_PLAYER = 1

class GameManagerProcess:

    list_waiting_players = set()

    mapped_players = dict()

    list_games = dict()

    mapped_games = dict()
    list_first_words = dict()


    field_states = dict()
    current_moves = dict()
    first_players = dict()
    second_players = dict()

    scores = dict()
    number_of_spare_cells = dict()
    ended_games = set()

    first_player_words = dict()
    second_player_words = dict()

    cnt = 0

    def add_player(self, user):
        self.list_waiting_players.add(user)

    def add_waiting_player(self, user):
        if not self.mapped_players.get(user) is None:
            return self.mapped_games[user]
        else:
            for player in self.list_waiting_players:
                #TODO fix these rule for ending game
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
            dictionary.setup_connection(game_id)
            word = self.list_first_words[game_id]

            dictionary.pin_first_word(game_id, word)
            self.field_states[game_id] = FieldState(5, 5, word)
            self.current_moves[game_id] = FIRST_PLAYER
            self.scores[game_id] = (0, 0)
            self.number_of_spare_cells[game_id] = 20
            self.first_player_words[game_id] = []
            self.second_player_words[game_id] = []


    def get_first_word_for_game(self, game_id):
        return self.list_first_words[game_id]

    def get_players(self, game_id):
        return self.list_games[game_id]

    def get_scores(self, game_id):
        return self.scores.get(game_id)

    def get_current_player(self, game_id):
        return self.current_moves.get(game_id)

    def is_game_ended(self, game_id):
        if self.number_of_spare_cells.get(game_id) is None:
            return True
        return self.number_of_spare_cells.get(game_id) == 0


    def get_field(self, game_id):
        field_state = self.field_states.get(game_id)
        list_fields = []
        for i in range(5):
            for j in range(5):
                state, letter = field_state.get_letter_state(i, j)
                list_fields.append({"height_level": i, "width_level": j, "letter": letter, "cell_state": state})
        return json.dumps(list_fields)

    def get_list_of_words(self, game_id):
        return (self.first_player_words.get(game_id), self.second_player_words.get(game_id))

    def check_for_connection(self, game_id):
        first_user, second_user = self.get_players(game_id)
        player1 = UserPlayer.objects.get(user=first_user)
        player2 = UserPlayer.objects.get(user=second_user)
        if not player1.online_in_game(game_id):
            self.give_up(game_id, first_user)
        if not player2.online_in_game(game_id):
            self.give_up(game_id, second_user)



    def give_up(self, game_id, user):
        self.end_game(game_id, user)
        self.number_of_spare_cells[game_id] = 0


    def recalculate_rating(self, user1, user2, won_user=None):
        """
        Elo rating implemented
        :param user1: First User
        :param user2: Second User
        :param won_user: Won User
        """
        player1 = UserPlayer.objects.get(user=user1)
        player2 = UserPlayer.objects.get(user=user2)
        rating1 = player1.rating
        rating2 = player2.rating
        points1 = 0.0
        points2 = 0.0
        if won_user is None:
            points1 = 0.5
            points2 = 0.5
        elif player1 == won_user:
            points1 = 1.0
            points2 = 0.0
        else:
            points1 = 0.0
            points2 = 1.0
        K = 20.0
        expectation1 = 1./(1. + pow(10., (rating2 - rating1) / 400.))
        expectation2 = 1./(1. + pow(10., (rating1 - rating2) / 400.))

        new_rating1 = rating1 + K*(points1 - expectation1)
        new_rating2 = rating2 + K*(points2 - expectation2)

        player1.rating = int(new_rating1)
        player2.rating = int(new_rating2)
        player1.save()
        player2.save()



    def end_game(self, game_id, given_up_user = None):
        first_player, second_player = self.get_players(game_id)
        if game_id in self.ended_games:
            return
        if given_up_user is not None:
            lost_user = None
            win_user = None
            if first_player == given_up_user:
                lost_user = UserPlayer.objects.get(user=first_player)
                win_user = UserPlayer.objects.get(user=second_player)
                # TODO Elo rating system
            else:
                lost_user = UserPlayer.objects.get(user=second_player)
                win_user = UserPlayer.objects.get(user=first_player)
            lost_user.loses += 1
            win_user.wins += 1
            lost_user.save()
            win_user.save()
        else:
            first_score, second_score = self.scores.get(game_id)
            lost_user = None
            win_user = None
            draw1_user = None
            draw2_user = None
            if first_score < second_score:
                lost_user = UserPlayer.objects.get(user=first_player)
                win_user = UserPlayer.objects.get(user=second_player)
            elif first_score > second_score:
                lost_user = UserPlayer.objects.get(user=second_player)
                win_user = UserPlayer.objects.get(user=first_player)
            else:
                draw1_user = UserPlayer.objects.get(user=first_player)
                draw2_user = UserPlayer.objects.get(user=second_player)
            if lost_user is not None:
                lost_user.loses += 1
                win_user.wins += 1
                lost_user.save()
                win_user.save()
            else:
                draw1_user.draws += 1
                draw2_user.draws += 1
                draw1_user.save()
                draw2_user.save()
        self.recalculate_rating(first_player, second_player, win_user)
        self.ended_games.add(game_id)
        self.list_waiting_players.remove(first_player)
        self.list_waiting_players.remove(second_player)
        self.mapped_games.pop(first_player)
        self.mapped_games.pop(second_player)
        self.mapped_players.pop(first_player)
        self.mapped_players.pop(second_player)

    def check_board_consistency(self, game_id, pinned_height, pinned_width, word, heights, widths):
        field_state = self.field_states.get(game_id)
        flag = True
        for i in range(len(heights)):
            height_cell = heights[i]
            width_cell = widths[i]
            state, letter = field_state.get_letter_state(height_cell, width_cell)
            if state == FIXED:
                if letter != word[i]:
                    flag = False
                if pinned_height == height_cell and pinned_width == width_cell:
                    flag = False
            elif state == PINNED:
                flag = False
            elif state == SPARE:
                flag = flag and (pinned_height == height_cell)
                flag = flag and (pinned_width == width_cell)
        return flag

    def change_move(self, user, game_id, word, pinned_height, pinned_width, pinned_letter):

        first_player, second_player = self.get_players(game_id)
        score = len(word)
        field_state = self.field_states.get(game_id)
        field_state.set_state(pinned_height, pinned_width, FIXED, pinned_letter)
        score1, score2 = self.scores.get(game_id)

        #check for hacks
        if self.number_of_spare_cells.get(game_id) is None:
            return False
        if self.number_of_spare_cells.get(game_id) == 0:
            return False
        if self.current_moves.get(game_id) is None:
            return False
        if first_player == user and self.current_moves.get(game_id) != FIRST_PLAYER:
            return False
        if second_player == user and self.current_moves.get(game_id) != SECOND_PLAYER:
            return False


        if first_player == user:
            score1 += score
            list1 = self.first_player_words.get(game_id)
            list1.append(word)
            self.first_player_words[game_id] = list1
            self.current_moves[game_id] = SECOND_PLAYER
        else:
            score2 += score
            list1 = self.second_player_words.get(game_id)
            list1.append(word)
            self.second_player_words[game_id] = list1
            self.current_moves[game_id] = FIRST_PLAYER
        # TODO need field state update or not?

        self.scores[game_id] = (score1, score2)
        self.number_of_spare_cells[game_id] = self.number_of_spare_cells.get(game_id) - 1
        # print(self.number_of_spare_cells.get(game_id))
        return True




GameProcessor = GameManagerProcess()
