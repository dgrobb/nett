""" Copyright (C) 2020  Matthys Grobbelaar 
    Full license notice can be found in Nett.cpp
"""
import unittest
import subprocess
from runner import NETT_PATH
from runner import TEST_PATH

class TestLineLengthChecks(unittest.TestCase):
    
    def setUp(self):
        self.maxDiff = None

    def test_line_length_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/line-length/lineLengthCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 12: [LINE_LENGTH] Line length of 116 is over the maximum of 80.",
            "Line 13: [LINE_LENGTH] Line length of 81 is over the maximum of 80.",
            "Line 17: [LINE_LENGTH] Line length of 104 is over the maximum of 80.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    