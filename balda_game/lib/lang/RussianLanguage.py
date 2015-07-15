from balda_game.lib.lang.Language import Language

__author__ = 'akhtyamovpavel'

class RussianLanguage(Language):

    def __init__(self):
        super(RussianLanguage, self).__init__()
        self.__russian_letters__ = [chr(number) for number in range(ord('А'), ord('Е') + 1)]
        self.__russian_letters__.append('Ё')
        self.__russian_letters__.extend([chr(number) for number in range(ord('Ж'), ord('Я') + 1)])

    def get_list(self):
        return self.__russian_letters__


if __name__ == '__main__':
    print(RussianLanguage().get_list())