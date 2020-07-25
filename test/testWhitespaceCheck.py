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

    def test_binary_operator_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/binaryOperatorWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 8: [WHITESPACE] '*' at position 12, expected 1 space, found 2",
            "Line 8: [WHITESPACE] 'y' at position 16, expected 1 space, found 3",
            "Line 9: [WHITESPACE] '/' at position 13, expected 1 space, found 3",
            "Line 9: [WHITESPACE] 'y' at position 17, expected 1 space, found 3",
            "Line 10: [WHITESPACE] '%' at position 14, expected 1 space, found 4",
            "Line 10: [WHITESPACE] 'y' at position 17, expected 1 space, found 2",
            "Line 13: [WHITESPACE] '+' at position 12, expected 1 space, found 2",
            "Line 13: [WHITESPACE] 'y' at position 15, expected 1 space, found 2",
            "Line 14: [WHITESPACE] '-' at position 13, expected 1 space, found 3",
            "Line 14: [WHITESPACE] 'y' at position 16, expected 1 space, found 2",
            "Line 17: [WHITESPACE] '<<' at position 12, expected 1 space, found 2",
            "Line 17: [WHITESPACE] 'y' at position 16, expected 1 space, found 2",
            "Line 18: [WHITESPACE] '>>' at position 12, expected 1 space, found 2",
            "Line 18: [WHITESPACE] 'x' at position 16, expected 1 space, found 2",
            "Line 21: [WHITESPACE] '<' at position 12, expected 1 space, found 2",
            "Line 21: [WHITESPACE] 'x' at position 15, expected 1 space, found 2",
            "Line 22: [WHITESPACE] '>' at position 19, expected 1 space, found 2",
            "Line 22: [WHITESPACE] 'y' at position 22, expected 1 space, found 2",
            "Line 23: [WHITESPACE] '>=' at position 20, expected 1 space, found 3",
            "Line 23: [WHITESPACE] 'x' at position 25, expected 1 space, found 3",
            "Line 23: [WHITESPACE] ',' at position 27, expected 0 spaces, found 1",
            "Line 23: [WHITESPACE] 'x' at position 30, expected 1 space, found 2", 
            "Line 23: [WHITESPACE] '<=' at position 33, expected 1 space, found 2",
            "Line 23: [WHITESPACE] 'y' at position 37, expected 1 space, found 2",
            "Line 28: [WHITESPACE] '==' at position 12, expected 1 space, found 2",
            "Line 28: [WHITESPACE] 'y' at position 16, expected 1 space, found 2",
            "Line 28: [WHITESPACE] '||' at position 19, expected 1 space, found 2",
            "Line 28: [WHITESPACE] 'y' at position 23, expected 1 space, found 2",  
            "Line 28: [WHITESPACE] '!=' at position 26, expected 1 space, found 2",    
            "Line 28: [WHITESPACE] 'x' at position 30, expected 1 space, found 2", 
            "Line 28: [WHITESPACE] '&&' at position 33, expected 1 space, found 2",
            "Line 28: [WHITESPACE] 'argc' at position 37, expected 1 space, found 2",
            "Line 29: [WHITESPACE] '&' at position 19, expected 1 space, found 2",
            "Line 29: [WHITESPACE] 'y' at position 22, expected 1 space, found 2",
            "Line 29: [WHITESPACE] '|' at position 25, expected 1 space, found 2",
            "Line 29: [WHITESPACE] 'z' at position 28, expected 1 space, found 2",
            "Line 29: [WHITESPACE] '^' at position 31, expected 1 space, found 2",
            "Line 29: [WHITESPACE] 'argc' at position 34, expected 1 space, found 2",
            "Line 33: [WHITESPACE] '=' at position 8, expected 1 space, found 2",
            "Line 33: [WHITESPACE] 'x' at position 11, expected 1 space, found 2",
            "Line 34: [WHITESPACE] '*=' at position 8, expected 1 space, found 2",
            "Line 34: [WHITESPACE] 'y' at position 12, expected 1 space, found 2",
            "Line 35: [WHITESPACE] '/=' at position 9, expected 1 space, found 3",
            "Line 35: [WHITESPACE] 'x' at position 13, expected 1 space, found 2",
            "Line 36: [WHITESPACE] '%=' at position 10, expected 1 space, found 4",
            "Line 36: [WHITESPACE] 'y' at position 17, expected 1 space, found 5",
            "Line 37: [WHITESPACE] '+=' at position 9, expected 1 space, found 3",
            "Line 37: [WHITESPACE] 'y' at position 13, expected 1 space, found 2",
            "Line 38: [WHITESPACE] '-=' at position 8, expected 1 space, found 2",
            "Line 38: [WHITESPACE] 'x' at position 13, expected 1 space, found 3",
            "Line 39: [WHITESPACE] '<<=' at position 9, expected 1 space, found 3",
            "Line 39: [WHITESPACE] 'argc' at position 14, expected 1 space, found 2",
            "Line 40: [WHITESPACE] '>>=' at position 9, expected 1 space, found 3",
            "Line 40: [WHITESPACE] 'argc' at position 14, expected 1 space, found 2",
            "Line 41: [WHITESPACE] '&=' at position 10, expected 1 space, found 4",
            "Line 41: [WHITESPACE] 'x' at position 14, expected 1 space, found 2",
            "Line 42: [WHITESPACE] '^=' at position 9, expected 1 space, found 3",
            "Line 42: [WHITESPACE] 'x' at position 13, expected 1 space, found 2",
            "Line 43: [WHITESPACE] '|=' at position 9, expected 1 space, found 3",
            "Line 43: [WHITESPACE] 'argc' at position 14, expected 1 space, found 3",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_unary_operator_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/unaryOperatorWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 8: [WHITESPACE] '++' at position 7, expected 0 spaces, found 1",
            "Line 9: [WHITESPACE] '--' at position 8, expected 0 spaces, found 2",
            "Line 10: [WHITESPACE] 'x' at position 8, expected 0 spaces, found 1",
            "Line 11: [WHITESPACE] 'x' at position 9, expected 0 spaces, found 2",
            "Line 13: [WHITESPACE] Unary operators should look like: ++X",
            "Line 16: [WHITESPACE] Unary operators should look like: X--",
            "Line 19: [WHITESPACE] 'x' at position 11, expected 0 spaces, found 1",
            "Line 20: [WHITESPACE] Unary operators should look like: !X",
            "Line 24: [WHITESPACE] 'x' at position 12, expected 0 spaces, found 2",
            "Line 25: [WHITESPACE] Unary operators should look like: -X",
            "Line 29: [WHITESPACE] 'x' at position 13, expected 0 spaces, found 3",
            "Line 30: [WHITESPACE] Unary operators should look like: &X",
            "Line 33: [WHITESPACE] 'z' at position 11, expected 0 spaces, found 1",
            "Line 34: [WHITESPACE] Unary operators should look like: *X",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_ternary_operator_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/ternaryOperatorWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 2: [WHITESPACE] ';' at position 28, expected 0 spaces, found 1",
            "Line 3: [WHITESPACE] '?' at position 23, expected 1 space, found 2",
            "Line 3: [WHITESPACE] '3' at position 26, expected 1 space, found 2",
            "Line 3: [WHITESPACE] ':' at position 29, expected 1 space, found 2",
            "Line 3: [WHITESPACE] '++' at position 34, expected 1 space, found 4",
            "Line 5: [WHITESPACE] '3' at position 16, expected 1 space, found 2",
            "Line 7: [WHITESPACE] ':' at position 21, expected 1 space, found 2",
            ""
        ])

    def test_struct_union_member_access_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/structUnionMemberAccessWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 9: [WHITESPACE] 'pear' at position 12, expected 0 spaces, found 1",
            "Line 10: [WHITESPACE] '->' at position 10, expected 0 spaces, found 1",
            "Line 12: [WHITESPACE] Member accesses should look like: X->a",
            "Line 20: [WHITESPACE] 'pear' at position 8, expected 0 spaces, found 1",
            "Line 21: [WHITESPACE] '.' at position 7, expected 0 spaces, found 1",
            "Line 22: [WHITESPACE] '.' at position 7, expected 0 spaces, found 1",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_statement_semicolon_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/statementSemicolonWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 3: [WHITESPACE] ';' at position 18, expected 0 spaces, found 3",
            "Line 5: [WHITESPACE] Semicolons should be on the same line as the end of a statement.",
            "Line 8: [WHITESPACE] ';' at position 19, expected 0 spaces, found 2",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_global_variable_semicolon_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/globalVariableSemicolonWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 3: [WHITESPACE] ';' at position 23, expected 0 spaces, found 1",
            "Line 7: [WHITESPACE] Semicolons should be on the same line as the end of a declaration.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    
    def test_if_stmt_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/ifStmtWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 3: [WHITESPACE] '(' at position 9, expected 1 space, found 2",
            "Line 7: [WHITESPACE] '{' at position 16, expected 1 space, found 2",
            "Line 11: [WHITESPACE] 'argc' at position 10, expected 0 spaces, found 1",
            "Line 13: [WHITESPACE] '{' at position 13, expected 1 space, found 2",
            "Line 17: [WHITESPACE] ')' at position 14, expected 0 spaces, found 1",
            "Line 19: [WHITESPACE] else-if statements should look like: } else if (cond...",
            "Line 24: [WHITESPACE] If statements should look like: if (cond...",
            "Line 27: [WHITESPACE] 'else' at position 8, expected 1 space, found 2",
            "Line 27: [WHITESPACE] 'if' at position 14, expected 1 space, found 2",
            "Line 29: [WHITESPACE] Closing parentheses should look like: ...X)",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_for_loop_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/forLoopWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 3: [WHITESPACE] For loops should look like: for (...",
            "Line 9: [WHITESPACE] Closing parentheses should look like: ...X)",
            "Line 12: [WHITESPACE] 'int' at position 12, expected 0 spaces, found 2",
            "Line 12: [WHITESPACE] ')' at position 37, expected 0 spaces, found 3",
            "Line 15: [WHITESPACE] '(' at position 12, expected 1 space, found 4",
            "Line 15: [WHITESPACE] '{' at position 39, expected 1 space, found 3",
            "Line 20: [WHITESPACE] ';' at position 11, expected 0 spaces, found 1",
            "Line 20: [WHITESPACE] ';' at position 13, expected 0 spaces, found 1",
            "Line 20: [WHITESPACE] ')' at position 15, expected 0 spaces, found 1",
            "Line 22: [WHITESPACE] ';' at position 20, expected 0 spaces, found 1",
            "Line 22: [WHITESPACE] ';' at position 29, expected 0 spaces, found 1",
            "Line 27: [WHITESPACE] ';' at position 12, expected 0 spaces, found 2",
            "Line 35: [WHITESPACE] ';' at position 24, expected 0 spaces, found 1",
            "Line 37: [WHITESPACE] Missing for loop conditions should look like: for (...;; ...",
            "Line 43: [WHITESPACE] ')' at position 35, expected 0 spaces, found 2",
            "Line 46: [WHITESPACE] Closing parentheses should look like: ...X)",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_do_loop_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/doLoopWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 4: [WHITESPACE] '{' at position 9, expected 1 space, found 2",
            "Line 10: [WHITESPACE] 'while' at position 9, expected 1 space, found 3",
            "Line 15: [WHITESPACE] '(' at position 15, expected 1 space, found 3",
            "Line 21: [WHITESPACE] Do-While loop conditions should look like: while (...",
            "Line 28: [WHITESPACE] Closing parentheses should look like: ...X)",
            "Line 33: [WHITESPACE] ';' at position 21, expected 0 spaces, found 2",
            "Line 36: [WHITESPACE] Do-While loop conditions should look like: ...);",
            "Line 42: [WHITESPACE] 'argc' at position 15, expected 0 spaces, found 1",
            "Line 42: [WHITESPACE] ')' at position 20, expected 0 spaces, found 1",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_while_loop_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/whileLoopWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 4: [WHITESPACE] While loops should look like: while (...",
            "Line 9: [WHITESPACE] '(' at position 12, expected 1 space, found 2",
            "Line 16: [WHITESPACE] Closing parentheses should look like: ...X)",
            "Line 20: [WHITESPACE] 'argc' at position 13, expected 0 spaces, found 1",
            "Line 20: [WHITESPACE] ')' at position 18, expected 0 spaces, found 1",
            "Line 24: [WHITESPACE] '{' at position 19, expected 1 space, found 2",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_switch_stmt_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/switchStmtWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 6: [WHITESPACE] Switch statements should look like: switch (...",
            "Line 9: [WHITESPACE] '1' at position 15, expected 1 space, found 2",
            "Line 14: [WHITESPACE] '(' at position 13, expected 1 space, found 2",
            "Line 16: [WHITESPACE] ':' at position 21, expected 0 spaces, found 1",
            "Line 21: [WHITESPACE] 'argc' at position 14, expected 0 spaces, found 1",
            "Line 21: [WHITESPACE] ')' at position 19, expected 0 spaces, found 1",
            "Line 23: [WHITESPACE] '{' at position 18, expected 1 space, found 2",
            "Line 31: [WHITESPACE] Closing parentheses should look like: ...X)",
            "Line 33: [WHITESPACE] Case statements should look like: case X:",
            "Line 39: [WHITESPACE] '{' at position 20, expected 1 space, found 2",
            "Line 41: [WHITESPACE] Case expressions should look like: case X:",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_function_decl_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/functionDeclWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 2: [WHITESPACE] ';' at position 27, expected 0 spaces, found 1",
            "Line 5: [WHITESPACE] ')' at position 21, expected 0 spaces, found 1",
            "Line 8: [WHITESPACE] '(' at position 27, expected 0 spaces, found 1",
            "Line 12: [WHITESPACE] Function declarations should look like func_name(...",
            "Line 16: [WHITESPACE] Semicolons should be on the same line as the end of a declaration.",
            "Line 19: [WHITESPACE] 'void' at position 22, expected 0 spaces, found 1",
            "Line 19: [WHITESPACE] ')' at position 27, expected 0 spaces, found 1",
            "Line 24: [WHITESPACE] 'int' at position 21, expected 0 spaces, found 1",
            "Line 24: [WHITESPACE] ')' at position 36, expected 0 spaces, found 1",
            "Line 29: [WHITESPACE] 'int' at position 23, expected 0 spaces, found 1",
            "Line 29: [WHITESPACE] ')' at position 43, expected 0 spaces, found 1",
            "Line 34: [WHITESPACE] ',' at position 23, expected 0 spaces, found 1",
            "Line 34: [WHITESPACE] 'char' at position 33, expected 1 space, found 2",
            "Line 40: [WHITESPACE] ',' at position 15, expected 0 spaces, found 1",
            "Line 40: [WHITESPACE] 'char' at position 25, expected 1 space, found 2",
            "Line 41: [WHITESPACE] Closing parentheses should look like: ...X)",
            "Line 46: [WHITESPACE] 'void' at position 37, expected 0 spaces, found 1",
            "Line 46: [WHITESPACE] ')' at position 42, expected 0 spaces, found 1",
            "Line 51: [WHITESPACE] 'int' at position 42, expected 0 spaces, found 1",
            "Line 51: [WHITESPACE] ',' at position 48, expected 0 spaces, found 1",
            "Line 51: [WHITESPACE] 'char' at position 51, expected 1 space, found 2",
            "Line 51: [WHITESPACE] ')' at position 58, expected 0 spaces, found 1",
            "Line 55: [WHITESPACE] A function's return type should be on the same line as its name.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    
    def test_enum_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/enumWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([
            "Line 2: [WHITESPACE] 'enum' at position 10, expected 1 space, found 2",
            "Line 4: [WHITESPACE] ';' at position 7, expected 0 spaces, found 1",
            "Line 7: [WHITESPACE] Typedef'd enums should look like: typedef enum ...",
            "Line 9: [WHITESPACE] ',' at position 10, expected 0 spaces, found 1",
            "Line 13: [WHITESPACE] 'Bax' at position 7, expected 1 space, found 2",
            "Line 18: [WHITESPACE] '{' at position 11, expected 1 space, found 2",
            "Line 21: [WHITESPACE] Semicolons should be on the same line as the end of a declaration.",
            "Line 26: [WHITESPACE] '{' at position 11, expected 1 space, found 2",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_struct_union_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/structUnionWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([   
            "Line 4: [WHITESPACE] ';' at position 12, expected 0 spaces, found 1",
            "Line 5: [WHITESPACE] ';' at position 7, expected 0 spaces, found 1",
            "Line 11: [WHITESPACE] Semicolons should be on the same line as the end of a declaration.",
            "Line 15: [WHITESPACE] 'Bax' at position 9, expected 1 space, found 2",
            "Line 20: [WHITESPACE] '{' at position 12, expected 1 space, found 2",
            "Line 23: [WHITESPACE] Semicolons should be on the same line as the end of a declaration.",
            "Line 28: [WHITESPACE] '{' at position 13, expected 1 space, found 2",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_function_call_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/funcCallWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([   
            "Line 5: [WHITESPACE] '(' at position 12, expected 0 spaces, found 1",
            "Line 9: [WHITESPACE] Closing parentheses should look like: ...X)",
            "Line 11: [WHITESPACE] Function calls should look like: func_call(...",
            "Line 15: [WHITESPACE] ',' at position 18, expected 0 spaces, found 1",
            "Line 15: [WHITESPACE] 'argc' at position 31, expected 1 space, found 2",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)
    
    def test_var_decl_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/varDeclWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([   
            "Line 5: [WHITESPACE] 'const' at position 13, expected 1 space, found 2",
            "Line 5: [WHITESPACE] 'int' at position 20, expected 1 space, found 2",
            "Line 5: [WHITESPACE] 'a' at position 26, expected 1 space, found 3",
            "Line 5: [WHITESPACE] '=' at position 29, expected 1 space, found 2",
            "Line 5: [WHITESPACE] '2' at position 32, expected 1 space, found 2",
            "Line 6: [WHITESPACE] 'volatile' at position 14, expected 1 space, found 3",
            "Line 6: [WHITESPACE] 'char' at position 25, expected 1 space, found 3",
            "Line 6: [WHITESPACE] 'b' at position 31, expected 1 space, found 2",
            "Line 7: [WHITESPACE] 'DIM' at position 12, expected 0 spaces, found 1",
            "Line 7: [WHITESPACE] ']' at position 16, expected 0 spaces, found 1",
            "Line 8: [WHITESPACE] '[' at position 11, expected 0 spaces, found 1",
            "Line 8: [WHITESPACE] '[' at position 15, expected 0 spaces, found 1",
            "Line 8: [WHITESPACE] Opening brackets should look like: [X...",
            "Line 10: [WHITESPACE] Closing brackets should look like: ...X]",
            "Line 14: [WHITESPACE] Pointers should be a* b or a *b, not a * b.",
            "Line 15: [WHITESPACE] Pointers should not be split across lines.",
            "Line 18: [WHITESPACE] '*' at position 12, expected 0 spaces, found 1",
            "Line 18: [WHITESPACE] ')' at position 15, expected 0 spaces, found 1",
            "Line 18: [WHITESPACE] '(' at position 17, expected 0 spaces, found 1",
            "Line 18: [WHITESPACE] ')' at position 29, expected 0 spaces, found 1",
            "Line 18: [WHITESPACE] Pointers should consistently be a* b or a *b, not a mix.",
            "Line 19: [WHITESPACE] Pointers should consistently be a* b or a *b, not a mix.",
            "Line 20: [WHITESPACE] Pointers should consistently be a* b or a *b, not a mix.",
            "Line 21: [WHITESPACE] ',' at position 13, expected 0 spaces, found 1",
            "Line 21: [WHITESPACE] '*' at position 16, expected 1 space, found 2",
            "Line 21: [WHITESPACE] 'm' at position 18, expected 0 spaces, found 1",
            "Line 21: [WHITESPACE] Pointers should be a* b or a *b, not a*b.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)

    def test_function_separation_whitespace_check(self):
        stdout = subprocess.check_output([NETT_PATH, TEST_PATH + "/test_files/whitespace/functionSeparationWhitespaceCheckTest.c"])
        extracted_output = "\n".join(stdout.decode("utf-8").split('\n')[1:])

        expected_output = "\n".join([   
            "Line 14: [WHITESPACE] Enums and Structs should be separated by reasonable whitespace.",
            "Line 20: [WHITESPACE] Structs and Typedefs should be separated by reasonable whitespace.",
            "Line 23: [WHITESPACE] Typedefs should be separated by reasonable whitespace.",
            "Line 27: [WHITESPACE] Typedefs and Structs should be separated by reasonable whitespace.",
            "Line 31: [COMMENTS] Functions should be preceded by explanatory comments.",
            "Line 35: [WHITESPACE] Functions should be separated by reasonable whitespace.",
            ""
        ])

        self.assertEqual(expected_output, extracted_output)