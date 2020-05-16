""" Copyright (C) 2020  Matthys Grobbelaar 
    Full license notice can be found in Nett.cpp
"""
import unittest
import subprocess
from runner import NETT_PATH
from runner import TEST_PATH

class TestCommentsChecks(unittest.TestCase):
    
    def setUp(self):
        self.maxDiff = None

    def test_global_var_comments_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/comments/globalVarCommentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 25: [COMMENTS] Global variables should be commented.",
            "Line 27: [COMMENTS] Global variables should be commented.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_function_comments_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/comments/functionCommentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 7: [COMMENTS] Functions should be preceded by explanatory comments.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    