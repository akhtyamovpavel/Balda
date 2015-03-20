from random import Random
from Dictionary import Dictionary
from Letter import CellLetter, Coordinates
from Player import Player
from bot.Bor import Bor, PRE_VERTEX

__author__ = 'akhtyamovpavel'

from PySide import QtCore

EASY = 1
MEDIUM = 2
HARD = 3
HARDEST = 100500

DEFAULT_WIDTH = 5
DEFAULT_HEIGHT = 5


class Bot(Player):
    get_dictionary = QtCore.Signal()
    send_letter = QtCore.Signal(CellLetter)
    send_word = QtCore.Signal(str)

    @QtCore.Slot()
    def begin_step(self):
        self.run_process()

    @QtCore.Slot()
    def setup_dictionary(self, words):
        print("Get words")
        for word in words:
            self.__bor_vocabulary__.add_word(word)

    def __init__(self, language, width=DEFAULT_WIDTH, height=DEFAULT_HEIGHT):
        super(Bot, self).__init__()
        self.__width__ = width
        self.__height__ = height
        self.__moves__ = [(0, 1), (0, -1), (1, 0), (-1, 0)]

        self.__level__ = EASY #default value
        self.__bor_vocabulary__ = Bor()

    def maximal_length(self, variants):
        pass

    def easy_index_word(self, variants):
        size = len(variants) - 1
        random = Random()
        d = int(size*random.random())
        return d

    def medium_index_word(self, variants):
        size = len(variants) - 1
        val = 0
        for variant in variants:
            val += len(variant.possible_word) * len(variant.possible_word)

        random_number = int(Random().random() * val)

        cur = 0
        cnt = 0
        for variant in variants:
            cur += len(variant.possible_word) * len(variant.possible_word)
            if cur >= random_number:
                return cnt
            cnt += 1

        return 0


    def hard_index_word(self, variants):
        pass

    def hardest_index_word(self, variants, symbols, not_allowed_words):
        pass

    def set_level(self, difficulty):
        self.__level__ = difficulty

    def connect_to_dictionary(self, dictionary: Dictionary):
        self.get_dictionary.connect(dictionary.send_dictionary)

    def begin_step(self):
        self.run_process()

    def run_process(self):
        not_allowed_words = list()

        self.show_board.emit()

        symbols = [['#' for i in range(self.__width__ + 2)] for j in range(self.__height__ + 2)]

        for i in range(1, self.__height__ + 1):
            for j in range(1, self.__width__ + 1):
                symbols[i][j] = self.__board__[i - 1][j - 1]

        variants = self.possible_variants(symbols)
        is_committed = False

        id = -1

        if self.__level__ == EASY:
            id = self.easy_index_word(variants)
        elif self.__level__ == MEDIUM:
            id = self.medium_index_word(variants)
        elif self.__level__ == HARD:
            id = self.hard_index_word(variants)
        else:
            id = self.hardest_index_word(variants, symbols, not_allowed_words)

        if id == -1:
            self.don_t_make_move.emit()

        while not self.__temp_committed__:
            cnt = 0
            for coordinate in variants[id].coordinates:
                x = coordinate.x - 1
                y = coordinate.y - 1
                if self.__board__[x][y] == '-':
                    used_x = x
                    used_y = y
                    c = variants[id].possible_word[cnt]
                cnt += 1

            res = CellLetter(used_x, used_y, c)
            self.choose_letter.emit(res)

            for coordinate in variants[id].coordinates:
                x = coordinate.x - 1
                y = coordinate.y - 1
                coordinates = Coordinates(x, y)
                self.push_letter.emit(coordinates)

            self.commit_word.emit()

            if self.__temp_committed__:
                self.__temp_committed__ = False
                return
            variants.pop(id)

            if self.__level__ == EASY:
                id = self.easy_index_word(variants)
            elif self.__level__ == MEDIUM:
                id = self.medium_index_word(variants)
            elif self.__level__ == HARD:
                id = self.hard_index_word(variants)
            else:
                id = self.hardest_index_word(variants, symbols, not_allowed_words)

            if id == -1:
                self.don_t_make_move.emit()



    def not_belong(self, not_allowed_words, check_in):
        for word in not_allowed_words:
            if word == check_in:
                return False
        return True

    def dfs(self, table, words, x, y, cur_position, cur_used, cur_string, cur_coords, used_empty):
        pass

    def possible_variants(self, table):
        N = len(table) - 2
        M = len(table[0]) - 2

        words = list()
        cur_string = str()
        for i in range(1, N + 1):
            for j in range(1, M + 1):
                cur_used = [[False for k in range(M + 2)] for l in range(N + 2)]
                cur_coordinates = list()
                cur_position = 0
                if table[i][j] != '-' and self.__bor_vocabulary__.bor_vertices[cur_position].find_children(table[i][j]) != -1:
                    self.dfs(table, self.words, i, j,
                             self.__bor_vocabulary__[cur_position].find_children(table[i][j]),
                             cur_used, cur_string, cur_coordinates, False)
                else:
                    if table[i][j] == '-':
                        self.dfs(table, words, i, j, PRE_VERTEX, cur_used, cur_string, cur_coordinates, False)

        corrects = list()

        for word in words:
            corrects.append(word)

        return corrects



