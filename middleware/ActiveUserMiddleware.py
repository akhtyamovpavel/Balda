from datetime import datetime
from django.conf import settings
from django.core.cache import cache
__author__ = 'akhtyamovpavel'

class ActiveUserMiddleware:

    def process_request(self, request):
        current_user = request.user
        if request.user.is_authenticated():
            now = datetime.now()
            cache.set('seen_%s' % (current_user.username), now, settings.USER_LAST_SEEN_TIMEOUT)