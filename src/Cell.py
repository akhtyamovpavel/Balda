__author__ = 'user1'
from PySide import QtCore


class Cell(QtCore.QObject):
    def __init__(self, letter):
        super(Cell, self).__init__()
        self.__letter__ = letter
        self.__is_marked__ = False

    def get_letter(self):
        return self.__letter__

    def set_letter(self, letter):
        self.__letter__ = letter

    def is_marked(self):
        return self.__is_marked__

    def set_marked(self, value):
        self.__is_marked__ = value