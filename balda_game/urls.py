from django.conf.urls import patterns, url
from balda_game import views

__author__ = 'akhtyamovpavel'

urlpatterns = patterns('',

    url(r'^$', views.index, name='index'),

    url(r'^run_game/$', views.run_game, name='game')

)