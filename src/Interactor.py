from os import path
import os
import time


__author__ = 'akhtyamovpavel'

class Interactor:
    game_sources = dict()
    game_last_command = dict()
    read_log_path = 'log'
    write_log_path = 'results'

    def __init__(self):
        if not path.exists('log'):
            os.mkdir('log')
        if not path.exists('results'):
            os.mkdir('results')

    def process_command(self):
        from interface.GraphicBoard import create_simple_graphic_board
        if not path.exists('log'):
            os.mkdir('log')
        while(True):
            list_files = os.listdir('log')
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
                    self.game_last_command[game_id] = 0
                    gb = create_simple_graphic_board(game_id)
                    self.game_sources[game_id] = gb

                    print(gb.__game_manager__.__first_word__)
                elif command == 'add':
                    gb = self.game_sources.get(game_id)
                    if gb is None:
                        print("Game isn't started")
                    x, y = int(list_log[counter]), int(list_log[counter + 1])
                    letter = list_log[counter + 2]
                    gb.buttons[x][y].set_letter(letter)
                elif command == 'print':
                    gb = self.game_sources.get(game_id)
                    if gb is None:
                        print("Game isn't started")
                    for i in range(5):
                        for j in range(5):
                            print(gb.buttons[i][j].text(), end=' ')
                        print()
                elif command == 'push':
                    gb = self.game_sources.get(game_id)
                    if gb is None:
                        print("Game isn't started")
                    x, y = int(list_log[counter]), int(list_log[counter + 1])
                    gb.buttons[x][y].on_letter_clicked()
                elif command == 'commit':
                    gb = self.game_sources.get(game_id)
                    gb.on_commit_button_clicked()
                elif command == 'exit':
                    break
                else:
                    print("Wrong command")


    def get_instance_id(self, game_instance):
        return game_instance.get_game_id()


    def print_command(self, game_instance, name_command, **kwargs):
        value = self.get_instance_id(game_instance)
        if value == -1:
            # TODO throw exception
            return
        else:
            command_number = self.game_last_command.get(value)
            self.game_last_command[value] += command_number + 1
            dir_path = path.join(self.write_log_path, str(value))
            if not path.exists(dir_path):
                os.mkdir(dir_path)

            file = open(path.join(dir_path, '.'.join([str(command_number), 'txt'])), 'w')
            file.writelines([name_command, '\n', kwargs.__repr__()])
            file.close()



    def end_game(self, game_instance, **kwargs):
        self.print_command(game_instance, 'finish', kwargs)
        self.game_sources.pop(game_instance)


    def on_commit_result(self, game_instance, **kwargs):
        self.print_command(game_instance, 'commit', **kwargs)

    def on_game_started(self, game_instance, **kwargs):
        self.print_command(game_instance, 'start', **kwargs)

    def on_letter_chosen(self, game_instance, **kwargs):
        self.print_command(game_instance, 'choose', **kwargs)

    def on_letter_pushed(self, game_instance, **kwargs):
        self.print_command(game_instance, 'push', **kwargs)

    def on_word_reset(self, game_instance, **kwargs):
        self.print_command(game_instance, 'reset', kwargs)


