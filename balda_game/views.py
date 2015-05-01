import json
from os import path
from django.contrib.auth import logout, authenticate, login
from django.contrib.auth.decorators import login_required
from django.contrib.auth.forms import UserCreationForm
from django.http import HttpResponseRedirect, HttpResponse
from django.shortcuts import render, redirect

# Create your views here.
from balda_game.CellState import SPARE, FIXED
from balda_game.GameManagerProcessor import GameProcessor
from balda_game.SingletonDictionary import dictionary
from balda_game.lang.RussianLanguage import RussianLanguage
from balda_game.models import UserPlayer


def index(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']
    first_word = dictionary.get_first_word(5)
    print(first_word)
    return render(request, 'index.html')


def run_game(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']
    return render(request, 'field.html', {'field': field})


def start_game(request, game_id):
    field = [[['.', SPARE] for i in range(5)] for j in range(5)]
    #TODO check for errors
    word = GameProcessor.list_first_words.get(int(game_id))
    GameProcessor.start_game(int(game_id))
    field[2] = [[letter, FIXED] for letter in word]
    lang_list = RussianLanguage().get_list()
    return render(request, 'field.html', {'field': field, 'game_id': game_id, 'lang_list': lang_list})

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


@login_required
def game_wait(request):
    GameProcessor.add_player(request.user)
    return render(request, 'game_wait.html', {})

@login_required
def wait_query(request):
    value = GameProcessor.add_waiting_player(request.user)
    json_result = {'game': value}
    return HttpResponse(json.dumps(json_result), content_type="application/json")

@login_required
def get_field(request, game_id):
    game_id = int(game_id)
    field_pack = GameProcessor.get_field(game_id)
    current_player = GameProcessor.get_current_player(game_id)

    first_player, second_player = GameProcessor.get_players(game_id)

    user_player = 1
    if request.user == first_player:
        user_player = 0

    is_your_move = user_player == current_player
    json_result = {"field": field_pack, "current_player": current_player, "is_your_move": is_your_move}
    return HttpResponse(json.dumps(json_result), content_type="application/json")

@login_required
def commit_word(request, game_id):
    pass