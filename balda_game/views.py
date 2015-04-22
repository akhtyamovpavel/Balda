from os import path
from django.shortcuts import render

# Create your views here.



def index(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']

    file = open('src/log/1.txt', 'w')
    file.write('BaldaLogGame 1 start')
    file.close()
    return render(request, 'field.html', {'field': field})


def run_game(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']
    print('BaldaGameLog', 1, )
    return render(request, 'field.html', {'field': field})


def start_game(request, game_id):
    file_path = path.join('src/log', '.'.join([str(game_id), 'txt']))
    file = open(file_path, 'w')
    file.writelines(['BaldaLogGame ', str(game_id), ' start'])
    file.close()
    file_path_read = path.join('src/results', str(game_id), '0.txt')
    file_read_from = None
    while True:
        try:
            file_read_from = open(file_path_read, 'r')
            break
        except FileNotFoundError as e:
            continue

    command = file_read_from.readline()
    data = eval(file_read_from.readline())
    print(data)
    first_word = data.get('first_word')
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = [first_word[i] for i in range(5)]

    return render(request, 'field.html', {'field': field})