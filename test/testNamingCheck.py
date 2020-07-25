""" Copyright (C) 2020  Matthys Grobbelaar 
    Full license notice can be found in Nett.cpp
"""
import unittest
import subprocess
from runner import NETT_PATH
from runner import TEST_PATH

class TestNamingChecks(unittest.TestCase):

    def setUp(self):
        self.maxDiff = None

    def test_var_name_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/varNameCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 2: [NAMING] 'UPPER_SNAKE_CASE' misnamed. Variables should be namedLikeThis.",
            "Line 5: [NAMING] 'lower_snake_case' misnamed. Variables should be namedLikeThis.",
            "Line 9: [NAMING] 'IncorrectCamelCase' misnamed. Variables should be namedLikeThis.",
            "Line 10: [NAMING] 'someChr' misnamed. Hungarian notation is not to be used.",
            "Line 11: [NAMING] 'ptrForSomething' misnamed. Hungarian notation is not to be used.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_func_name_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/funcNameCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 10: [NAMING] 'incorrect_Snake_case_NAME' misnamed. Functions should be named_like_this.",
            "Line 15: [NAMING] 'incorrectCamelCaseLower' misnamed. Functions should be named_like_this.",
            "Line 20: [NAMING] 'IncorrectCamelCaseUpper' misnamed. Functions should be named_like_this.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_file_name_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/UpperCamelCaseFileNameCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 1: [NAMING] 'UpperCamelCaseFileNameCheckTest.c' misnamed. Files should be namedLikeThis.c",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/Snake_CASE_fileNameCheck_test.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 1: [NAMING] 'Snake_CASE_fileNameCheck_test.c' misnamed. Files should be namedLikeThis.c",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_struct_union_name_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/structUnionNameCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 10: [NAMING] 'badCamelCase' misnamed. Types should be NamedLikeThis.",
            "Line 15: [NAMING] 'bad_snake_case' misnamed. Types should be NamedLikeThis.",
            "Line 20: [NAMING] 'OTHER_SNAKE_CASE' misnamed. Types should be NamedLikeThis.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_struct_union_field_name_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/structUnionFieldNameCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 4: [NAMING] 'IncorrectCamelCase' misnamed. Struct/Union fields should be namedLikeThis.",
            "Line 5: [NAMING] 'someChar' misnamed. Hungarian notation is not to be used.",
            "Line 10: [NAMING] 'incorrect_lower_snake_case' misnamed. Struct/Union fields should be namedLikeThis.",
            "Line 11: [NAMING] 'INCORRECT_UPPER_SNAKE_CASE' misnamed. Struct/Union fields should be namedLikeThis.",
            "Line 12: [NAMING] 'somePtr' misnamed. Hungarian notation is not to be used.",
            "Line 13: [NAMING] 'anotherPointer' misnamed. Hungarian notation is not to be used.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_enum_name_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/enumNameCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 9: [NAMING] 'incorrectCamelCase' misnamed. Enums should be NamedLikeThis.",
            "Line 13: [NAMING] 'Some_snake_case' misnamed. Enums should be NamedLikeThis.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_enum_const_name_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/enumConstNameCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 4: [NAMING] 'bad_snake_case' misnamed. Enum constants should be NAMED_LIKE_THIS.",
            "Line 5: [NAMING] 'badLowerCamelCase' misnamed. Enum constants should be NAMED_LIKE_THIS.",
            "Line 6: [NAMING] 'BadUpperCamelCase' misnamed. Enum constants should be NAMED_LIKE_THIS.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_define_name_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/defineNameCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 1: [NAMING] 'incorrect_snake_case' misnamed. #defines should be NAMED_LIKE_THIS.",
            "Line 3: [NAMING] 'IncorrectUpperCamelCase' misnamed. #defines should be NAMED_LIKE_THIS.",
            "Line 4: [NAMING] 'incorrectLowerCamelCase' misnamed. #defines should be NAMED_LIKE_THIS.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_typedef_name_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/naming/typedefNameCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 22: [NAMING] 'badBlah' misnamed. Types should be NamedLikeThis.",
            "Line 24: [NAMING] 'bad_int' misnamed. Types should be NamedLikeThis.",
            "Line 28: [NAMING] 'Bad_Foo' misnamed. Types should be NamedLikeThis.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)