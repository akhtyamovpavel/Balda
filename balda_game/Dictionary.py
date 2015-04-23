from os import getcwd
from random import Random
import unittest
from PySide import QtCore, QtSql
from sqlite3 import connect

__author__ = 'akhtyamovpavel'

GET_WORDS_QUERY = "SELECT word FROM Words WHERE id = root_id"
TEST_QUERY = "SELECT 1"
CHECK_WORD_QUERY = "SELECT word FROM Words WHERE (id = root_id AND word = ?)"



class Dictionary(QtCore.QObject):




    sz = 100
    send_dictionary_pool = list()


    pool_dictionary = dict()

    signal_mapper = QtCore.QSignalMapper()

    def __init__(self):
        super(Dictionary, self).__init__()
        self.__used_words__ = list()
        self.db = QtSql.QSqlDatabase()


        self.random = Random()

    def init_dictionary(self):
        self.db = connect(getcwd() + "/dictionary.db")

    def load_dictionary(self):
        pass

    def setup_connection(self, game_id):

        pool_size = len(self.pool_dictionary)
        self.pool_dictionary[game_id] = pool_size
        self.__used_words__.append(set())


    def get_first_word(self, width):

        first_word_list = list()
        cursor = self.db.cursor()
        cursor.execute(GET_WORDS_QUERY)
        for row in cursor:
            word = row[0]
            if len(word) == width:
                first_word_list.append(word)
        print(len(first_word_list))
        first_word = first_word_list[int(self.random.random()*len(first_word_list))]
        return first_word

    # TODO This method is buggy, need to rewrite in pool way
    def connect_to_bot(self, bot):
        self.send_dictionary.connect(bot.setup_dictionary)


    def is_word_correct_built(self, _x_list_, _y_list_, _changed_cell_):
        if len(self._x_list_) == 0:
            return False
        _is_approved_ = True
        cnt_new = 0
        for i in range(1, len(_x_list_)):
            if abs(_x_list_[i] - _x_list_[i - 1]) + abs(_y_list_[i] - _y_list_[i - 1]) != 1:
                _is_approved_ = False


        for i in range(len(self._x_list_)):
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
        number_id = self.pool_dictionary.get(self.sender())
        if number_id is None:
            return

        if not self.is_word_correct_built(x_list, y_list, changed_cell):
            return False


        return  self.is_word_good(word, number_id)

    def is_word_good(self, word, number_id):
        cursor = self.db.cursor()
        cursor.execute(CHECK_WORD_QUERY, (word))

        for row in cursor:
            current_set = self.__used_words__[number_id]
            if word not in current_set:
                current_set.add(word)
                self.__used_words__[number_id] = current_set
                print("WORD FOUND")
                return True
            else:
                print("WORD NOT FOUND")
                return False
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
