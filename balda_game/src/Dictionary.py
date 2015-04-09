from os import getcwd
from random import Random
import unittest
from PySide import QtCore, QtSql

__author__ = 'akhtyamovpavel'

GET_WORDS_QUERY = "SELECT word FROM Words WHERE id = root_id"
TEST_QUERY = "SELECT 1"
CHECK_WORD_QUERY = "SELECT word FROM Words WHERE (id = root_id AND :word = word)"

class Dictionary(QtCore.QObject):

    send_check_result = QtCore.Signal(int)
    send_dictionary = QtCore.Signal(list)

    def __init__(self):
        super(Dictionary, self).__init__()
        self.__set_of_words__ = set()
        self.__used_words__ = set()
        self.db = QtSql.QSqlDatabase()


        self.random = Random()

    def load_dictionary(self):
        print(getcwd())
        self.db = QtSql.QSqlDatabase.addDatabase("QSQLITE")
        self.db.setDatabaseName(getcwd() + "/dictionary.db")
        if not self.db.open():
            print("ERROR: database not loaded")
        else:
            print("Base loaded")

    def setup_connection(self, word_collector):
        self.send_check_result.connect(word_collector.set_word_approved)

    def get_first_word(self, width):

        first_word_list = list()
        query = QtSql.QSqlQuery(self.db)
        query.exec_(GET_WORDS_QUERY)
        while query.next():
            word = str(query.value(0))
            if len(word) == width:
                first_word_list.append(word)
        print(len(first_word_list))
        first_word = first_word_list[int(self.random.random()*len(first_word_list))]
        return first_word


    def connect_to_bot(self, bot):
        self.send_dictionary.connect(bot.setup_dictionary)

    @QtCore.Slot(str)
    def check_word(self, word):
        if self.is_word_good(word):
            self.send_check_result.emit(1)
        else:
            self.send_check_result.emit(0)


    def is_word_good(self, word):
        query = QtSql.QSqlQuery(self.db)
        query.prepare(CHECK_WORD_QUERY)
        query.bindValue(":word", word)
        if not query.exec_():
            print("BAD")
            return
        if query.next():
            if word not in self.__used_words__:
                self.__used_words__.add(word)
                print("WORD FOUND")
                return True
            else:
                print("WORD NOT FOUND")
                return False
        else:
            print("WORD NOT FOUND")
            return False


    @QtCore.Slot()
    def send_dict(self):
        words = list()
        query = QtSql.QSqlQuery(self.db)
        query.exec_(GET_WORDS_QUERY)

        while query.next():
            words.append(str(query.value(0)))
        print(words)

        self.send_dictionary.emit(words)


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
