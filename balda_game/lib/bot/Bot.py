import random
from balda_game.lib.bot.Bor import Bor, PRE_VERTEX, NOT_FOUND
from balda_game.lib.bot.Level import get_bot_by_level, Level
from balda_game.lib.bot.Word import Word
from balda_game.lib.dictionary.SingletonDictionary import dictionary
from balda_game.lib.field.Letter import CellLetter, Coordinates
from balda_game.lib.lang.RussianLanguage import RussianLanguage

__author__ = 'akhtyamovpavel'

EASY = 1
MEDIUM = 2
HARD = 3
HARDEST = 100500

DEFAULT_WIDTH = 5
DEFAULT_HEIGHT = 5


class Bot:
    def __init__(self, parent, game_id, width=DEFAULT_WIDTH, height=DEFAULT_HEIGHT):

        self.game_id = game_id
        self.__language__ = RussianLanguage()
        self.__width__ = width
        self.__height__ = height
        self.__moves__ = [(0, 1), (0, -1), (1, 0), (-1, 0)]

        self.__level__ = Level.EASY  # default value
        self.__bor_vocabulary__ = Bor()
        self.__not_allowed_words__ = set()
        self.parent = parent
        self.setup_dictionary_for_bot()

    def setup_dictionary_for_bot(self):
        word_list = dictionary.get_words()
        for word in word_list:
            self.__bor_vocabulary__.add_word(word)

    def maximal_length(self, variants):
        if len(variants) == 0:
            return 0
        return max(len(variant.__possible_word__) for variant in variants)

    def allowed_variants(self, variants, not_allowed_word):
        return [variant for variant in variants if
                variant not in self.__not_allowed_words__ and variant != not_allowed_word]

    def easy_index_word(self, variants):
        print("easy")
        size = len(variants)
        random_number = random.randint(0, size - 1)
        return random_number

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

    def run_process(self):
        field = self.parent.get_field(self.game_id)

        self.__used_words__ = dictionary.get_used_words(self.game_id)

        print(self.__not_allowed_words__)

        symbols = [['#' for i in range(self.__width__ + 2)] for j in range(self.__height__ + 2)]

        for i in range(1, self.__height__ + 1):
            for j in range(1, self.__width__ + 1):
                symbols[i][j] = field.get_letter(i - 1, j - 1)

        variants = self.possible_variants(symbols)

        is_committed = False

        id = -1

        if self.__level__ == Level.EASY:
            id = self.easy_index_word(variants)
        elif self.__level__ == Level.MEDIUM:
            id = self.medium_index_word(variants)
        elif self.__level__ == Level.HARD:
            id = self.hard_index_word(variants)
        else:
            id = self.hardest_index_word(variants, symbols[:])

        if id == -1:
            return False

        # TODO Make board
        while not is_committed:
            cnt = 0
            for coordinate in variants[id].__coordinates__:
                x = coordinate.x
                y = coordinate.y
                if symbols[x][y] == '.':
                    print(cnt)
                    used_x = x - 1
                    used_y = y - 1
                    c = variants[id].__possible_word__[cnt]
                cnt += 1

            heights = [coordinate.x - 1 for coordinate in variants[id].__coordinates__]
            widths = [coordinate.y - 1 for coordinate in variants[id].__coordinates__]

            if self.parent.commit_word(self.game_id, pinned_height=used_x, pinned_width=used_y, pinned_letter=c,
                                       heights=heights, widths=widths, word=variants[id].__possible_word__,
                                       user=get_bot_by_level(self.__level__)):
                return True

            variants.pop(id)

            if self.__level__ == Level.EASY:
                id = self.easy_index_word(variants)
            elif self.__level__ == Level.MEDIUM:
                id = self.medium_index_word(variants)
            elif self.__level__ == Level.HARD:
                id = self.hard_index_word(variants)
            else:
                id = self.hardest_index_word(variants, symbols[:])

            if id == -1:
                return False

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

        if table[x][y] == '.' and not used_empty:
            letters = self.__language__.get_list()
            for letter in letters:
                table[x][y] = letter

                if self.__bor_vocabulary__.bor_vertices[cur_position].find_children(letter) != -1:
                    self.dfs(table, words, x, y,
                             self.__bor_vocabulary__.bor_vertices[cur_position].find_children(letter),
                             cur_used, cur_string, cur_coords, True)
            table[x][y] = '.'
        else:
            for (dx, dy) in self.__moves__:
                xx = x + dx
                yy = y + dy
                if table[xx][yy] == '#':
                    continue
                if table[xx][yy] == '.' and used_empty:
                    continue
                if cur_used[xx][yy]:
                    continue

                cur_coords.append(Coordinates(x, y))
                cur_string += table[x][y]

                if table[xx][yy] != '.':
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
                if table[i][j] != '-' and self.__bor_vocabulary__.bor_vertices[cur_position].find_children(
                        table[i][j]) != -1:
                    self.dfs(table, words, i, j,
                             self.__bor_vocabulary__.bor_vertices[cur_position].find_children(table[i][j]),
                             cur_used, cur_string, cur_coordinates, False)
                elif table[i][j] == '-':
                    self.dfs(table, words, i, j, PRE_VERTEX, cur_used, cur_string, cur_coordinates, False)

        corrects = list()

        for word in words:
            corrects.append(word)

        return corrects
