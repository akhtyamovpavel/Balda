import unittest
from bot.BorLetter import Letter

__author__ = 'akhtyamovpavel'

NOT_FOUND = -1
PRE_VERTEX = 0

class Bor:
    def __init__(self):
        self.bor_vertices = list()
        self.bor_vertices.append(Letter())


    def add_letter(self, position, symbol):
        new_letter = Letter(symbol, False)
        self.bor_vertices.append(new_letter)
        self.bor_vertices[position].add_children(len(self.bor_vertices) - 1, symbol)

    def add_word(self, word):
        index_at_the_end_of_word = 0
        for i in range(len(word)):
            current_letter = word[i]
            children_vertex = self.bor_vertices[index_at_the_end_of_word].find_children(current_letter)
            if children_vertex == NOT_FOUND:
                self.add_letter(index_at_the_end_of_word, current_letter)
                index_at_the_end_of_word = len(self.bor_vertices) - 1
            else:
                index_at_the_end_of_word = children_vertex

        self.bor_vertices[index_at_the_end_of_word].set_leaf(True)

    def not_belong(self, not_allowed_words, check_in):
        # TODO: it's not used
        pass


class BorTest(unittest.TestCase):
    def setUp(self):
        self.bor = Bor()

    def test(self):
        self.bor.add_word('mama')
        self.bor.add_word('papa')
        self.bor.add_word('milk')
        self.assertEqual(len(self.bor.bor_vertices), 12, "Fail")

        self.bor.add_word('papaya')

        self.assertEqual(len(self.bor.bor_vertices), 14, "Fail")

if __name__ == '__main__':
    unittest.main()