from django import forms
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm

from balda_game.models import UserPlayer
import datetime


class CreationUserForm(UserCreationForm):
    first_name = forms.CharField(required=False)
    last_name = forms.CharField(required=False)

    def __init__(self, request=None, *args, **kwargs):
        super(CreationUserForm, self).__init__(request, args, kwargs)

        for field in ['username', 'first_name', 'last_name', 'password1', 'password2']:
            self.fields[field].widget.attrs.update({'class': 'form-control'})

    class Meta:
        model = User
        fields = (
            'username', 'first_name', 'last_name', 'password1', 'password2')

    def save(self, commit=True):
        print('Saving data')
        user = super(CreationUserForm, self).save(commit=False)
        # print(self.cleanedData['first_name'])
        user.first_name = self.cleaned_data.get('first_name')
        user.last_name = self.cleaned_data.get('last_name')
        user.save()
        if commit:
            user.save()
            user1 = UserPlayer()
            user1.wins = 0
            user1.loses = 0
            user1.draws = 0
            user1.rating = 1500
            user1.user = user
            user1.was_online = datetime.datetime.now()
            user1.save()

        return user
