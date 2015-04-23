from Letter import Coordinates
from PySide import QtCore
import copy

__author__ = 'user1'


class WordCollector(QtCore.QObject):
    send_to_dictionary = QtCore.Signal(str)
    clear_state = QtCore.Signal(Coordinates)
    end_of_transaction = QtCore.Signal(str)
    approve_word = QtCore.Signal()

    def __init__(self):
        QtCore.QObject.__init__(self)
        self._word_ = ''

        self._x_list_ = list()
        self._y_list_ = list()

        self._is_new_ = list()
        self._changed_cell_ = Coordinates()
        self._is_approved_ = False


    def connect_to_dictionary(self, dictionary):
        self.send_to_dictionary.connect(dictionary.check_word)

    def connect_to_board(self, board):
        self.clear_state.connect(board.reset_state)
        self.end_of_transaction.connect(board.remake_move)
        self.approve_word.connect(board.set_approved)

    def clear_word(self):
        if not self._is_approved_:
            self.clear_state.emit(self._changed_cell_)
        sent_word = copy.deepcopy(self._word_)
        self._word_ = ''
        self._x_list_ = list()
        self._y_list_ = list()
        self._is_new_ = list()
        self.end_of_transaction.emit(sent_word)

    def end_move(self):
        print("Move ended")
        print("Approved word " + self._word_)
        self.approve_word.emit()

    @QtCore.Slot(str)
    def add_letter(self, letter):
        self._word_ += letter

    @QtCore.Slot(int)
    def add_x(self, x):
        self._x_list_.append(x)

    @QtCore.Slot(int)
    def add_y(self, y):
        self._y_list_.append(y)

    @QtCore.Slot(Coordinates)
    def add_changed_cell(self, coordinates: Coordinates):
        self._changed_cell_ = coordinates

    @QtCore.Slot()
    def has_approved(self):
        return self._is_approved_

    @QtCore.Slot(int)
    def add_new(self, is_new_value):
        self._is_new_.append(is_new_value)

    @QtCore.Slot()
    def check_word(self):
        if len(self._x_list_) == 0:
            self._is_approved_ = False
            self.clear_word()
            return
        self._is_approved_ = True
        cnt_new = 0
        for i in range(1, len(self._x_list_)):
            if abs(self._x_list_[i] - self._x_list_[i - 1]) + abs(self._y_list_[i] - self._y_list_[i - 1]) != 1:
                self._is_approved_ = False


        for i in range(len(self._x_list_)):
            if self._x_list_[i] == self._changed_cell_.x and self._y_list_[i] == self._changed_cell_.y:
                cnt_new += 1
        if cnt_new != 1:
            self._is_approved_ = False

        for i in range(0, len(self._x_list_)):
            for j in range(i + 1, len(self._y_list_)):
                if self._x_list_[i] == self._x_list_[j] and self._y_list_[i] == self._y_list_[j]:
                    self._is_approved_ = False

        if self._is_approved_:
            self.send_to_dictionary.emit(self._word_)

        if self._is_approved_:
            self.end_move()

        self.clear_word()



    @QtCore.Slot(int)
    def set_word_approved(self, value):
        self._is_approved_ = value


if __name__ == '__main__':
    wc = WordCollector()
