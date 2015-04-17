from django.conf.urls import patterns, url
from balda_game import views

__author__ = 'akhtyamovpavel'

urlpatterns = patterns('',

    url(r'^$', views.index, name='index'),

    url(r'^run_game/$', views.run_game, name='game'),

    url(r'^game/(?P<game_id>\d+)', views.start_game, name='start_game')
)