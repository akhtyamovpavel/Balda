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
        self.clicked.connect(self.on_letter_clicked)

        self.setSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Preferred)
        self.create_menu()


    def create_menu(self):
        self.signal_mapper = QtCore.QSignalMapper(self)
        self.__menu__ = QtGui.QMenu('Set of letters')

        self.__actions__ = list()

        current_language = self.__language__.get_list()

        for letter in current_language:
            action = QtGui.QAction(letter, self)


            self.__menu__.addAction(action)
            self.signal_mapper.setMapping(action, letter)

            action.triggered.connect(self.signal_mapper.map)
            self.__actions__.append(action)

        self.signal_mapper.mapped[str].connect(self.set_letter)
        self.setMenu(self.__menu__)

    def connect_to_panel(self, gb):
        self.on_letter_chosen.connect(gb.on_cell_chosen)
        self.on_letter_pushed.connect(gb.on_cell_pushed)

