from random import Random
from Dictionary import Dictionary
from Letter import CellLetter, Coordinates
from Player import Player
from bot.Bor import Bor, PRE_VERTEX, NOT_FOUND
from bot.Word import Word
from lang.Language import Language

__author__ = 'akhtyamovpavel'

from PySide import QtCore

EASY = 1
MEDIUM = 2
HARD = 3
HARDEST = 100500

DEFAULT_WIDTH = 5
DEFAULT_HEIGHT = 5


class Bot(Player):
    __get_dictionary__ = QtCore.Signal()
    __get_used_words__ = QtCore.Signal()
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

    @QtCore.Slot()
    def get_used_dictionary(self, used_words):
        self.__not_allowed_words__ = used_words

    def __init__(self, language: Language, width=DEFAULT_WIDTH, height=DEFAULT_HEIGHT):
        super(Bot, self).__init__()
        self.__language__ = language
        self.__width__ = width
        self.__height__ = height
        self.__moves__ = [(0, 1), (0, -1), (1, 0), (-1, 0)]

        self.__level__ = EASY #default value
        self.__bor_vocabulary__ = Bor()
        self.__not_allowed_words__ = set()

    def maximal_length(self, variants):
        if len(variants) == 0:
            return 0
        return max(len(variant.__possible_word__) for variant in variants)

    def allowed_variants(self, variants, not_allowed_word):
        return [variant for variant in variants if variant not in self.__not_allowed_words__ and variant != not_allowed_word]


    def easy_index_word(self, variants):
        print("easy")
        size = len(variants)
        random = Random()
        random_number = int(size * random.random())
        return min(random_number, size - 1)

    def medium_index_word(self, variants):
        print("medium")
        size = len(variants)
        val = 0
        for i in range(size):
            val += len(variants[i].__possible_word__) * len(variants[i].__possible_word__)

        random = Random()
        random_number = int(random.random() * val)

        cur = 0
        cnt = 0
        for variant in variants:
            cur += len(variant.__possible_word__) * len(variant.__possible_word__)
            if cur > random_number:
                return cnt
            cnt += 1

        return size - 1


    def hard_index_word(self, variants):
        print("hard")
        size = len(variants)
        if (size == 0):
            return -1

        max_len = self.maximal_length(variants)
        maximal_variants = [variant for variant in variants if len(variant.__possible_word__) == max_len]
        maximal_size = len(maximal_variants)
        if (maximal_size == 0):
            return -1

        random = Random()
        random_number = min(maximal_size - 1, int(maximal_size * random.random()))
        for cnt in range(size):
            if variants[cnt] == maximal_variants[random_number]:
                return cnt

        return -1

    def hardest_index_word(self, variants, symbols):
        best_index = -1
        max_score = -self.__width__ * self.__height__

        max_lens = {len(variant.__possible_word__) for variant in variants}
        for i in range(3):
            if (len(max_lens) > 0):
                min_len = max(max_lens)
            max_lens.remove(min_len)
        maximal_variants = [variant for variant in variants if len(variant.__possible_word__) >= min_len]

        for index in range(len(maximal_variants)):
            for i in range(len(maximal_variants[index].__coordinates__)):
                x = maximal_variants[index].__coordinates__[i].x
                y = maximal_variants[index].__coordinates__[i].y
                if symbols[x][y] == '-':
                    used_x = x
                    used_y = y
                    c = maximal_variants[index].__possible_word__[i]

            symbols[used_x][used_y] = c
            new_variants = self.possible_variants(symbols)
            symbols[used_x][used_y] = '-'
            new_allowed_variants = self.allowed_variants(new_variants, maximal_variants[index].__possible_word__)

            bot_score = len(maximal_variants[index].__possible_word__)
            player_score = self.maximal_length(new_allowed_variants)
            if (bot_score - player_score > max_score):
                max_score = bot_score - player_score
                best_index = index

        if (best_index == -1):
            return -1

        for cnt in range(len(variants)):
            if variants[cnt] == maximal_variants[best_index]:
                return cnt
        return -1


    def set_level(self, difficulty):
        self.__level__ = difficulty

    def connect_to_dictionary(self, dictionary: Dictionary):
        self.__get_dictionary__.connect(dictionary.send_dictionary)

    def get_dictionary(self):
        self.__get_dictionary__.emit()

    def connect_to_used_dictionary(self, dictionary: Dictionary):
        self.__get_used_words__.connect(dictionary.send_used_words)

    def begin_step(self):
        self.run_process()

    def run_process(self):
        self.show_board.emit()
        self.__get_used_words__.emit()
        print(self.__not_allowed_words__)

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
            id = self.hardest_index_word(variants, symbols[:])

        if id == -1:
            self.don_t_make_move.emit()

        while not self.__temp_committed__:
            cnt = 0
            for coordinate in variants[id].__coordinates__:
                x = coordinate.x - 1
                y = coordinate.y - 1
                if self.__board__[x][y] == '-':
                    print(cnt)
                    used_x = x
                    used_y = y
                    c = variants[id].__possible_word__[cnt]
                cnt += 1

            res = CellLetter(used_x, used_y, c)
            self.choose_letter.emit(res)

            for coordinate in variants[id].__coordinates__:
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
                id = self.hardest_index_word(variants, symbols[:])

            if id == -1:
                self.don_t_make_move.emit()



    def not_belong(self, not_allowed_words, check_in):
        for word in not_allowed_words:
            if word == check_in:
                return False
        return True

    def dfs(self, table, words, x, y, cur_position, cur_used, cur_string, cur_coords, used_empty):
        cur_used[x][y] = True
        if self.__bor_vocabulary__.bor_vertices[cur_position].is_leaf() and used_empty:
            cur_coords.append(Coordinates(x, y))
            cur_string += table[x][y]
            words.append(Word(cur_string, cur_coords))
            cur_string = cur_string[:-1]
            cur_coords.pop()

        if table[x][y] == '-' and not used_empty:
            letters = self.__language__.get_list()
            for letter in letters:
                table[x][y] = letter

                if self.__bor_vocabulary__.bor_vertices[cur_position].find_children(letter) != -1:
                    self.dfs(table, words, x, y,
                             self.__bor_vocabulary__.bor_vertices[cur_position].find_children(letter),
                             cur_used, cur_string, cur_coords, True)
            table[x][y] = '-'
        else:
            for (dx, dy) in self.__moves__:
                xx = x + dx
                yy = y + dy
                if table[xx][yy] == '#':
                    continue
                if table[xx][yy] == '-' and used_empty:
                    continue
                if cur_used[xx][yy]:
                    continue

                cur_coords.append(Coordinates(x, y))
                cur_string += table[x][y]

                if table[xx][yy] != '-':
                    if self.__bor_vocabulary__.bor_vertices[cur_position].find_children(table[xx][yy]) != NOT_FOUND:
                        self.dfs(table, words, xx, yy,
                            self.__bor_vocabulary__.bor_vertices[cur_position].find_children(table[xx][yy]),
                            cur_used, cur_string, cur_coords, used_empty)
                else:
                    self.dfs(table, words, xx, yy, cur_position, cur_used, cur_string, cur_coords, used_empty)

                cur_string = cur_string[:-1]
                cur_coords.pop()

        cur_used[x][y] = False


    def possible_variants(self, table):
        N = len(table) - 2
        M = len(table[0]) - 2
        words = list()

        for i in range(1, N + 1):
            for j in range(1, M + 1):
                cur_used = [[False for k in range(M + 2)] for l in range(N + 2)]
                cur_coordinates = list()
                cur_string = str()
                cur_position = 0
                if table[i][j] != '-' and self.__bor_vocabulary__.bor_vertices[cur_position].find_children(table[i][j]) != -1:
                    self.dfs(table, words, i, j,
                             self.__bor_vocabulary__.bor_vertices[cur_position].find_children(table[i][j]),
                             cur_used, cur_string, cur_coordinates, False)
                elif table[i][j] == '-':
                        self.dfs(table, words, i, j, PRE_VERTEX, cur_used, cur_string, cur_coordinates, False)

        corrects = list()

        for word in words:
            corrects.append(word)

        return corrects



