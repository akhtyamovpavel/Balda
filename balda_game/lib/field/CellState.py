__author__ = 'akhtyamovpavel'


FIXED = 0
PINNED = 1
SPARE = 2

class CellState:

    def __init__(self, state, letter=None):
        self._state_ = state
        if state == FIXED:
            self._letter_ = letter
        else:
            self._letter_ = '.'

    def get_cell(self):
        return [self._state_, self._letter_]

    def get_letter(self):
        return self._letter_



    def set_cell(self, state, letter=None):
        """
        Set cell current state
        :param state: state of cell
        :param letter: used if state is FIXED or PINNED
        :return:
        """
        self._state_ = state
        if self._state_ != SPARE:
            self._letter_ = letter