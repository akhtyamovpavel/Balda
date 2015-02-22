from PySide.QtCore import QObject

__author__ = 'user1'


class WordCollector(QObject):

    def __init__(self):
        super(WordCollector, self).__init__()
