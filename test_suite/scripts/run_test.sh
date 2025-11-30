#!/bin/bash
# filepath: test_suite/scripts/run_test.sh

# ================= 配置区域 =================
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BASE_DIR="$(dirname "$SCRIPT_DIR")"  # test_suite 目录
PROJECT_ROOT="$(dirname "$BASE_DIR")" # 项目根目录

CASES_ROOT="$BASE_DIR/cases"
ANS_ROOT="$BASE_DIR/answers"
LOGS_ROOT="$BASE_DIR/logs"
EXECUTABLE="$PROJECT_ROOT/bin/main"

# 颜色定义
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'
# ===========================================

# 检查可执行文件
if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${RED}错误: 未找到可执行文件 '$EXECUTABLE'${NC}"
    echo "请先在项目根目录运行 'make' 进行编译。"
    exit 1
fi

# 统计变量
PASS_COUNT=0
FAIL_COUNT=0
SKIP_COUNT=0

# 核心测试函数
# 参数: 相对于 cases 目录的文件路径 (例如 "test_cmd/cmd_1.txt" 或 "expr_1.txt")
run_single_case() {
    local rel_path="$1"
    local input_file="$CASES_ROOT/$rel_path"
    
    # 提取不带后缀的文件名和目录名
    # 例如 rel_path="test_cmd/a.txt" -> dir_name="test_cmd", base_name="a"
    local dir_name=$(dirname "$rel_path")
    local base_name=$(basename "$rel_path" .txt)
    
    # 构建输出和答案的路径 (保持目录结构)
    local log_dir="$LOGS_ROOT/$dir_name"
    local log_file="$log_dir/${base_name}.log"
    local ans_file="$ANS_ROOT/$rel_path" # 假设答案文件结构与 cases 完全一致
    ans_file="${ans_file%.txt}.ans"       # 替换后缀 .txt -> .ans

    # 确保日志目录存在
    mkdir -p "$log_dir"

    if [ ! -f "$input_file" ]; then
        echo -e "${RED}[ERROR]${NC} 文件不存在: $input_file"
        return
    fi

    # 运行程序
    "$EXECUTABLE" < "$input_file" &> "$log_file"

    # 检查答案
    if [ ! -f "$ans_file" ]; then
        echo -e "${YELLOW}[SKIP]${NC} $rel_path (无答案文件)"
        ((SKIP_COUNT++))
        return
    fi

    # 比对
    if diff -q -w -B "$log_file" "$ans_file" > /dev/null 2>&1; then
        echo -e "${GREEN}[PASS]${NC} $rel_path"
        ((PASS_COUNT++))
    else
        echo -e "${RED}[FAIL]${NC} $rel_path"
        echo -e "       Diff: diff $log_file $ans_file"
        ((FAIL_COUNT++))
    fi
}

# ================= 主逻辑 =================

TARGET="$1"

if [ -z "$TARGET" ]; then
    # 模式 1: 无参数，运行所有测试
    echo -e "${BLUE}=== 运行所有测试 ===${NC}"
    # 使用 find 查找 cases 下所有的 .txt 文件
    # -type f: 只找文件
    # -name "*.txt": 只找 txt
    # sed 处理: 去掉前缀 "$CASES_ROOT/"，得到相对路径
    find "$CASES_ROOT" -type f -name "*.txt" | while read file; do
        # 获取相对路径
        rel_path="${file#$CASES_ROOT/}"
        # 去掉开头的 / (如果存在)
        rel_path="${rel_path#/}"
        run_single_case "$rel_path"
    done

elif [ -d "$CASES_ROOT/$TARGET" ]; then
    # 模式 2: 参数是目录 (例如 test_cmd)
    echo -e "${BLUE}=== 运行目录: $TARGET ===${NC}"
    # 遍历该目录下的 txt
    for file in "$CASES_ROOT/$TARGET"/*.txt; do
        # 检查 glob 是否匹配到了文件
        [ -e "$file" ] || continue
        
        rel_path="${file#$CASES_ROOT/}"
        rel_path="${rel_path#/}"
        run_single_case "$rel_path"
    done

elif [ -f "$CASES_ROOT/$TARGET" ]; then
    # 模式 3: 参数是具体文件 (例如 test_cmd/cmd_1.txt)
    echo -e "${BLUE}=== 运行单文件: $TARGET ===${NC}"
    run_single_case "$TARGET"

else
    echo -e "${RED}错误: 找不到测试目标 '$TARGET'${NC}"
    echo "请检查 '$CASES_ROOT/$TARGET' 是否存在。"
    exit 1
fi

# 由于管道可能导致子 shell 变量丢失，这里简单输出结束信息
# 如果需要精确统计，可以将 find 的结果存入数组再遍历
echo ""
echo "测试运行结束。"