from threading import Thread
import sys

from PySide import QtGui
from Pool import factory


__author__ = 'akhtyamovpavel'


if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)

    read_thread = Thread(target=factory.process_command)
    read_thread.daemon = True
    read_thread.start()
    sys.exit(app.exec_())