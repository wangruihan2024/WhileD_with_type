# WhileDT 语法定义

## 类型
- 基本类型：`int`, `short`, `long`, `long long`
- 指针类型：`<type> *` (例如：`int *`, `int **`)

## 表达式
- **常量**：自然数 (例如：`0`, `123`)
- **变量**：标识符 (例如：`x`, `var_name`)
- **括号**：`( <expr> )`
- **一元运算**：
  - 逻辑非：`! <expr>`
  - 负号/取反：`- <expr>`
  - 解引用：`* <expr>`
  - 取地址：`& <expr>`
- **类型转换**：`( <type> ) <expr>`
- **二元运算** (按优先级排序)：
  - `*`, `/`, `%`
  - `+`, `-`
  - `<`, `<=`, `>`, `>=`
  - `==`, `!=`
  - `&&`
  - `||`

## 命令

### Skip 命令 (空指令)
```
SKIP
```

### 声明命令
```
<type> <var_name>
```
示例：`int x`, `bool b`, `int *p`

### 赋值命令
```
<var_name> = <expression>
```
示例：`x = 1 + 2`

### 解引用赋值命令
```
* <expression> = <expression>
```
示例：`*p = 5`, `*(p + 1) = 10`

### 序列命令
```
<command> ; <command>
```
示例：`x = 1; y = 2`

### If 命令
```
if ( <expression> ) then { <command> } else { <command> }
```
示例：`if (x > 0) then { y = 1 } else { y = 2 }`

### While 命令
```
while ( <expression> ) do { <command> }
```
示例：`while (x > 0) do { x = x - 1 }`

## 抽象语法树 (AST) 格式

以以下格式输出 AST：

### 表达式
- `CONST(n)`
- `VAR(name)`
- `BINOP(op, left, right)` 其中 `op` 为 `ADD`, `SUB`, `MUL`, `DIV`, `MOD`, `LT`, `GT`, `LE`, `GE`, `EQ`, `NE`, `AND`, `OR`
- `UNOP(op, right)` 其中 `op` 为 `NOT`, `NEG`
- `DEREF(right)` (解引用)
- `ADDROF(right)` (取地址)
- `TYPECONV(type, right)` (类型转换)

### 命令
- `SKIP`
- `VARDECLARE(name)` (变量声明)
- `ASGN(name, expr)` (赋值)
- `ASGNDREF(addr_expr, val_expr)` (解引用赋值)
- `SEQ(cmd1, cmd2)` (顺序)
- `IF(cond, then_cmd, else)` (条件)
- `WHILE(cond, body_cmd)` (循环)