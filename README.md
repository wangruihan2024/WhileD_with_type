# 带类型 WhileD 语言 (WhileD with Type)

本仓库实现了带有简单类型系统的 WhileD 编程语言 (WhileDT)。

## 概览

本项目实现了 WhileDT 语言的编译器前端，包括词法分析、语法分析、AST（抽象语法树）生成和简单类型检查器。

### 项目结构

*   **`WhileDT 语法定义.md`**: WhileDT 语法和 AST 格式的详细定义

*   **`类型检查规则.pdf`**: WhileDT 语言类型系统的设计和转换规则说明

*   **`WhileDT.v`**: WhileDT 语言在 Coq 中的定义

*   **`src/`**: 编译器前端的源代码
    *   `lang.l`: Flex 词法分析器定义
    *   `lang.y`: Bison 语法分析器定义
    *   `lang.c/h`: AST 数据结构和打印逻辑
    *   `main.c`: 程序入口
    *   `checker.cpp`: 类型检查实现

*   **`test_suite/`**: 自动化测试框架
    *   `cases/`: 输入源代码文件
    *   `answers/`: 预期的 AST 输出
    *   `scripts/`: 生成和运行测试的脚本
    *   **`Testing Guide.md`**: 测试框架使用说明

*   **`Makefile`**

*   **`bin/`**: 包含最终的可执行文件 (main)

*   **`build/`**: 编译过程中生成的中间文件。关键是，这里包含从 Bison 和 Flex 定义派生的 C 源代码文件（`parser.c`, `lexer.c`）