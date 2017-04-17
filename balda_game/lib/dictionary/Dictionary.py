from os import getcwd
from random import Random
from functools import wraps
import unittest
from sqlite3 import connect

__author__ = 'akhtyamovpavel'

GET_WORDS_QUERY = "SELECT word FROM Words WHERE id = root_id"
TEST_QUERY = "SELECT 1"
CHECK_WORD_QUERY = "SELECT word FROM Words WHERE (id = root_id AND word = ?)"


class Dictionary():
    sz = 100

    pool_dictionary = dict()
    used_words = dict()

    def __init__(self):
        super(Dictionary, self).__init__()

        self.random = Random()

    def init_dictionary(self):
        self.db = connect(getcwd() + "/dictionary.db")

    def close_connection(self):
        self.db.close()

    def load_dictionary(self):
        pass

    def setup_connection(self, game_id):
        pool_size = len(self.pool_dictionary)
        self.pool_dictionary[game_id] = pool_size
        self.used_words[game_id] = set()

    def pin_first_word(self, game_id, word):
        if self.used_words.get(game_id) is not None:
            current_set = self.used_words.get(game_id)
            current_set.add(word)
            self.used_words[game_id] = current_set

    def get_first_word(self, width):
        # TODO Realize with decorators
        self.init_dictionary()
        first_word_list = list()
        cursor = self.db.cursor()
        cursor.execute(GET_WORDS_QUERY)
        for row in cursor:
            word = row[0]
            if len(word) == width:
                first_word_list.append(word)
        print(len(first_word_list))
        first_word = first_word_list[int(self.random.random() * len(first_word_list))]
        self.close_connection()
        return first_word

    def get_words(self):
        self.init_dictionary()
        cursor = self.db.cursor()
        cursor.execute(GET_WORDS_QUERY)
        return [row[0] for row in cursor]

    def get_used_words(self, game_id):
        return self.used_words.get(game_id)

    def is_word_correct_built(self, _x_list_, _y_list_, _changed_cell_):
        if len(_x_list_) == 0:
            return False
        _is_approved_ = True
        cnt_new = 0
        for i in range(1, len(_x_list_)):
            if abs(_x_list_[i] - _x_list_[i - 1]) + abs(_y_list_[i] - _y_list_[i - 1]) != 1:
                _is_approved_ = False

        for i in range(len(_x_list_)):
            if _x_list_[i] == _changed_cell_.x and _y_list_[i] == _changed_cell_.y:
                cnt_new += 1
        if cnt_new != 1:
            _is_approved_ = False

        for i in range(0, len(_x_list_)):
            for j in range(i + 1, len(_y_list_)):
                if _x_list_[i] == _x_list_[j] and _y_list_[i] == _y_list_[j]:
                    _is_approved_ = False

        if _is_approved_:
            return True

        return False

    def check_word(self, number_id, x_list, y_list, changed_cell, word):
        self.init_dictionary()

        if number_id is None:
            return False

        if not self.is_word_correct_built(x_list, y_list, changed_cell):
            return False

        value = self.is_word_good(word, number_id)
        self.close_connection()
        return value

    def is_word_good(self, word, number_id):
        self.init_dictionary()
        cursor = self.db.cursor()
        cursor.execute(CHECK_WORD_QUERY, (word,))

        for row in cursor:
            current_set = self.used_words.get(number_id)
            if word not in current_set:
                current_set.add(word)
                self.used_words[number_id] = current_set
                print("WORD FOUND")
                self.close_connection()
                return True
            else:
                print("WORD NOT FOUND")
                self.close_connection()
                return False
        self.close_connection()
        print("WORD NOT FOUND")
        return False


class DictionaryTest(unittest.TestCase):
    def setUp(self):
        self.dictionary = Dictionary()
        self.dictionary.load_dictionary()

    def test_check(self):
        self.assertTrue(self.dictionary.is_word_good("МАМА"))
        self.assertTrue(self.dictionary.is_word_good("ПАПА"))

    def test_first_word(self):
        for i in range(10):
            self.assertTrue(self.dictionary.is_word_good(self.dictionary.get_first_word(5)))

        for i in range(10):
            self.assertTrue(self.dictionary.is_word_good(self.dictionary.get_first_word(6)))


if __name__ == '__main__':
    unittest.main()
