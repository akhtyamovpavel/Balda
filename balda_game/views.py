from django.shortcuts import render

# Create your views here.



def index(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']

    for i in range(10):
        print('BaldaLogGame', i, 'start')
    return render(request, 'field.html', {'field': field})


def run_game(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']
    print('BaldaGameLog', 1, )
    return render(request, 'field.html', {'field': field})

