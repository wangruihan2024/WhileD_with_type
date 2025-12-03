# 编译器和选项
CC  = gcc
CXX = g++
LD  = g++

CFLAGS  = -I./src -I./build -g -Wall
CXXFLAGS = -I./src -I./build -g -Wall

YACC = bison
LEX  = flex

# 目录定义
SRC_DIR   = src
BUILD_DIR = build
BIN_DIR   = bin

TARGET = $(BIN_DIR)/main

# 源文件
LANG_C  = $(SRC_DIR)/lang.c
LANG_H  = $(SRC_DIR)/lang.h
LANG_Y  = $(SRC_DIR)/lang.y
LANG_L  = $(SRC_DIR)/lang.l
MAIN_CPP = $(SRC_DIR)/main.cpp
CHECKER_CPP = $(SRC_DIR)/checker.cpp
CHECKER_H   = $(SRC_DIR)/checker.h

# Bison/Flex 生成文件
PARSER_C = $(BUILD_DIR)/parser.c
PARSER_H = $(BUILD_DIR)/parser.h
LEXER_C  = $(BUILD_DIR)/lexer.c
LEXER_H  = $(BUILD_DIR)/lexer.h

# 对象文件
LANG_O    = $(BUILD_DIR)/lang.o
PARSER_O  = $(BUILD_DIR)/parser.o
LEXER_O   = $(BUILD_DIR)/lexer.o
MAIN_O    = $(BUILD_DIR)/main.o
CHECKER_O = $(BUILD_DIR)/checker.o

# 禁用所有隐式规则和隐式变量
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables

# 防止 make 删除中间文件
.SECONDARY: $(PARSER_C) $(PARSER_H) $(LEXER_C) $(LEXER_H)

# 清空所有后缀规则
.SUFFIXES:

# 默认目标
all: $(TARGET)

# 创建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# ==========================================
# Bison
# ==========================================
$(PARSER_C): $(LANG_Y) | $(BUILD_DIR)
	$(YACC) -o $(PARSER_C) -d -v $(LANG_Y)
	@echo "Wrapping $(PARSER_H) with extern \"C\"..."
	@echo '#ifdef __cplusplus' > $(PARSER_H).tmp
	@echo 'extern "C" {' >> $(PARSER_H).tmp
	@echo '#endif' >> $(PARSER_H).tmp
	@cat $(PARSER_H) >> $(PARSER_H).tmp
	@echo '#ifdef __cplusplus' >> $(PARSER_H).tmp
	@echo '}' >> $(PARSER_H).tmp
	@echo '#endif' >> $(PARSER_H).tmp
	@mv $(PARSER_H).tmp $(PARSER_H)

$(PARSER_H): $(PARSER_C)

# ==========================================
# Flex
# ==========================================
$(LEXER_C): $(LANG_L) $(PARSER_H) | $(BUILD_DIR)
	$(LEX) --header-file=lexer.h -o lexer.c $(LANG_L)
	mv lexer.c $(LEXER_C)
	mv lexer.h $(LEXER_H)
	@echo "Wrapping $(LEXER_H) with extern \"C\"..."
	@echo '#ifdef __cplusplus' > $(LEXER_H).tmp
	@echo 'extern "C" {' >> $(LEXER_H).tmp
	@echo '#endif' >> $(LEXER_H).tmp
	@cat $(LEXER_H) >> $(LEXER_H).tmp
	@echo '#ifdef __cplusplus' >> $(LEXER_H).tmp
	@echo '}' >> $(LEXER_H).tmp
	@echo '#endif' >> $(LEXER_H).tmp
	@mv $(LEXER_H).tmp $(LEXER_H)

$(LEXER_H): $(LEXER_C)

# ==========================================
# 编译规则
# ==========================================

# C 文件
$(LANG_O): $(LANG_C) $(LANG_H) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LANG_C) -o $(LANG_O)

$(PARSER_O): $(PARSER_C) $(PARSER_H) $(LEXER_H) $(LANG_H)
	$(CC) $(CFLAGS) -c $(PARSER_C) -o $(PARSER_O)

$(LEXER_O): $(LEXER_C) $(LEXER_H) $(PARSER_H) $(LANG_H)
	$(CC) $(CFLAGS) -c $(LEXER_C) -o $(LEXER_O)

# C++ 文件
$(MAIN_O): $(MAIN_CPP) $(LEXER_H) $(PARSER_H) $(LANG_H) $(CHECKER_H)
	$(CXX) $(CXXFLAGS) -c $(MAIN_CPP) -o $(MAIN_O)

$(CHECKER_O): $(CHECKER_CPP) $(CHECKER_H) $(LANG_H)
	$(CXX) $(CXXFLAGS) -c $(CHECKER_CPP) -o $(CHECKER_O)

# ==========================================
# 链接（必须用 g++）
# ==========================================
$(TARGET): $(LANG_O) $(PARSER_O) $(LEXER_O) $(CHECKER_O) $(MAIN_O) | $(BIN_DIR)
	$(LD) $(LANG_O) $(PARSER_O) $(LEXER_O) $(CHECKER_O) $(MAIN_O) -o $(TARGET)

# ==========================================
# 清理
# ==========================================
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	rm -f lexer.c lexer.h parser.c parser.h

clean_logs:
	rm -f test_suite/logs/*.log

distclean: clean clean_logs

.PHONY: all clean clean_logs distclean
