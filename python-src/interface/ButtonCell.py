__author__ = 'akhtyamovpavel'

from PySide import QtCore, QtGui

class ButtonCell(QtGui.QToolButton):
    on_letter_chosen = QtCore.Signal(str)
    on_letter_pushed = QtCore.Signal()


    @QtCore.Slot(str)
    def set_letter(self, letter):
        self.on_letter_chosen.emit(letter)

    @QtCore.Slot()
    def on_letter_clicked(self):
        self.on_letter_pushed.emit()

    def __init__(self, language):
        super(ButtonCell, self).__init__()
        self.__language__ = language
        self.clicked().connect(self.on_letter_clicked)

        self.setSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Preferred)
        self.create_menu()


    def create_menu(self):
        self.__signal_mapper__ = QtCore.QSignalMapper(self)
        self.__menu__ = QtGui.QMenu('Set of letters')

        self.__actions__ = list()

        current_language = self.__language__.get_list()

        for letter in current_language:
            self.__actions__.append(QtGui.QAction(letter, self))
            self.__menu__.addAction(self.__actions__[-1])
            self.__signal_mapper__.setMapping(self.__actions__[-1], letter)
            self.__actions__[-1].triggered.connect(self.__signal_mapper__.map)

        self.__signal_mapper__.mapped().connect(self.set_letter)


    def connect_to_panel(self, gb):
        pass

