from os import path
from django.contrib.auth import logout, authenticate, login
from django.contrib.auth.decorators import login_required
from django.contrib.auth.forms import UserCreationForm
from django.http import HttpResponseRedirect
from django.shortcuts import render, redirect

# Create your views here.
from balda_game.SingletonDictionary import dictionary
from balda_game.models import UserPlayer


def index(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']
    first_word = dictionary.get_first_word(5)
    print(first_word)
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

def register(request):
    if request.method == 'POST':
        form = UserCreationForm(request.POST)
        if form.is_valid():
            new_user = form.save()
            user1 = UserPlayer()
            user1.wins = 0
            user1.loses = 0
            user1.draws = 0
            user1.rating = 0
            user1.user = new_user
            user1.save()
            user = UserPlayer.objects.create
            return HttpResponseRedirect('/')
    form = UserCreationForm()
    return render(request, "register.html", {
        'form': form
    })


def login_view(request):
    if request.method == 'POST':
        username = request.POST.get('username', False)
        password = request.POST.get('password', False)

        user = authenticate(username=username, password=password)
        if user is not None:
            if user.is_active:
                login(request, user)
                return HttpResponseRedirect('/')
            else:
                return render(request, 'login.html', {"message": "Wrong login or password"})
    else:
        return render(request, 'login.html', {})

@login_required
def logout_view(request):
    logout(request)
    return redirect('/')

@login_required
def profile(request):
    user_profile = UserPlayer.objects.get(user=request.user)
    return render(request, 'profile.html', {'user_profile': user_profile})
