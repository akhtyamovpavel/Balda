__author__ = 'user1'


class CellLetter:
    def __init__(self, x=None, y=None, letter=None):
        self.x = x
        self.y = y
        self.letter = letter

    def set(self, x, y, letter):
        self.x = x
        self.y = y
        self.letter = letter

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.letter == other.letter

class Coordinates:
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y
