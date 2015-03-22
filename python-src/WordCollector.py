from Letter import Coordinates
from PySide import QtCore
import copy

__author__ = 'user1'


class WordCollector(QtCore.QObject):

    def __init__(self):
        QtCore.QObject.__init__(self)
        self.__word__ = ''

        self.__x_list__ = list()
        self.__y_list__ = list()

        self.__is_new__ = list()
        self.__changed_cell__ = Coordinates()
        self.__is_approved__ = False

        self.send_to_dictionary = QtCore.Signal(str)
        self.clear_state = QtCore.Signal()
        self.end_of_transaction = QtCore.Signal(str)
        self.approve_word = QtCore.Signal()

    def connect_to_dictionary(self, dictionary):
        pass

    def connect_to_board(self, board):
        pass

    def clear_word(self):
        if not self.__is_approved__:
            self.clear_state.emit(self.__changed_cell__)
        sent_word = copy.deepcopy(self.__word__)
        self.__word__ = ''
        self.__x_list__ = list()
        self.__y_list__ = list()
        self.__is_new__ = list()
        self.end_of_transaction.emit(sent_word)

    def end_move(self):
        print("Move ended")
        print("Approved word " + self.__word__)
        self.approve_word()

    @QtCore.Slot(str)
    def add_letter(self, letter):
        self.__word__ += letter

    @QtCore.Slot(int)
    def add_x(self, x):
        self.__x_list__.append(x)

    @QtCore.Slot(int)
    def add_y(self, y):
        self.__y_list__.append(y)

    @QtCore.Slot(Coordinates)
    def add_changed_cell(self, coordinates: Coordinates):
        self.__changed_cell__ = coordinates

    @QtCore.Slot()
    def has_approved(self):
        return self.__is_approved__

    @QtCore.Slot(int)
    def add_new(self, is_new_value):
        self.__is_new__.append(is_new_value)

    @QtCore.Slot()
    def check_word(self):
        if len(self.__x_list__) == 0:
            self.__is_approved__ = False
            self.clear_word()
            return
        self.__is_approved__ = True
        cnt_new = 0
        for i in range(1, len(self.__x_list__)):
            if abs(self.__x_list__[i] - self.__x_list__[i - 1]) + abs(self.__y_list__[i] - self.__y_list__[i - 1]) != 1:
                self.__is_approved__ = False
        if cnt_new != 1:
            self.__is_approved__ = False

        for i in range(0, len(self.__x_list__)):
            for j in range(i + 1, len(self.__y_list__)):
                if self.__x_list__[i] == self.__x_list__[j] and self.__y_list__[i] == self.__y_list__[j]:
                    self.__is_approved__ = False

        if self.__is_approved__:
            self.send_to_dictionary.emit(self.__word__)

        if self.__is_approved__:
            self.end_move()



    @QtCore.Slot(int)
    def set_word_approved(self, value):
        self.__is_approved__ = value


if __name__ == '__main__':
    wc = WordCollector()
