# 编译器和选项
CC = gcc
CFLAGS = -I./src -I./build -g -Wall
YACC = bison
LEX = flex

# 目录定义
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# 目标文件
TARGET = $(BIN_DIR)/main

# 源文件
LANG_C = $(SRC_DIR)/lang.c
LANG_H = $(SRC_DIR)/lang.h
LANG_Y = $(SRC_DIR)/lang.y
LANG_L = $(SRC_DIR)/lang.l
MAIN_C = $(SRC_DIR)/main.c

# 生成的文件
PARSER_C = $(BUILD_DIR)/parser.c
PARSER_H = $(BUILD_DIR)/parser.h
LEXER_C = $(BUILD_DIR)/lexer.c
LEXER_H = $(BUILD_DIR)/lexer.h

# 对象文件
LANG_O = $(BUILD_DIR)/lang.o
PARSER_O = $(BUILD_DIR)/parser.o
LEXER_O = $(BUILD_DIR)/lexer.o
MAIN_O = $(BUILD_DIR)/main.o

# 防止 make 删除中间文件
.SECONDARY: $(PARSER_C) $(PARSER_H) $(LEXER_C) $(LEXER_H)

# 默认目标
all: $(TARGET)

# 创建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# ==========================================
# 生成规则 (Bison & Flex)
# ==========================================

# Bison 规则
$(PARSER_C): $(LANG_Y) | $(BUILD_DIR)
	$(YACC) -o $(PARSER_C) -d -v $(LANG_Y)

$(PARSER_H): $(PARSER_C)

# Flex 规则 [修改版]
# 强制在当前目录生成 lexer.c/h，然后移动到 build 目录
# 这样可以兼容 lang.l 中可能存在的 %option outfile 设置
$(LEXER_C): $(LANG_L) $(PARSER_H) | $(BUILD_DIR)
	$(LEX) --header-file=lexer.h -o lexer.c $(LANG_L)
	mv lexer.c $(LEXER_C)
	mv lexer.h $(LEXER_H)

$(LEXER_H): $(LEXER_C)

# ==========================================
# 编译规则
# ==========================================

$(LANG_O): $(LANG_C) $(LANG_H) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(LANG_C) -o $(LANG_O)

$(PARSER_O): $(PARSER_C) $(PARSER_H) $(LEXER_H) $(LANG_H)
	$(CC) $(CFLAGS) -c $(PARSER_C) -o $(PARSER_O)

$(LEXER_O): $(LEXER_C) $(LEXER_H) $(PARSER_H) $(LANG_H)
	$(CC) $(CFLAGS) -c $(LEXER_C) -o $(LEXER_O)

$(MAIN_O): $(MAIN_C) $(LEXER_H) $(PARSER_H) $(LANG_H)
	$(CC) $(CFLAGS) -c $(MAIN_C) -o $(MAIN_O)

$(TARGET): $(LANG_O) $(PARSER_O) $(LEXER_O) $(MAIN_O) | $(BIN_DIR)
	$(CC) $(LANG_O) $(PARSER_O) $(LEXER_O) $(MAIN_O) -o $(TARGET)

# 清理
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	rm -f lexer.c lexer.h parser.c parser.h  # 清理可能遗留在根目录的文件

clean_logs:
	rm -f test_suite/logs/*.log

distclean: clean clean_logs

.PHONY: all clean clean_logs distclean