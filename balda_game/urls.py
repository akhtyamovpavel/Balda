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
    url(r'^profile/$', views.profile, name='profile'),

    #GameProcess Links
    url(r'^game_wait/$', views.game_wait, name='game_wait'),
    url(r'^wait_query/$', views.wait_query, name='wait_query'),
    url(r'^commit_word/(?P<game_id>\d+)/$', views.commit_word, name='commit_word'),
    url(r'^get_field/(?P<game_id>\d+)', views.get_field, name='get_field'),
)