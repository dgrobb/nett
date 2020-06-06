""" Copyright (C) 2020  Matthys Grobbelaar 
    Full license notice can be found in Nett.cpp
"""
import unittest
import subprocess
from runner import NETT_PATH
from runner import TEST_PATH

class TestWarningChecks(unittest.TestCase):
    
    def setUp(self):
        self.maxDiff = None

    def test_digraph_trigraph_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/warning/digraphTrigraphCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 1: [WARNING] Digraphs and Trigraphs should not be used.",
            "Line 2: [WARNING] Digraphs and Trigraphs should not be used.",
            "Line 5: [WARNING] Digraphs and Trigraphs should not be used.",
            "Line 7: [WARNING] Digraphs and Trigraphs should not be used.",
            "Line 15: [WARNING] Digraphs and Trigraphs should not be used.",
            "Line 17: [WARNING] Digraphs and Trigraphs should not be used.",
            "Line 19: [WARNING] Digraphs and Trigraphs should not be used.",
            "Line 21: [WARNING] Digraphs and Trigraphs should not be used.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_goto_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/warning/goToCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 7: [WARNING] 'goto' is banned and should not be used.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    