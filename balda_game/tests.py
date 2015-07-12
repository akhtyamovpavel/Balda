import random
import string
from django.test import TestCase
from balda_game.lib.Packer import deserialize_cell_letter_from_json, \
    deserialize_word_from_json, serialize_word_to_json, deserialize_move_from_json, serialize_move_to_json
from balda_game.lib.JSONlib import serialize_cell_letter_to_json, deserialize_cell_letter_from_json, \
    serialize_word_to_json, serialize_move_to_json, deserialize_word_from_json, deserialize_move_from_json
from balda_game.lib.db.Move import Move
from balda_game.lib.field.Letter import CellLetter


def generate_cell_letter():
    return CellLetter(random.randint(0,4), random.randint(0,4), random.choice(string.ascii_letters))

def generate_word():
    length = random.randint(5, 15)
    word_sample = [generate_cell_letter() for i in range(length)]
    return word_sample


class JSONCellLetterTest(TestCase):

    json_string_sample = '{"x": 0, "y": 0, "letter": "B"}'

    def test_serialize(self):
        self.assertJSONEqual(serialize_cell_letter_to_json(CellLetter(0, 0, 'B')), self.json_string_sample)

    def test_deserialize(self):
        deserialized_cell_letter = deserialize_cell_letter_from_json(self.json_string_sample)
        self.assertEquals(deserialized_cell_letter, CellLetter(0, 0, 'B'))

    def test_serialize_deserialize(self):
        self.assertNotEqual(CellLetter(0, 0, 'C'), deserialize_cell_letter_from_json(
                                                                serialize_cell_letter_to_json(CellLetter(0, 0 ,'B'))))

    def test_one_hundred_random(self):
        for i in range(100):
            cell_letter = generate_cell_letter()
            self.assertEqual(cell_letter, deserialize_cell_letter_from_json(serialize_cell_letter_to_json(cell_letter)))


class JSONWordTest(TestCase):

    def test_serialize_deserialize(self):
        word_sample = [CellLetter(0, 0, 'B'), CellLetter(0, 1, 'A')]
        self.assertListEqual(word_sample, deserialize_word_from_json(serialize_word_to_json(word_sample)))

    def test_one_hundred_random(self):
        for i in range(100):
            word_sample = generate_word()
            self.assertListEqual(word_sample, deserialize_word_from_json(serialize_word_to_json(word_sample)))

class JSONMoveTest(TestCase):

    def test_serialize_deserialize(self):
        move = Move()
        move.set_added_letter(CellLetter(0, 0, 'B'))
        move.set_word_structure([CellLetter(0, 0, 'B'), CellLetter(0, 1, 'A')])
        result = deserialize_move_from_json(serialize_move_to_json(move))
        self.assertTrue(isinstance(result, Move))
        self.assertEqual(result.get_added_letter(), CellLetter(0, 0, 'B'))
        self.assertEqual(result.get_word_structure(), [CellLetter(0, 0, 'B'), CellLetter(0, 1, 'A')])

    def test_one_hundred_random(self):
        for i in range(100):
            word_sample = generate_word()
            added_letter = word_sample[0]
            move_sample = Move()
            move_sample.set_added_letter(added_letter)
            move_sample.set_word_structure(word_sample)
            result = deserialize_move_from_json(serialize_move_to_json(move_sample))
            self.assertListEqual(result.get_word_structure(), word_sample)
            self.assertEqual(result.get_added_letter(), added_letter)
