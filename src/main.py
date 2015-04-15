import os
from os import path
from threading import Thread
import sys

from PySide import QtGui
import time

from interface.GraphicBoard import GraphicBoard



__author__ = 'akhtyamovpavel'



game_sources = dict()





def input_game():
    if not path.exists('log'):
        os.mkdir('log')
    while(True):
        list_files = os.listdir(path='log')
        print(list_files)
        if len(list_files) == 0:
            time.sleep(10)
        for file_name in list_files:
            f = open(path.join('log', file_name))
            list_log = f.readline().split()
            f.close()
            os.remove(path.join('log', file_name))
            if len(list_log) == 0:
                continue
            print(list_log)
            for i in range(len(list_log)):
                list_log[i] = list_log[i].strip()


            if (list_log[0] != 'BaldaLogGame'):
                continue


            game_id = int(list_log[1])
            command = list_log[2].strip()
            counter = 3
            if command == 'start':

                gb = GraphicBoard(5, 5, game_lang='Russian', players=2, parent=None)
                game_sources[game_id] = gb
                print(gb.__game_manager__.__first_word__)
            elif command == 'add':
                gb = game_sources.get(game_id)
                if gb is None:
                    print("Game isn't started")
                x, y = int(list_log[counter]), int(list_log[counter + 1])
                letter = list_log[counter + 2]
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
                x, y = int(list_log[counter]), int(list_log[counter + 1])
                gb.buttons[x][y].on_letter_clicked()
            elif command == 'commit':
                gb = game_sources.get(game_id)
                gb.on_commit_button_clicked()
            elif command == 'exit':
                break
            else:
                print("Wrong command")



if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)

    read_thread = Thread(target=input_game)
    read_thread.daemon = True
    read_thread.start()
    sys.exit(app.exec_())