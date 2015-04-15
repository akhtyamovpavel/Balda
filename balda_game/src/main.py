import os
from threading import Thread
from PySide import QtGui
import sys
from balda_game.src.MainWindow import MainWindow
from balda_game.src.interface.GraphicBoard import GraphicBoard


__author__ = 'akhtyamovpavel'


game_sources = dict()





def input_game():
    while(True):
        game_id = int(input())
        command = input()
        if command == 'start':

            gb = GraphicBoard(5, 5, game_lang='Russian', players=2, parent=None)
            game_sources[game_id] = gb
            print(gb.__game_manager__.__first_word__)
        elif command == 'add':
            gb = game_sources.get(game_id)
            if gb is None:
                print("Game isn't started")
            x, y = map(int, input().split())
            letter = input()
            gb.buttons[x][y].set_letter(letter)
        elif command == 'print':
            gb = game_sources.get(game_id)
            if gb is None:
                print("Game isn't started")
            for i in range(5):
                for j in range(5):
                    print(gb.buttons[i][j].text(), end=' ')
                print()
        elif command == 'push':
            gb = game_sources.get(game_id)
            if gb is None:
                print("Game isn't started")
            x, y = map(int, input().split())
            gb.buttons[x][y].on_letter_clicked()
        elif command == 'commit':
            gb = game_sources.get(game_id)
            gb.on_commit_button_clicked()
        else:
            print("Wrong command")



if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)



    read_thread = Thread(target=input_game)
    read_thread.daemon = True
    read_thread.start()
    sys.exit(app.exec_())