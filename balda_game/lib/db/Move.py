from balda_game.lib.field.Letter import CellLetter

__author__ = 'akhtyamovpavel'

class Move:

    def __init__(self):
        self._added_letter = CellLetter()
        self._word_structure = list()

    def get_added_letter(self):
        return self._added_letter

    def set_added_letter(self, added_letter):
        self._added_letter = added_letter

    def set_word_structure(self, word_structure):
        self._word_structure = word_structure

    def get_word_structure(self):
        return self._word_structure

    def get_word(self):
        word = ""
        for cell_letter in self._word_structure:
            word += cell_letter.letter
        return word

    def get_word_length(self):
        return len(self.get_word())

