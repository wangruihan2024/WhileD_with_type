Require Import Coq.Strings.String.
Require Import Coq.ZArith.ZArith.
Require Import Coq.micromega.Psatz.
Local Open Scope string.
Local Open Scope Z.

Definition var_name: Type := string. (* 字符串表示变量名 *)

(* 这里先保留了bool的相关运算符 *)
Inductive binop : Type :=
  | OOr | OAnd
  | OLt | OLe | OGt | OGe | OEq | ONe
  | OPlus | OMinus | OMul | ODiv | OMod.

Inductive unop : Type :=
  | ONot | ONeg.

(* 带类型的WhileD语言 *)

Module Lang_WhileDT.

(* 定义WhileDT语言中变量的属性，short,int,long,long long,指针 *)
Inductive type : Type := 
  | Tshort : type
  | Tint : type
  | Tlong : type
  | Tlonglong : type
  | TBool : type
  | Tptr (t : type) : type.

(* 表达式的语法树，添加了类型转换 *)
Inductive expr : Type :=
  | EConst (n: Z): expr
  | EVar (x: var_name): expr
  | EBinop (op: binop) (e1 e2: expr): expr
  | EUnop (op: unop) (e: expr): expr
  | EDeref (e: expr): expr
  | EAddrOf (e: expr): expr
  | ETypeConv (t: type) (e: expr): expr.

(* 语句的语法树，添加了变量声明 *)
Inductive com : Type :=
  | CSkip: com
  | CAsgnVar (x: var_name) (e: expr): com
  | CAsgnDeref (e1 e2: expr): com (* 解引用情形的赋值，是左值，保留 *)
  | CSeq (c1 c2: com): com
  | CIf (e: expr) (c1 c2: com): com
  | CWhile (e: expr) (c: com): com
  | CVarDeclare (t : type) (x : var_name): com.

End Lang_WhileDT.

(* 定义常数范围和默认类型？一律留到类型检查的时候 🤔 ✅ *)

(* 语义模型的定义 笑死 保留一手 *)
Module Semantic_WhileDT.

Record state: Type := {
  var: var_name -> Z;
  mem: Z -> mem_val;
}.

Definition deref_sem (D: state -> SetMonadE.M Z):
  state -> SetMonadE.M Z :=
  fun s =>
    x <- D s;;
    match s.(mem) x with
    | Mem_NoPerm => abort
    | Mem_HasPerm Var_U => abort
    | Mem_HasPerm (Var_I n) => ret n
    end.

Definition var_addr_sem (X: var_name):
  state -> SetMonadE.M Z :=
  fun s => ret (s.(var) X).

Definition var_sem (X: var_name):
  state -> SetMonadE.M Z :=
  fun s =>
    y <- ret (s.(var) X);;
    match s.(mem) y with
    | Mem_NoPerm => abort
    | Mem_HasPerm Var_U => abort
    | Mem_HasPerm (Var_I n) => ret n
    end.

(* pdf里直接说表达式要分左值右值，直接定义在语义层也好，不能全留到后面类型检查再看🤔 *)
Record EDenote: Type := {
  lvalue: state -> SetMonadE.M Z;
  rvalue: state -> SetMonadE.M Z;
}.

Fixpoint eval_expr (e: expr): EDenote :=
  match e with
  | EConst n =>
    {| lvalue := fun _ => abort;
       rvalue := const_sem n |}
  | EVar X =>
    {| lvalue := var_addr_sem X;
       rvalue := var_sem X |}
  | EBinop op e1 e2 =>
    {| lvalue := fun _ => abort;
       rvalue := binop_sem op (eval_expr e1).(rvalue)
                              (eval_expr e2).(rvalue) |}
  | EUnop op e1 =>
    {| lvalue := fun _ => abort;
       rvalue := unop_sem op (eval_expr e1).(rvalue) |}
  | EDeref e1 =>
    {| lvalue := (eval_expr e1).(rvalue);
       rvalue := deref_sem (eval_expr e1).(rvalue) |}
  | EAddrOf e1 =>
    {| lvalue := fun _ => abort;
       rvalue := (eval_expr e1).(lvalue) |}
  end.

End Semantic_WhileDT.


