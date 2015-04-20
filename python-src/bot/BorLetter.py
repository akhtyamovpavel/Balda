__author__ = 'akhtyamovpavel'


class Letter:

    def __init__(self, symbol=' ', leaf=False):
        self.__symbol__ = symbol
        self.__children__ = list()
        self.__children_letters__ = list()
        self.__leaf__ = leaf

    def get_symbol(self):
        return self.__symbol__

    def is_leaf(self):
        return self.__leaf__

    def set_leaf(self, is_new_leaf):
        self.__leaf__ = is_new_leaf

    def add_children(self, children_position, symbol):
        self.__children__.append(children_position)
        self.__children_letters__.append(symbol)

    def find_children(self, symbol):
        for i in range(len(self.__children_letters__)):
            if self.__children_letters__[i] == symbol:
                return self.__children__[i]
        return -1

