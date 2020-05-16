""" Copyright (C) 2020  Matthys Grobbelaar 
    Full license notice can be found in Nett.cpp
"""
import unittest
import subprocess
from runner import NETT_PATH
from runner import TEST_PATH

class TestBracesChecks(unittest.TestCase):
    
    def setUp(self):
        self.maxDiff = None

    def test_missing_braces_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/braces/missingBracesCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 4: [BRACES] Braces are required, even for single line blocks.",
            "Line 6: [BRACES] Braces are required, even for single line blocks.",
            "Line 8: [BRACES] Braces are required, even for single line blocks.",
            "Line 11: [BRACES] Braces are required, even for single line blocks.",
            "Line 14: [BRACES] Braces are required, even for single line blocks.",
            "Line 17: [BRACES] Braces are required, even for single line blocks.",
            "Line 21: [BRACES] Braces are required, even for single line blocks.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_incorrect_brace_positioning_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/braces/incorrectBracePositioningCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([    
            "Line 2: [BRACES] Opening braces should look like: enum X {",
            "Line 7: [BRACES] Opening braces should look like: struct X {",
            "Line 13: [BRACES] Opening braces should look like: union X {",
            "Line 21: [BRACES] Opening braces should look like: if (cond) {",
            "Line 24: [BRACES] Opening braces should look like: if (cond) {",
            "Line 27: [BRACES] Opening braces should look like: else {",
            "Line 33: [BRACES] Closing braces should look like: } else",
            "Line 39: [BRACES] Opening braces should look like: for (...;...;...) {",
            "Line 44: [BRACES] Opening braces should look like: while (cond) {",
            "Line 49: [BRACES] Opening braces should look like: do {",
            "Line 51: [BRACES] Closing braces should look like: } while (cond)",
            "Line 55: [BRACES] Opening braces should look like: switch (cond) {",
            "Line 58: [BRACES] Opening braces should look like: case X: {",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_brace_not_end_of_line_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/braces/braceNotEndOfLineCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([   
            "Line 1: [INDENTATION] 'int' Expected indent of 4 spaces, found 14.",
            "Line 1: [BRACES] Opening brace is not the last character on a line.",
            "Line 4: [BRACES] Opening brace is not the last character on a line.",
            "Line 4: [INDENTATION] 'int' Expected indent of 4 spaces, found 12.",
            "Line 7: [BRACES] Opening brace is not the last character on a line.",
            "Line 11: [BRACES] Opening brace is not the last character on a line.",
            "Line 11: [INDENTATION] 'argc' Expected indent of 4 spaces, found 34.",
            "Line 13: [BRACES] Opening brace is not the last character on a line.",
            "Line 13: [INDENTATION] 'return' Expected indent of 8 spaces, found 15.",
            "Line 14: [BRACES] Opening brace is not the last character on a line.",
            "Line 14: [INDENTATION] 'return' Expected indent of 8 spaces, found 12.",
            "Line 17: [BRACES] Opening brace is not the last character on a line.",
            "Line 17: [INDENTATION] 'argc' Expected indent of 8 spaces, found 36.",
            "Line 20: [BRACES] Opening brace is not the last character on a line.",
            "Line 23: [BRACES] Opening brace is not the last character on a line.",
            "Line 23: [INDENTATION] 'argc' Expected indent of 8 spaces, found 22.",
            "Line 26: [BRACES] Opening brace is not the last character on a line.",
            "Line 26: [INDENTATION] 'case' Expected indent of 8 spaces, found 20.",
            "Line 27: [BRACES] Opening brace is not the last character on a line.",
            "Line 27: [INDENTATION] 'return' Expected indent of 12 spaces, found 17.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_brace_not_end_of_line_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/braces/functionBracePositioningCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([   
            "Line 10: [BRACES] The opening brace of a function should be at most one line away from its parameters.",
            "Line 17: [BRACES] The opening brace of a function should be left-aligned if not on the same line as its parameters.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
