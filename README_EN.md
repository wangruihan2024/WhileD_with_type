# WhileD with Type

This repository contains an implementation of the WhileD programming language with a simple type system (WhileDT).

## Overview

This project implements a compiler frontend for the WhileDT language, including lexical analysis, parsing, AST generation and type checker.

### Project Structure

*   **`WhileDT Syntax Definition.md`**: Detailed definition of the WhileDT syntax and AST format.

*   **`类型检查规则.pdf`**: Type checking rules for WhileDT (only in Chinese).

*   **`WhileDT.v`**: Definition of WhileDT language in coq.

*   **`src/`**: Source code for the compiler.
    *   `lang.l`: Flex lexer definition.
    *   `lang.y`: Bison parser definition.
    *   `lang.c/h`: AST data structures and printing logic.
    *   `main.c`: Entry point.
    *   `checker.cpp`: Type checking implementation.

*   **`test_suite/`**: Automated testing framework.
    *   `cases/`: Input source code files.
    *   `answers/`: Expected AST outputs.
    *   `scripts/`: Scripts to generate and run tests.
    *   **`Testing Guide.md`**: Instructions on how to run the test suite.

*   **`Makefile`**

*   **`bin/`**: Contains the final executable (main).

*   **`build/`**: Intermediate files generated during compilation. Crucially, this contains the generated C source files (`parser.c`, `lexer.c`) derived from Bison and Flex definitions.