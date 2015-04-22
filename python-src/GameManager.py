from PySide import QtCore
from Board import Board
from Dictionary import Dictionary
from Player import Player
from WordCollector import WordCollector
from bot.Bot import Bot, EASY, MEDIUM, HARD, HARDEST
from lang.Language import Language

__author__ = 'akhtyamovpavel'

FIRST_PLAYER = 1
SECOND_PLAYER = 2

class GameManager(QtCore.QObject):

    start_move_first = QtCore.Signal()
    start_move_second = QtCore.Signal()
    ask_for_cells = QtCore.Signal()
    game_ended = QtCore.Signal(str)

    show_board = QtCore.Signal()


    @QtCore.Slot()
    def step_ended(self):
        if self.__current_id__ == FIRST_PLAYER:
            self.__current_id__ =  SECOND_PLAYER
        else:
            self.__current_id__ = FIRST_PLAYER

        self.__number_of_spare_cells__ -= 1

    @QtCore.Slot(int)
    def get_number_of_cells(self, value):
        self.__number_of_spare_cells__ = value

    @QtCore.Slot()
    def game_ending(self):
        message = None
        if self.__players_number__ == 2:
            score1 = self.__player1__.get_score()
            score2 = self.__player2__.get_score()
            if score1 > score2:
                message = 'First player win'
            elif score1 == score2:
                message = 'Draw'
            else:
                message = 'Second player win'
        else:
            score1 = self.__player1__.get_score()
            score2 = self.__player2__.get_score()
            if score1 > score2:
                message = 'You win'
            elif score1 == score2:
                message = 'Draw'
            else:
                message = 'Computer win'
        self.game_ended.emit(message)


    def __init__(self, language: Language, width, height, players_number, level=''):
        super(GameManager, self).__init__()
        self.__bot__ = Bot(language, width, height)
        self.__width__ = width
        self.__height__ = height
        self.__players_number__ = players_number

        self.__board__= Board()
        self.__board__.init_board(width, height)

        self.__dictionary__ = Dictionary()
        self.__dictionary__.load_dictionary()
        self.__wc__ = WordCollector()
        self.__wc__.connect_to_dictionary(self.__dictionary__)
        self.__wc__.connect_to_board(self.__board__)

        self.__dictionary__.setup_connection(self.__wc__)
        self.__board__.setup_connection(self.__wc__)

        self.__first_word__ = self.__dictionary__.get_first_word(width)

        self.__player1__ = Player()
        self.__player2__ = Player()

        if players_number == 2:
            self.__player1__.connect_to_board(self.__board__)
            self.__player1__.connect_to_manager(self)

            self.__player2__.connect_to_board(self.__board__)
            self.__player2__.connect_to_manager(self)
        else:
            self.__player1__.connect_to_board(self.__board__)
            self.__player1__.connect_to_manager(self)
            self.__dictionary__.connect_to_bot(self.__bot__)
            self.__dictionary__.used_words_to_bot(self.__bot__)
            if level == 'EASY':
                self.__bot__.set_level(EASY)
            elif level == 'MEDIUM':
                self.__bot__.set_level(MEDIUM)
            elif level == 'HARD':
                self.__bot__.set_level(HARD)
            elif level == 'HARDEST':
                self.__bot__.set_level(HARDEST)
            self.__bot__.connect_to_board(self.__board__)
            self.__bot__.connect_to_manager(self)
            self.__bot__.connect_to_dictionary(self.__dictionary__)
            self.__bot__.get_dictionary()
            self.__bot__.connect_to_used_dictionary(self.__dictionary__)


        self.__current_player__ = self.__player1__
        self.__current_id__ = FIRST_PLAYER
        self.__number_of_spare_cells__ = width*(height - 1)

        if players_number == 2:
            self.__board__.connect_to_players(self.__player1__, self.__player2__)
            self.__board__.set_first_player(FIRST_PLAYER)
        else:
            self.__board__.connect_to_players(self.__player1__, self.__bot__)
            self.__board__.set_first_player(FIRST_PLAYER)

        self.ask_for_cells.connect(self.__board__.get_number_of_cells)
        self.start_move_first.connect(self.__player1__.begin_step)

        if players_number == 2:
            self.start_move_second.connect(self.__player2__.begin_step)
        else:
            self.start_move_second.connect(self.__bot__.begin_step)

        self.__board__.set_first_word(self.__first_word__)

    def run_game(self):
        if not self.is_game_ended():
            self.show_board.emit()
            if self.__players_number__ == 2:
                if self.__current_id__ == FIRST_PLAYER:
                    print("First player: your move")
                    self.start_move_first.emit()
                else:
                    print("Second player: your move")
                    self.start_move_second.emit()

            if self.__players_number__ == 1:
                if self.__current_id__ == FIRST_PLAYER:
                    print("First player: your move")
                    self.start_move_first.emit()
                else:
                    print("Second player: your move")
                    self.start_move_second.emit()
            return
        self.game_ending()


    def get_first_player(self):
        return self.__player1__

    def get_second_player(self):
        if self.__players_number__ == 2:
            return self.__player2__
        else:
            return self.__bot__

    def get_current_player(self):
        return self.__current_id__

    def get_first_word(self):
        return self.__first_word__

    def is_game_ended(self):
        self.ask_for_cells.emit()
        return self.__number_of_spare_cells__ == 0
