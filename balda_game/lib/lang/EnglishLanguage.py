from balda_game.lib.lang.Language import Language

__author__ = 'akhtyamovpavel'

class EnglishLanguage(Language):

    def __init__(self):
        super(EnglishLanguage, self).__init__()
        self.__english_letters__ = [chr(number) for number in range(ord('A'), ord('Z') + 1)]


    def get_list(self):
        return self.__english_letters__


if __name__ == '__main__':
    print(EnglishLanguage().get_list())