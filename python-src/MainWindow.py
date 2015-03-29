from PySide import QtGui, QtCore
from interface.GraphicBoard import GraphicBoard

__author__ = 'akhtyamovpavel'

class MainWindow(QtGui.QMainWindow):
    
    def __init__(self):
        super(MainWindow, self).__init__()
        self.__width_letters__ = 5
        self.__height_letters__ = 5

        self.create_actions()
        self.create_menus()
        self.rules()
        self.gb = None

        #settings

        self.resize(600, 400)

    def create_actions(self):
        self.new_game = QtGui.QAction('New game', self)
        self.new_game.triggered.connect(self.start_new_game)
        self.exit_game = QtGui.QAction('Exit', self)
        self.exit_game.triggered.connect(QtGui.qApp.quit)

    def create_menus(self):
        self.menu = QtGui.QMenu('Game', self)
        self.menu.addAction(self.new_game)
        self.menu.addSeparator()
        self.menu.addAction(self.exit_game)

        self.menuBar().addMenu(self.menu)

    def rules(self):
        self.label = QtGui.QLabel(self)
        self.label.setText("OK")
        self.setCentralWidget(self.label)

    def get_height(self):
        return self.__height_letters__

    def get_width(self):
        return self.__width_letters__

    @QtCore.Slot()
    def start_new_game(self):
        self.gb = GraphicBoard(width=self.__width_letters__, height=self.__height_letters__, parent=self)
        self.setCentralWidget(self.gb)

    @QtCore.Slot()
    def reset_field(self):
        self.setCentralWidget(None)
        self.rules()


