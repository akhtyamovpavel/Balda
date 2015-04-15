from PySide import QtGui
from django.shortcuts import render

# Create your views here.
import sys
from balda_game.src.MainWindow import MainWindow
from balda_game.src.interface.GraphicBoard import GraphicBoard


def index(request):
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']
    return render(request, 'field.html', {'field': field})


def run_game(request):
    app = QtGui.QApplication(sys.argv)
    w = GraphicBoard(5, 5, game_lang='Russian', players=2, parent=None)
    field = [['-' for i in range(5)] for j in range(5)]
    field[2] = ['Б', 'А', 'Л', 'Д', 'А']
    return render(request, 'field.html', {'field': field})

