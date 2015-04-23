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
    
    def __init__(self, width, height, parent):
        super(GraphicBoard, self).__init__()

        self.__width__ = width
        self.__height__ = height
        self.__language__ = None
        lang_list = ['Russian', 'English']
        game_lang, flag2 = QtGui.QInputDialog.getItem(self, 'Choose language of the game', 'Выберите язык игры', lang_list)
        if game_lang == 'Russian':
            self.__language__ = RussianLanguage()
        else:
            self.__language__ = EnglishLanguage()

        self.table_layout = QtGui.QGridLayout()
        self.buttons = list()

        for i in range(height):
            tmp_button_list = list()
            for j in range(width):
                tmp_button = ButtonCell(self.__language__)
                tmp_button.connect_to_panel(self)
                self.table_layout.addWidget(tmp_button, i, j)
                tmp_button_list.append(tmp_button)
            self.buttons.append(tmp_button_list)

        self.game_panel = QtGui.QHBoxLayout()
        self.game_board_panel = QtGui.QVBoxLayout()
        self.game_board_panel.addLayout(self.table_layout)

        self.word_panel = QtGui.QHBoxLayout()
        self.current_word = QtGui.QLabel('')
        self.commit_button = QtGui.QPushButton('Enter!')
        self.commit_give_up = QtGui.QPushButton('Give up')
        self.word_panel.addWidget(self.commit_give_up)
        self.word_panel.addWidget(self.current_word)
        self.word_panel.addWidget(self.commit_button)

        self.game_board_panel.addLayout(self.word_panel)

        self.first_player_score = QtGui.QLabel('0', self)
        self.first_player_words = QtGui.QListWidget(self)
        self.first_player_panel = QtGui.QVBoxLayout()
        self.first_player_panel.addWidget(self.first_player_score)
        self.first_player_panel.addWidget(self.first_player_words)

        self.second_player_score = QtGui.QLabel('0', self)
        self.second_player_words = QtGui.QListWidget(self)
        self.second_player_panel = QtGui.QVBoxLayout()
        self.second_player_panel.addWidget(self.second_player_score)
        self.second_player_panel.addWidget(self.second_player_words)

        self.game_panel.addLayout(self.first_player_panel)
        self.game_panel.addLayout(self.game_board_panel)
        self.game_panel.addLayout(self.second_player_panel)

        self.setLayout(self.game_panel)

        players, flag = QtGui.QInputDialog.getInt(self, 'Enter number of players', 'Введите число игроков', 1, 1, 2)
        if players == 2:
            self.__game_manager__ = GameManager(self.__language__, width=width, height=height, players_number=players)
        else:
            list_levels = ['EASY', 'MEDIUM', 'HARD', 'HARDEST']
            level = QtGui.QInputDialog.getItem(self, 'Choose level', 'Выберите сложность', list_levels)
            self.__game_manager__ = GameManager(self.__language__, width, height, players, level[0])

        self.connect_to_players(self.__game_manager__.get_first_player(),
                                self.__game_manager__.get_second_player())

        self.__game_manager__.get_first_player().connect_to_interface(self)
        self.__game_manager__.get_second_player().connect_to_interface(self)

        word = self.__game_manager__.get_first_word()
        for i in range(width):
            self.buttons[height//2][i].setText(word[i])
            self.buttons[height//2][i].setMenu(None)

        self.commit_button.clicked.connect(self.on_commit_button_clicked)
        self.commit_give_up.clicked.connect(self.on_commit_give_up_clicked)

        self.__game_manager__.game_ended.connect(self.finish_game)


        self.quit.connect(parent.reset_field)


    @QtCore.Slot()
    def on_cell_pushed(self):
        x_sender = 0
        y_sender = 0
        for i in range(self.__height__):
            for j in range(self.__width__):
                if self.sender() == self.buttons[i][j]:
                    x_sender = i
                    y_sender = j
        send_letter = Coordinates(x_sender, y_sender)
        if self.__game_manager__.get_current_player() == 1:
            self.push_letter_first.emit(send_letter)
        else:
            self.push_letter_second.emit(send_letter)

    @QtCore.Slot(str)
    def on_cell_chosen(self, letter):
        x_sender = 0
        y_sender = 0
        for i in range(self.__height__):
            for j in range(self.__width__):
                if self.sender() == self.buttons[i][j]:
                    x_sender = i
                    y_sender = j

        send_letter = CellLetter(x_sender, y_sender, letter)
        if self.__game_manager__.get_current_player() == 1:
            self.choose_letter_first.emit(send_letter)
        else:
            self.choose_letter_second.emit(send_letter)

    @QtCore.Slot(Coordinates)
    def after_cell_pushed(self, coordinates: Coordinates):
        self.current_word.setText(self.current_word.text() +
                                      self.buttons[coordinates.x][coordinates.y].text())

    @QtCore.Slot(CellLetter)
    def after_cell_chosen(self, coordinates: CellLetter):
        self.buttons[coordinates.x][coordinates.y].setMenu(None)
        self.buttons[coordinates.x][coordinates.y].setText(coordinates.letter)


    @QtCore.Slot(str)
    def after_word_committed(self, word):
        self.current_word.setText(None)
        if self.sender() == self.__game_manager__.get_first_player():
            self.first_player_words.addItem(word)
            score = self.__game_manager__.get_first_player().get_score()
            self.first_player_score.setText(str(score))
        else:
            self.second_player_words.addItem(word)
            score = self.__game_manager__.get_second_player().get_score()
            self.second_player_score.setText(str(score))
        self.run_step()


    @QtCore.Slot(Coordinates)
    def on_player_reset_word(self, coordinates: Coordinates):
        self.buttons[coordinates.x][coordinates.y].create_menu()
        self.buttons[coordinates.x][coordinates.y].setText('')
        self.current_word.setText('')

    @QtCore.Slot()
    def on_commit_button_clicked(self):
        if self.__game_manager__.get_current_player() == 1:
            self.commit_word_first.emit()
        else:
            self.commit_word_second.emit()

    @QtCore.Slot()
    def on_commit_give_up_clicked(self):
        if self.__game_manager__.get_current_player() == 1:
            self.give_up_first.emit()
        else:
            self.give_up_second.emit()

    @QtCore.Slot(str)
    def finish_game(self, message):
        box = QtGui.QMessageBox(self)
        box.setText(message)
        ok_button = box.addButton(QtGui.QMessageBox.Ok)
        box.exec_()
        if box.clickedButton() == ok_button:
            self.quit.emit()

    def connect_to_players(self, player1: Player, player2: Player):
        self.choose_letter_first.connect(player1.on_letter_chosen)
        self.push_letter_first.connect(player1.on_letter_pushed)

        self.choose_letter_second.connect(player2.on_letter_chosen)
        self.push_letter_second.connect(player2.on_letter_pushed)

        self.commit_word_first.connect(player1.on_word_committed)
        self.commit_word_second.connect(player2.on_word_committed)

        self.give_up_first.connect(player1.send_end)
        self.give_up_second.connect(player2.send_end)


    def run_step(self):
        self.__game_manager__.run_game()



