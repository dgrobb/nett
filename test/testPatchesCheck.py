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
    
    def test_tab_expansion(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/patches/tabExpansionTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 3: [LINE-LENGTH] Line length of 80 is over the maximum of 79.",
            "Line 4: [INDENTATION] 'int' Expected indent of 4 spaces, found 8.",
            "Line 7: [INDENTATION] 'i' Expected indent of 12 spaces, found 16.",
            "Line 8: [INDENTATION] 'i' Expected indent of 12 spaces, found 16.",
            "Line 10: [INDENTATION] 'x' Expected indent of 8 spaces, found 9.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)