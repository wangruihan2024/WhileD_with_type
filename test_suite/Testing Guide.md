# Testing Guide

This document explains how to use the `run_test.sh` script to verify the compiler.

## 1. Prerequisites

Ensure the project is built and `bin/main` exists:

```bash
make
```

## 2. Running Tests
The script is located at `test_suite/scripts/run_test.sh`. It supports three modes:

### Mode 1: Run All Tests
Runs every test case found in cases.
```bash
./test_suite/scripts/run_test.sh
```

### Mode 2: Run a Specific Suite
Runs all tests within a specific subdirectory (e.g., test_cmd).
```bash
./test_suite/scripts/run_test.sh <test_suite_name>
```
e.g.:
```bash
./test_suite/scripts/run_test.sh test_cmd
```

### Mode 3: Run a Specific Test Case
Runs a single test case by specifying its path.
```bash
./test_suite/scripts/run_test.sh <test_case_relative_path>
```
e.g.:
```bash
./test_suite/scripts/run_test.sh test_cmd/cmd_01_io.txt
```

## 3. Results
- [PASS]: Output matches the expected AST.
- [FAIL]: Output differs.
- [SKIP]: Missing answer file (`.ans`).

The expected output files are located in `test_suite/answers/test_suite_name/`.