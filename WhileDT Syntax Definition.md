# WhileDT Syntax Definition

## Types
- Basic types: `int`, `short`, `long`, `long long`, `bool`
- Pointer types: `<type> *` (e.g., `int *`, `int **`)

## Expressions
- **Constants**: Natural numbers (e.g., `0`, `123`)
- **Variables**: Identifiers (e.g., `x`, `var_name`)
- **Parentheses**: `( <expr> )`
- **Unary Operations**:
  - Logical NOT: `! <expr>`
  - Negation: `- <expr>`
  - Dereference: `* <expr>`
  - Address of: `& <expr>`
- **Type Conversion**: `( <type> ) <expr>`
- **Binary Operations** (ordered by precedence):
  - `*`, `/`, `%`
  - `+`, `-`
  - `<`, `<=`, `>`, `>=`
  - `==`, `!=`
  - `&&`
  - `||`
- **Memory Allocation**: `malloc( <expr> )`
- **Input**:
  - `read_int()`
  - `read_char()`

## Commands

### Skip Command
```
SKIP
```

### Declare Command
```
<type> <var_name>
```
Example: `int x`, `bool b`, `int *p`

### Assign Command
```
<var_name> = <expression>
```
Example: `x = 1 + 2`

### Assign Dereferenced Command
```
* <expression> = <expression>
```
Example: `*p = 5`, `*(p + 1) = 10`

### Sequence Command
```
<command> ; <command>
```
Example: `x = 1; y = 2`

### If Command
```
if ( <expression> ) then { <command> } else { <command> }
```
Example: `if (x > 0) then { y = 1 } else { y = 2 }`

### While Command
```
while ( <expression> ) do { <command> }
```
Example: `while (x > 0) do { x = x - 1 }`

### Output Commands
- `write_int( <expression> )`
- `write_char( <expression> )`

## Abstract Syntax Tree (AST) Format

The interpreter outputs the AST in the following format:

### Expressions
- `CONST(n)`
- `VAR(name)`
- `BINOP(op, left, right)` where `op` is `ADD`, `SUB`, `MUL`, `DIV`, `MOD`, `LT`, `GT`, `LE`, `GE`, `EQ`, `NE`, `AND`, `OR`
- `UNOP(op, right)` where `op` is `NOT`, `NEG`
- `DEREF(right)`
- `ADDROF(right)`
- `TYPECONV(type, right)`
- `MALLOC(size)`
- `READ_INT()`
- `READ_CHAR()`

### Commands
- `SKIP`
- `VARDECLARE(name)`
- `ASGN(name, expr)`
- `ASGNDREF(addr_expr, val_expr)`
- `SEQ(cmd1, cmd2)`
- `IF(cond, then_cmd, else_cmd)`
- `WHILE(cond, body_cmd)`
- `WRITE_INT(expr)`
- `WRITE_CHAR(expr)`