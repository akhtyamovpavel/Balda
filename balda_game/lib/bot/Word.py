__author__ = 'akhtyamovpavel'

class Word:

    def __init__(self, push_string=None, push_coordinates=None):
        self.__possible_word__ = push_string
        self.__coordinates__ = push_coordinates[:]

    def __lt__(self, other):
        return len(self.__possible_word__) < len(other.__possible_word__)