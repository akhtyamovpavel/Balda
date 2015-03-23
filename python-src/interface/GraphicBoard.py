from PySide import QtGui, QtCore
from GameManager import GameManager
from Letter import Coordinates, CellLetter
from Player import Player
from interface.ButtonCell import ButtonCell
from lang.EnglishLanguage import EnglishLanguage
from lang.RussianLanguage import RussianLanguage

__author__ = 'akhtyamovpavel'

class GraphicBoard(QtGui.QWidget):

    push_letter_first = QtCore.Signal(Coordinates)
    push_letter_second = QtCore.Signal(Coordinates)
    choose_letter_first = QtCore.Signal(CellLetter)
    choose_letter_second = QtCore.Signal(CellLetter)
    commit_word_first = QtCore.Signal()
    commit_word_second = QtCore.Signal()
    quit = QtCore.Signal()
    give_up_first = QtCore.Signal()
    give_up_second = QtCore.Signal()
    
    def __init__(self, width, height):
        super(GraphicBoard, self).__init__()

        self.__width__ = width
        self.__hegiht__ = height
        self.__language__ = None
        lang_list = ['Russian', 'English']
        game_lang = QtGui.QInputDialog.getItem(self, 'Choose language of the game', 'Выберите язык игры', lang_list)
        if game_lang == 'Russian':
            self.__language__ = RussianLanguage()
        else:
            self.__language__ = EnglishLanguage()

        self.__table_layout__ = QtGui.QGridLayout()
        self.__buttons__ = list()

        for i in range(height):
            tmp_button_list = list()
            for j in range(width):
                tmp_button = ButtonCell(game_lang)
                tmp_button.connect_to_panel(self)
                self.__table_layout__.addWidget(tmp_button, i, j)
                tmp_button_list.append(tmp_button)
            self.__buttons__.append(tmp_button_list)

        self.__game_panel__ = QtGui.QHBoxLayout()
        self.__game_board_panel__ = QtGui.QVBoxLayout()
        self.__game_board_panel__.addLayout(self.__table_layout__)

        self.__word_panel__ = QtGui.QHBoxLayout()
        self.__current_word__ = QtGui.QLabel('')
        self.__commit_button__ = QtGui.QPushButton('Enter!')
        self.__commit_give_up__ = QtGui.QPushButton('Give up')
        self.__word_panel__.addWidget(self.__commit_give_up__)
        self.__word_panel__.addWidget(self.__current_word__)
        self.__word_panel__.addWidget(self.__commit_button__)

        self.__game_board_panel__.addLayout(self.__word_panel__)

        self.__first_player_score__ = QtGui.QLabel('0', self)
        self.__first_player_words__ = QtGui.QListWidget(self)
        self.__first_player_panel__ = QtGui.QVBoxLayout(self)
        self.__first_player_panel__.addWidget(self.__first_player_score__)
        self.__first_player_panel__.addWidget(self.__first_player_words__)

        self.__second_player_score__ = QtGui.QLabel('0', self)
        self.__second_player_words__= QtGui.QListWidget(self)
        self.__second_player_panel__ = QtGui.QListWidget(self)
        self.__second_player_panel__.addWidget(self.__second_player_score__)
        self.__second_player_panel__.addWidget(self.__second_player_words__)

        self.setLayout(self.__game_panel__)

        players = QtGui.QInputDialog.getInt(self, 'Enter number of players', 'Введите число игроков', 1, 1, 2)
        if players == 2:
            self.__game_manager__ = GameManager(self.__language__, width=width, height=height, players_number=players)
        else:
            list_levels = ['EASY', 'MEDIUM', 'HARD', 'HARDEST']
            level = QtGui.QInputDialog.getItem(self, 'Choose level', 'Выберите сложность', list_levels)
            self.__game_manager__ = GameManager(self.__language__, width, height, players, level)

        self.connect_to_players(self.__game_manager__.get_first_player(),
                                self.__game_manager__.get_second_player())

        self.__game_manager__.get_first_player().connect_to_interface(self)
        self.__game_manager__.get_second_player().connect_to_interface(self)

        word = self.__game_manager__.get_first_word()
        for i in range(width):
            self.__buttons__[height//2][i].setText(word[i])
            self.__buttons__[height//2][i].setMenu(None)

        self.__commit_button__.clicked.connect(self.on_commit_button_clicked)
        self.__commit_give_up__.clicked.connect(self.on_commit_give_up_clicked)

        self.__game_manager__.game_ended.connect(self.finish_game)


        #self.quit.connect(self.resetField())


    @QtCore.Slot()
    def on_cell_pushed(self):
        pass

    @QtCore.Slot(str)
    def on_cell_chosen(self, letter):
        pass

    @QtCore.Slot(Coordinates)
    def after_cell_pushed(self, coordinates: Coordinates):
        pass

    @QtCore.Slot(CellLetter)
    def after_cell_chosen(self, coordinates: CellLetter):
        pass


    @QtCore.Slot(str)
    def after_word_committed(self, word):
        pass

    @QtCore.Slot(Coordinates)
    def on_player_reset_word(self, coordinates: Coordinates):
        pass

    @QtCore.Slot()
    def on_commit_button_clicked(self):
        pass

    @QtCore.Slot()
    def on_commit_give_up_clicked(self):
        pass

    @QtCore.Slot(str)
    def finish_game(self, message):
        pass
    


    def connect_to_players(self, player1: Player, player2: Player):
        pass

    def run_step(self):
        pass



