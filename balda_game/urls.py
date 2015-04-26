from django.conf.urls import patterns, url
from balda_game import views

__author__ = 'akhtyamovpavel'

urlpatterns = patterns('',

    url(r'^$', views.index, name='index'),

    url(r'^run_game/$', views.run_game, name='game'),

    url(r'^game/(?P<game_id>\d+)', views.start_game, name='start_game'),

    url(r'^login/$', views.login_view, name='login_view'),
    url(r'^logout/$', views.logout_view, name='logout_view'),
    url(r'^register/$', views.register, name='register'),
)