from PySide import QtGui
import sys
from MainWindow import MainWindow

__author__ = 'akhtyamovpavel'

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    w = MainWindow()
    w.show()

    sys.exit(app.exec_())