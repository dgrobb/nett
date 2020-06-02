""" Copyright (C) 2020  Matthys Grobbelaar 
    Full license notice can be found in Nett.cpp
"""
import unittest
import subprocess
from runner import NETT_PATH
from runner import TEST_PATH

class TestIndentationChecks(unittest.TestCase):
    
    def setUp(self):
        self.maxDiff = None
    
    # STANDARD INDENTATION TESTS
    def test_for_loop_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/forLoopIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 15: [INDENTATION] 'for' Expected indent of 4 spaces, found 6.",
            "Line 18: [INDENTATION] 'k' Expected indent of 16 spaces, found 15.",
            "Line 19: [INDENTATION] 'break' Expected indent of 16 spaces, found 18.",
            "Line 21: [INDENTATION] 'continue' Expected indent of 12 spaces, found 15.",
            "Line 22: [INDENTATION] '}' Expected indent of 8 spaces, found 4.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_do_loop_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/doLoopIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 6: [BRACES] Braces are required, even for single line blocks.",
            "Line 14: [BRACES] Braces are required, even for single line blocks.",
            "Line 21: [BRACES] Braces are required, even for single line blocks.",
            "Line 21: [INDENTATION] 'x' Expected indent of 8 spaces, found 10.",
            "Line 22: [INDENTATION] 'while' Expected indent of 4 spaces, found 6.",
            "Line 24: [INDENTATION] 'do' Expected indent of 4 spaces, found 5.",
            "Line 25: [INDENTATION] 'char' Expected indent of 8 spaces, found 9.",
            "Line 26: [INDENTATION] 'y' Expected indent of 8 spaces, found 15.",
            "Line 27: [INDENTATION] 'continue' Expected indent of 8 spaces, found 5.",
            "Line 28: [INDENTATION] '}' Expected indent of 4 spaces, found 3.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_while_loop_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/whileLoopIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 7: [BRACES] Braces are required, even for single line blocks.",
            "Line 16: [BRACES] Braces are required, even for single line blocks.",
            "Line 22: [BRACES] Braces are required, even for single line blocks.",
            "Line 22: [INDENTATION] 'y' Expected indent of 8 spaces, found 6.",
            "Line 24: [INDENTATION] 'while' Expected indent of 4 spaces, found 5.",
            "Line 25: [INDENTATION] 'char' Expected indent of 8 spaces, found 6.",
            "Line 26: [INDENTATION] 'while' Expected indent of 8 spaces, found 10.",
            "Line 28: [INDENTATION] 'break' Expected indent of 12 spaces, found 13.",
            "Line 29: [INDENTATION] '}' Expected indent of 8 spaces, found 9.",
            "Line 31: [BRACES] Braces are required, even for single line blocks.",
            "Line 31: [INDENTATION] 'x' Expected indent of 12 spaces, found 13.",
            "Line 32: [INDENTATION] 'z' Expected indent of 8 spaces, found 9.",
            "Line 33: [INDENTATION] 'continue' Expected indent of 8 spaces, found 6.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_if_stmt_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/ifStmtIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 9: [BRACES] Braces are required, even for single line blocks.",
            "Line 11: [BRACES] Braces are required, even for single line blocks.",
            "Line 19: [INDENTATION] 'if' Expected indent of 4 spaces, found 6.",
            "Line 20: [INDENTATION] 'char' Expected indent of 8 spaces, found 10.",
            "Line 21: [INDENTATION] 'x' Expected indent of 8 spaces, found 5.",
            "Line 22: [INDENTATION] 'if' Expected indent of 8 spaces, found 9.",
            "Line 23: [BRACES] Braces are required, even for single line blocks.",
            "Line 23: [INDENTATION] 'return' Expected indent of 12 spaces, found 13.",
            "Line 24: [INDENTATION] 'else' Expected indent of 8 spaces, found 12.",
            "Line 25: [BRACES] Braces are required, even for single line blocks.",
            "Line 26: [INDENTATION] 'else' Expected indent of 8 spaces, found 5.",
            "Line 27: [INDENTATION] 'x' Expected indent of 12 spaces, found 13.",
            "Line 28: [INDENTATION] '}' Expected indent of 8 spaces, found 9.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_switch_stmt_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/switchStmtIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 34: [BRACES] Braces are required, even for single line blocks.",
            "Line 38: [BRACES] Braces are required, even for single line blocks.",
            "Line 49: [INDENTATION] 'switch' Expected indent of 4 spaces, found 6.",
            "Line 58: [INDENTATION] 'case' Expected indent of 16 spaces, found 18.",
            "Line 63: [INDENTATION] '}' Expected indent of 24 spaces, found 26.",
            "Line 66: [INDENTATION] 'x' Expected indent of 20 spaces, found 28.",
            "Line 69: [INDENTATION] 'return' Expected indent of 20 spaces, found 22.",
            "Line 70: [INDENTATION] '}' Expected indent of 12 spaces, found 14.",
            "Line 71: [INDENTATION] 'if' Expected indent of 12 spaces, found 10.",
            "Line 74: [INDENTATION] 'default' Expected indent of 8 spaces, found 9.",
            "Line 76: [INDENTATION] '}' Expected indent of 4 spaces, found 8.",
            "Line 78: [INDENTATION] 'switch' Expected indent of 4 spaces, found 2.",
            "Line 79: [BRACES] Braces are required, even for single line blocks.",
            "Line 79: [INDENTATION] 'case' Expected indent of 8 spaces, found 9.",
            "Line 81: [INDENTATION] 'x' Expected indent of 12 spaces, found 14.",
            "Line 83: [BRACES] Braces are required, even for single line blocks.",
            "Line 84: [INDENTATION] 'case' Expected indent of 16 spaces, found 17.",
            "Line 85: [INDENTATION] 'x' Expected indent of 20 spaces, found 16.",
            "Line 86: [INDENTATION] 'return' Expected indent of 12 spaces, found 10.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_function_decl_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/functionDeclIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 1: [INDENTATION] 'int' Expected indent of 0 spaces, found 2.",
            "Line 8: [INDENTATION] 'int' Expected indent of 0 spaces, found 6.",
            "Line 10: [INDENTATION] '}' Expected indent of 0 spaces, found 3.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    # CONTINUATION INDENT TESTS
    def test_function_declaration_parameter_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/funcDeclParamContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 29: [INDENTATION] 'char' Expected indent of 8 spaces, found 5.",
            "Line 35: [INDENTATION] 'someParameter' Expected indent of 8 spaces, found 12.",
            "Line 36: [INDENTATION] 'anotherOne' Expected indent of 8 spaces, found 10.",
            "Line 42: [INDENTATION] 'bool' Expected indent of 8 spaces, found 14.",
            "Line 43: [INDENTATION] 'char' Expected indent of 8 spaces, found 13.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_function_call_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/funcCallContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 13: [INDENTATION] 'anotherLongParameterNameForLuck' Expected indent of 12 spaces, found 16.",
            "Line 14: [INDENTATION] '+' Expected indent of 12 spaces, found 13.",
            "Line 21: [INDENTATION] 'argc' Expected indent of 12 spaces, found 8.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_return_stmt_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/returnStmtContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 12: [INDENTATION] 'x' Expected indent of 16 spaces, found 12.",
            "Line 15: [INDENTATION] '/' Expected indent of 16 spaces, found 13.",
            "Line 18: [INDENTATION] '10' Expected indent of 16 spaces, found 13.",
            "Line 19: [INDENTATION] '*' Expected indent of 16 spaces, found 15.",
            "Line 22: [INDENTATION] '+' Expected indent of 12 spaces, found 8.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_bin_op_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/binOpContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 13: [INDENTATION] 'howAboutAnotherOne' Expected indent of 12 spaces, found 8.",
            "Line 14: [INDENTATION] '*' Expected indent of 12 spaces, found 10.",
            "Line 17: [INDENTATION] '*' Expected indent of 12 spaces, found 16.",
            "Line 20: [INDENTATION] 'howAboutAnotherOne' Expected indent of 12 spaces, found 10.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_var_decl_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/varDeclContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 11: [INDENTATION] 'howAboutAnotherOne' Expected indent of 12 spaces, found 8.",
            "Line 12: [INDENTATION] '*' Expected indent of 12 spaces, found 10.",
            "Line 15: [INDENTATION] '-' Expected indent of 12 spaces, found 16.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_global_var_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/globalVarContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 4: [INDENTATION] 'const' Expected indent of 0 spaces, found 2.",
            "Line 7: [INDENTATION] '+' Expected indent of 8 spaces, found 4.",
            "Line 8: [INDENTATION] 'aLongGlobalVariableName' Expected indent of 8 spaces, found 6.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_if_stmt_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/ifStmtContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 12: [BRACES] Braces are required, even for single line blocks.",
            "Line 14: [BRACES] Braces are required, even for single line blocks.",
            "Line 24: [INDENTATION] 'argc' Expected indent of 12 spaces, found 8.",
            "Line 25: [INDENTATION] 'argc' Expected indent of 12 spaces, found 6.",
            "Line 26: [INDENTATION] '<' Expected indent of 12 spaces, found 10.",
            "Line 31: [BRACES] Braces are required, even for single line blocks.",
            "Line 32: [INDENTATION] '2' Expected indent of 20 spaces, found 14.",
            "Line 34: [INDENTATION] 'x' Expected indent of 16 spaces, found 17.",
            "Line 35: [BRACES] Braces are required, even for single line blocks.",
            "Line 40: [BRACES] Braces are required, even for single line blocks.",
            "Line 41: [INDENTATION] '1' Expected indent of 16 spaces, found 9.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_for_loop_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/forLoopContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 7: [INDENTATION] 'j' Expected indent of 16 spaces, found 12.",
            "Line 8: [INDENTATION] 'j' Expected indent of 16 spaces, found 20.",
            "Line 10: [INDENTATION] 'k' Expected indent of 20 spaces, found 14.",
            "Line 11: [INDENTATION] 'k' Expected indent of 20 spaces, found 22.",
            "Line 21: [BRACES] Braces are required, even for single line blocks.",
            "Line 22: [INDENTATION] 'argc' Expected indent of 16 spaces, found 9.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_while_loop_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/whileLoopContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 8: [INDENTATION] '&&' Expected indent of 12 spaces, found 9.",
            "Line 9: [BRACES] Braces are required, even for single line blocks.",
            "Line 15: [INDENTATION] '||' Expected indent of 16 spaces, found 12.",
            "Line 20: [BRACES] Braces are required, even for single line blocks.",
            "Line 21: [INDENTATION] '2' Expected indent of 20 spaces, found 12.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_do_loop_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/doLoopContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 7: [BRACES] Braces are required, even for single line blocks.",
            "Line 14: [INDENTATION] 'argc' Expected indent of 12 spaces, found 10.",
            "Line 21: [BRACES] Braces are required, even for single line blocks.",
            "Line 22: [INDENTATION] '2' Expected indent of 20 spaces, found 16.",
            "Line 24: [INDENTATION] 'x' Expected indent of 16 spaces, found 12.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_switch_stmt_continuation_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/switchStmtContIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 18: [INDENTATION] 'argc' Expected indent of 28 spaces, found 26.",
            "Line 36: [BRACES] Braces are required, even for single line blocks.",
            "Line 40: [INDENTATION] '-' Expected indent of 20 spaces, found 16.",
            "Line 41: [BRACES] Braces are required, even for single line blocks.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_macro_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/macroIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 52: [INDENTATION] 'MACRO' Expected indent of 8 spaces, found 10.",
            "Line 56: [INDENTATION] 'i' Expected indent of 16 spaces, found 14.",
            "Line 59: [INDENTATION] 'MACRO' Expected indent of 8 spaces, found 6.",
            "Line 63: [INDENTATION] '2' Expected indent of 16 spaces, found 13.",
            "Line 64: [INDENTATION] ')' Expected indent of 16 spaces, found 12.",
            "Line 67: [INDENTATION] 'MACRO' Expected indent of 8 spaces, found 11.",
            "Line 72: [INDENTATION] 'argc' Expected indent of 16 spaces, found 21.",
            "Line 76: [INDENTATION] 'MACRO' Expected indent of 8 spaces, found 9.",
            "Line 78: [INDENTATION] 'MACRO' Expected indent of 8 spaces, found 5.",
            "Line 82: [INDENTATION] '2' Expected indent of 16 spaces, found 19.",
            "Line 85: [INDENTATION] '4' Expected indent of 16 spaces, found 21.",
            "Line 87: [INDENTATION] 'MACRO' Expected indent of 4 spaces, found 6.",
            "Line 89: [INDENTATION] '4' Expected indent of 12 spaces, found 8.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_struct_union_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/structUnionIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 40: [INDENTATION] 'struct' Expected indent of 0 spaces, found 2.",
            "Line 42: [INDENTATION] 'struct' Expected indent of 0 spaces, found 1.",
            "Line 44: [INDENTATION] 'int' Expected indent of 4 spaces, found 6.",
            "Line 46: [INDENTATION] 'enum' Expected indent of 4 spaces, found 5.",
            "Line 47: [INDENTATION] 'VAL1' Expected indent of 8 spaces, found 10.",
            "Line 51: [INDENTATION] 'struct' Expected indent of 4 spaces, found 5.",
            "Line 52: [INDENTATION] 'char' Expected indent of 8 spaces, found 9.",
            "Line 54: [INDENTATION] 'y' Expected indent of 16 spaces, found 12.",
            "Line 55: [INDENTATION] '}' Expected indent of 4 spaces, found 0.",
            "Line 56: [INDENTATION] '}' Expected indent of 0 spaces, found 4.",
            "Line 58: [INDENTATION] 'union' Expected indent of 0 spaces, found 4.",
            "Line 66: [INDENTATION] 'enum' Expected indent of 8 spaces, found 7.",
            "Line 67: [INDENTATION] 'A' Expected indent of 12 spaces, found 14.",
            "Line 71: [INDENTATION] '}' Expected indent of 8 spaces, found 9.",
            "Line 76: [INDENTATION] 'char' Expected indent of 8 spaces, found 10.",
            "Line 77: [INDENTATION] '}' Expected indent of 4 spaces, found 5.",
            "Line 78: [INDENTATION] '}' Expected indent of 0 spaces, found 1.",
            "Line 88: [INDENTATION] 'struct' Expected indent of 4 spaces, found 5.",
            "Line 89: [INDENTATION] 'struct' Expected indent of 4 spaces, found 6.",
            "Line 91: [INDENTATION] 'int' Expected indent of 8 spaces, found 9.",
            "Line 92: [INDENTATION] '}' Expected indent of 4 spaces, found 8.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_enum_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/enumIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 17: [INDENTATION] 'enum' Expected indent of 0 spaces, found 3.",
            "Line 19: [INDENTATION] 'enum' Expected indent of 0 spaces, found 1.",
            "Line 20: [INDENTATION] 'BA' Expected indent of 4 spaces, found 5.",
            "Line 21: [INDENTATION] 'BD' Expected indent of 4 spaces, found 6.",
            "Line 22: [INDENTATION] '}' Expected indent of 0 spaces, found 2.",
            "Line 24: [INDENTATION] 'typedef' Expected indent of 0 spaces, found 1.",
            "Line 26: [INDENTATION] 'BBB' Expected indent of 4 spaces, found 5.",
            "Line 40: [INDENTATION] 'enum' Expected indent of 4 spaces, found 5.",
            "Line 41: [INDENTATION] 'NG' Expected indent of 8 spaces, found 10.",
            "Line 42: [INDENTATION] '}' Expected indent of 4 spaces, found 3.",
            "Line 44: [INDENTATION] 'typedef' Expected indent of 4 spaces, found 0.",
            "Line 46: [INDENTATION] 'NL' Expected indent of 8 spaces, found 9.",
            "Line 47: [INDENTATION] '}' Expected indent of 4 spaces, found 5.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_typedef_indent_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/indentation/typedefIndentCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 6: [INDENTATION] 'typedef' Expected indent of 0 spaces, found 1.",
            "Line 7: [WHITESPACE] Line continuations should not contain blank lines.",
            "Line 8: [INDENTATION] 'MyFloat' Expected indent of 8 spaces, found 4.",
            "Line 14: [INDENTATION] 'typedef' Expected indent of 4 spaces, found 6.",
            "Line 15: [INDENTATION] 'char' Expected indent of 12 spaces, found 8.",
            "Line 16: [WHITESPACE] Line continuations should not contain blank lines.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)