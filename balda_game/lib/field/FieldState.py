from balda_game.lib.field.CellState import FIXED, SPARE
from balda_game.lib.field.CellState import CellState

__author__ = 'akhtyamovpavel'


class FieldState:

    def __init__(self, width, height, word):
        self._board_ = [[CellState(SPARE) for i in range(width)] for i in range(height)]
        self._board_[height//2] = [CellState(FIXED, letter) for letter in word]

    def get_copy(self):
        return self._board_

    def set_state(self, height_level, width_level, state, letter=None):
        current_cell_state = self._board_[height_level][width_level]
        current_cell_state.set_cell(state, letter)
        self._board_[height_level][width_level] = current_cell_state

    def get_letter_state(self, height_level, width_level):
        return self._board_[height_level][width_level].get_cell()

    def get_letter(self, height_level, width_level):
        return self._board_[height_level][width_level].get_letter()