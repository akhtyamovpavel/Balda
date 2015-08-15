import datetime
import json

from django.conf import settings
from django.contrib.auth import logout, authenticate, login
from django.contrib.auth.decorators import login_required
from django.contrib.auth.forms import UserCreationForm
from django.contrib.auth.models import User
from django.core.cache import cache
from django.core.exceptions import ObjectDoesNotExist
from django.http import HttpResponseRedirect, HttpResponse, Http404
from django.shortcuts import render, redirect






# Create your views here.
from balda_game.lib.bot.Level import Level, is_bot
from balda_game.lib.field.CellState import SPARE, FIXED
from balda_game.lib.GameProcessor import GameProcessor
from balda_game.lib.Packer import pack_game_message_with_action, deserialize_int, deserialize_list
from balda_game.lib.dictionary.SingletonDictionary import dictionary
from balda_game.lang.RussianLanguage import RussianLanguage
from balda_game.models import UserPlayer, GameModel


def index(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']
    first_word = dictionary.get_first_word(5)
    # print(first_word)
    return render(request, 'index.html')


def run_game(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']
    return render(request, 'field.html', {'field': field})


def start_game(request, game_id):
    game_model = None
    try:
        game_model = GameModel.objects.get(pk=int(game_id))
    except ObjectDoesNotExist as e:
        return render(request, 'not_started_game.html', {})

    if game_model.status == 'end':
        return render(request, 'ended_game.html', {'game_log' : game_model})

    field = [[['.', SPARE] for i in range(5)] for j in range(5)]
    # TODO check for errors
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
            user1.rating = 1500
            user1.user = new_user
            user1.was_online = datetime.datetime.now()
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
                return render(request, 'login.html', {"message": "Wrong username or password"})
        else:
            return render(request, 'login.html', {"message": "Wrong username or password"})
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
    now = datetime.datetime.now()
    cache.set('wait_%s' % (request.user.username), now, settings.USER_LAST_SEEN_TIMEOUT)
    return render(request, 'game_wait.html', {})


@login_required
def wait_query(request):
    value = GameProcessor.add_waiting_player(request.user)
    json_result = {'game': value}
    if value > 0:
        now = datetime.datetime.now()
        cache.set('seen_%d_%s' % (value, request.user.username), now, settings.USER_LAST_SEEN_TIMEOUT)

    return HttpResponse(json.dumps(json_result), content_type="application/json")


@login_required
def get_field(request, game_id):
    game_id = deserialize_int(game_id)
    now = datetime.datetime.now()
    cache.set('seen_%d_%s' % (game_id, request.user.username), now, settings.USER_LAST_SEEN_TIMEOUT)
    json_result = pack_game_message_with_action(game_id, request.user)
    return HttpResponse(json.dumps(json_result), content_type="application/json")


@login_required
def commit_word(request, game_id):
    game_id = deserialize_int(game_id)
    now = datetime.datetime.now()
    cache.set('seen_%d_%s' % (game_id, request.user.username), now, settings.USER_LAST_SEEN_TIMEOUT)

    pinned_height = deserialize_int(request.POST.get('pinned_height', False))
    pinned_width = deserialize_int(request.POST.get('pinned_width', False))
    word = request.POST.get('word', False)
    heights = deserialize_list(request.POST.getlist('heights[]', []))
    widths = deserialize_list(request.POST.getlist('widths[]', []))
    pinned_letter = request.POST.get('pinned_letter', False)
    flag = True
    # if not GameProcessor.check_board_consistency(game_id, pinned_height, pinned_width, word, heights, widths):
    #     return HttpResponse(pack_game_message_with_action(game_id, request.user, 'reset'),
    #                         content_type="application/json")
    # if not dictionary.check_word(game_id, heights, widths, Coordinates(pinned_height, pinned_width), word):
    #     return HttpResponse(pack_game_message_with_action(game_id, request.user, 'reset'),
    #                         content_type="application/json")
    # if not GameProcessor.change_move(request.user, game_id, word, pinned_height, pinned_width, pinned_letter):
    #     return HttpResponse(pack_game_message_with_action(game_id, request.user, 'reset'),
    #                         content_type="application/json")
    #

    if not GameProcessor.commit_word(game_id, pinned_height, pinned_width,
                                     pinned_letter, word, heights, widths, request.user):
        return HttpResponse(pack_game_message_with_action(game_id, request.user, 'reset'),
                            content_type="application/json")
    else:
        return HttpResponse(pack_game_message_with_action(game_id, request.user, 'ok', GameProcessor.is_bot_game(game_id)), content_type="application/json")


def give_up(request, game_id):
    game_id = deserialize_int(game_id)
    now = datetime.datetime.now()
    cache.set('seen_%d_%s' % (game_id, request.user.username), now, settings.USER_LAST_SEEN_TIMEOUT)
    GameProcessor.give_up(game_id, request.user)
    json_result = pack_game_message_with_action(game_id, request.user)
    return HttpResponse(json.dumps(json_result), content_type="application/json")


def view_profile(request, username):
    user = User.objects.get(username=username)
    if user is None:
        raise Http404
    else:
        user_profile = UserPlayer.objects.get(user=user)
        if user_profile is None:
            raise Http404
        else:
            return render(request, 'profile.html', {'user_profile': user_profile})


def load_best(request):
    users = UserPlayer.objects.order_by('-rating')[:10]
    result = []
    cnt = 0
    for i in range(len(users)):
        if not is_bot(users[i].user) and users[i].wins + users[i].draws + users[i].loses > 0:
            result.append({"place": cnt + 1, "user": users[i].user.username, "rating": users[i].rating})
            cnt += 1
    objects = {"field": result}

    return HttpResponse(json.dumps(objects), content_type="application/json")


def cancel_game_request(request):
    result = GameProcessor.cancel_game_request(request.user)
    return HttpResponse(json.dumps({'isGameCancelled': result}), content_type="application/json")


def play_with_bot(request):
    game_id = GameProcessor.add_bot(request.user, Level.EASY)

    json_result = {'game': game_id}
    return HttpResponse(json.dumps(json_result), content_type="application/json")