from django.contrib import admin

# Register your models here.

import balda_game.models

admin.site.register(balda_game.models.UserPlayer)
admin.site.register(balda_game.models.GameModel)
