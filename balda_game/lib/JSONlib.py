import json
from balda_game.lib.db.Move import Move
from balda_game.lib.field.Letter import CellLetter

__author__ = 'akhtyamovpavel'


def serialize_cell_letter_to_json(cell_letter: CellLetter):
    return json.dumps({'x': cell_letter.x, 'y': cell_letter.y, 'letter': cell_letter.letter})


def deserialize_cell_letter_from_json(json_string):
    dict_cell_letter = json.loads(json_string)
    return CellLetter(dict_cell_letter['x'], dict_cell_letter['y'], dict_cell_letter['letter'])


def serialize_word_to_json(word):
    return json.dumps([serialize_cell_letter_to_json(cell_letter) for cell_letter in word])


def serialize_move_to_json(move: Move):
    json_word = {
        "added_letter": serialize_cell_letter_to_json(move.get_added_letter()),
        "word": serialize_word_to_json(move.get_word_structure())
    }
    return json.dumps(json_word)


def deserialize_word_from_json(json_string):
    list_encoded_letters = json.loads(json_string)
    return [deserialize_cell_letter_from_json(encoded_letter) for encoded_letter in list_encoded_letters]


def deserialize_move_from_json(json_string):
    result_move = Move()
    decoded_json = json.loads(json_string)
    result_move.set_added_letter(deserialize_cell_letter_from_json(decoded_json.get('added_letter')))
    result_move.set_word_structure(deserialize_word_from_json(decoded_json.get('word')))
    return result_move


def serialize_game_log_to_json(list_moves):
    return json.dumps([serialize_move_to_json(move) for move in list_moves])


def deserialize_game_log_from_json(json_string):
    list_encoded_moves = json.loads(json_string)
    return [deserialize_move_from_json(encoded_move) for encoded_move in list_encoded_moves]