from balda_game.lib.field.Letter import CellLetter

__author__ = 'akhtyamovpavel'

class Move:

    def __init__(self):
        self.added_letter = CellLetter()
        self.word_structure = list()

    def get_added_letter(self):
        return self.added_letter

    def set_word_structure(self, word_structure):
        self.word_structure = word_structure

    def get_word_structure(self, word_structure):
        return self.word_structure

    def get_word(self):
        word = ""
        for cell_letter in self.word_structure:
            word += cell_letter.letter
        return word

    def get_word_length(self):
        return len(self.get_word())

