""" Copyright (C) 2020  Matthys Grobbelaar 
    Full license notice can be found in Nett.cpp
"""
import unittest
import subprocess
from runner import NETT_PATH
from runner import TEST_PATH

class TestWhitespaceChecks(unittest.TestCase):
    
    def setUp(self):
        self.maxDiff = None

    def test_stdbool_error_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/patches/stdboolErrorCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 3: [NAMING] 'foo' misnamed. Types should be NamedLikeThis.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_variadic_argument_error_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/patches/varargsIndentationCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 7: [INDENTATION] 'va_list' Expected indent of 4 spaces, found 8.",
            "Line 12: [WHITESPACE] 'va_arg' at position 22, expected 1 space, found 2",
            "Line 26: [INDENTATION] 'va_end' Expected indent of 4 spaces, found 1.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)