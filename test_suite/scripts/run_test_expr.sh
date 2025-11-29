#!/bin/bash
# filepath: test_suite/scripts/run_tests.sh

# ================= 配置区域 =================
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BASE_DIR="$(dirname "$SCRIPT_DIR")"  # test_suite 目录
PROJECT_ROOT="$(dirname "$BASE_DIR")" # 项目根目录

TEST_DIR="$BASE_DIR/cases"
ANS_DIR="$BASE_DIR/answers"
RESULT_DIR="$BASE_DIR/logs"
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

# 确保结果目录存在
mkdir -p "$RESULT_DIR"

run_single_test() {
    local input_file="$1"
    local base_name=$(basename "$input_file" .txt)
    
    # 构建文件路径
    local real_input="$TEST_DIR/${base_name}.txt"
    local result_file="$RESULT_DIR/${base_name}.log"
    local ans_file="$ANS_DIR/${base_name}.ans"

    if [ ! -f "$real_input" ]; then
        echo -e "${RED}[ERROR]${NC} 测试文件不存在: $real_input"
        return 1
    fi

    # 运行程序
    "$EXECUTABLE" < "$real_input" &> "$result_file"

    # 检查答案文件
    if [ ! -f "$ans_file" ]; then
        echo -e "${YELLOW}[SKIP]${NC} ${base_name}: 缺少答案文件"
        return 0
    fi

    # 比对结果
    if diff -q -w -B "$result_file" "$ans_file" > /dev/null 2>&1; then
        echo -e "${GREEN}[PASS]${NC} ${base_name}"
        return 0
    else
        echo -e "${RED}[FAIL]${NC} ${base_name}"
        echo -e "${BLUE}       运行命令查看差异:${NC} diff $result_file $ans_file"
        return 1
    fi
}

# 主逻辑
main() {
    local failed_count=0
    local passed_count=0
    local skipped_count=0

    if [ "$#" -ge 1 ]; then
        # 运行指定的测试
        echo -e "${BLUE}运行单个测试...${NC}"
        run_single_test "$1"
        exit $?
    else
        # 运行所有测试
        echo -e "${BLUE}=== 运行所有 expr 测试 ===${NC}"
        
        local test_files=("$TEST_DIR"/*.txt)
        if [ ! -e "${test_files[0]}" ]; then
            echo -e "${YELLOW}警告: $TEST_DIR 目录下没有测试文件${NC}"
            exit 0
        fi

        for file in "${test_files[@]}"; do
            run_single_test "$file"
            case $? in
                0) ((passed_count++)) ;;
                1) ((failed_count++)) ;;
            esac
        done

        # 输出统计信息
        echo ""
        echo -e "${BLUE}=== 测试统计 ===${NC}"
        echo -e "${GREEN}通过: $passed_count${NC}"
        echo -e "${RED}失败: $failed_count${NC}"
        
        if [ $failed_count -gt 0 ]; then
            exit 1
        else
            exit 0
        fi
    fi
}

main "$@"