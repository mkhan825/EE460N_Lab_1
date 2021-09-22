# -*- coding: utf-8 -*-
"""
Created on Sat Sep  5 20:32:07 2020

This script is written to be used for mass testing of EE 460N PL1
The scirpt will automatically scan for tests, and execute the tests.
It will verify the exit code and output of each test. 
Negative assertions ARE supported (write a test that is expected to fail)

Usage: 
For positive tests (tests that should pass):
    Name input file as pass_<test name>
    Name output file as out_<test name>
    Note: file extensions can be part of the test name
For negative tests (tests that should fail):
    Name input file as fail<expected error code>_<test name>
Place all test files (input and output) in ./test_cases/

Run the python script

Debugging: 
    'out', 'fail', and 'pass' are reserved words, and may not be in <test name>
    executable must be in placed in . and named assemble.exe

@author: adeel
"""

import os
import subprocess
import shutil
import filecmp

tests = os.listdir("test_cases/")

if ("output" in os.listdir()):
    shutil.rmtree("output")
os.mkdir("output")

#print(tests)

for test in tests:
    if ("out" not in test): 
        print("Starting test: " + test)
        process = subprocess.Popen(["../assemble", "test_cases/"+test, "output/"+test])
        process.wait(timeout=5)
        print("Test Ended with Return Code: " + str(process.returncode))
        if ("fail" in test):
            expectedExit = int(test[4])
            assert expectedExit == process.returncode
        if ("pass" in test):
            assert 0 == process.returncode
            assert filecmp.cmp("test_cases/out_"+test[5:], "output/"+test)

print("All Tests Completed Properly")
