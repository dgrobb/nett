""" Copyright (C) 2020  Matthys Grobbelaar 
    Full license notice can be found in Nett.cpp
"""
import unittest
import subprocess
from runner import NETT_PATH
from runner import TEST_PATH

class TestOverallChecks(unittest.TestCase):
    
    def setUp(self):
        self.maxDiff = None

    def test_function_definition_in_header_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/overall/functionDefinitionInHeaderCheckTest.h"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 5: [OVERALL] Headers should not contain function definitions.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_keyword_redefinition_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/overall/keywordRedefCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 3: [NAMING] 'if' misnamed. #defines should be NAMED_LIKE_THIS.",
            "Line 3: [OVERALL] Do not #define a keyword to something else.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_function_length_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/overall/functionLengthCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 1: [OVERALL] Function length of 51 is over the maximum of 50.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    