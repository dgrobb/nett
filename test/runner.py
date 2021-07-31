#!/usr/bin/env python3
import unittest
import os 

TEST_PATH = os.path.dirname(os.path.realpath(__file__))
NETT_PATH = TEST_PATH.split("llvm-project/", 1)[0] + "llvm-project/build/bin/nett"

# Import the test modules
import testNamingCheck
import testCommentsCheck
import testBracesCheck
import testWhitespaceCheck
import testIndentationCheck
import testLineLengthCheck
import testOverallCheck
import testWarningCheck
import testPatchesCheck

# Initialize the test suite
loader = unittest.TestLoader()
suite  = unittest.TestSuite()

# Add tests to the test suite
suite.addTests(loader.loadTestsFromModule(testNamingCheck))
suite.addTests(loader.loadTestsFromModule(testCommentsCheck))
suite.addTests(loader.loadTestsFromModule(testBracesCheck))
suite.addTests(loader.loadTestsFromModule(testWhitespaceCheck))
suite.addTests(loader.loadTestsFromModule(testIndentationCheck))
suite.addTests(loader.loadTestsFromModule(testLineLengthCheck))
suite.addTests(loader.loadTestsFromModule(testOverallCheck))
suite.addTests(loader.loadTestsFromModule(testWarningCheck))
suite.addTests(loader.loadTestsFromModule(testPatchesCheck))

# Initialize a runner, pass it the test suite and run it
runner = unittest.TextTestRunner(verbosity=3)

if __name__=="__main__":
    result = runner.run(suite)