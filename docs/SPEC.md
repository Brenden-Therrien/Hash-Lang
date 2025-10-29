# Hash Language Specification

## Version 0.1.0

## Table of Contents

1. [Introduction](#introduction)
2. [Lexical Structure](#lexical-structure)
3. [Types](#types)
4. [Expressions](#expressions)
5. [Statements](#statements)
6. [Functions](#functions)
7. [Behavior-Aware Features](#behavior-aware-features)

## Introduction

Hash is a statically typed, compiled programming language designed for systems programming with a focus on safety, performance, and predictability. Hash features Python-like syntax for readability while maintaining C-level control and performance.

## Lexical Structure

### Keywords

```
fn      pure    let     mut     if      else    while   for     return
import  struct  enum    match   true    false
```

### Types
```
i8  i16  i32  i64  u8  u16  u32  u64  f32  f64  bool  void  str
```

### Access Modifiers
```
pure_local    side_effect
```

### Operators

**Arithmetic**: `+` `-` `*` `/` `%`

**Comparison**: `==` `!=` `<` `<=` `>` `>=`

**Logical**: `&&` `||` `!`

**Bitwise**: `&` `|` `^` `~` `<<` `>>`

**Assignment**: `=`

### Comments

```hash
# Single line comment
```

### Indentation

Hash uses significant whitespace (like Python). Blocks are defined by indentation:

```hash
fn example():
    # This is indented
    let x: i32 = 5
    if x > 0:
        # This is further indented
        return x
```

## Types

### Primitive Types

- **Signed Integers**: `i8`, `i16`, `i32`, `i64`
- **Unsigned Integers**: `u8`, `u16`, `u32`, `u64`
- **Floating Point**: `f32`, `f64`
- **Boolean**: `bool` (values: `true`, `false`)
- **String**: `str`
- **Void**: `void`

### Type Declaration

```hash
let variable_name: type = value
```

## Expressions

### Literals

```hash
42          # Integer literal (i64)
3.14        # Float literal (f64)
"hello"     # String literal
true        # Boolean literal
false       # Boolean literal
```

### Binary Operations

```hash
a + b       # Addition
a - b       # Subtraction
a * b       # Multiplication
a / b       # Division
a % b       # Modulo
a == b      # Equality
a != b      # Inequality
a < b       # Less than
a <= b      # Less than or equal
a > b       # Greater than
a >= b      # Greater than or equal
a && b      # Logical AND
a || b      # Logical OR
a & b       # Bitwise AND
a | b       # Bitwise OR
a ^ b       # Bitwise XOR
a << b      # Left shift
a >> b      # Right shift
```

### Unary Operations

```hash
-x          # Negation
!x          # Logical NOT
~x          # Bitwise NOT
```

### Function Calls

```hash
function_name(arg1, arg2, arg3)
```

## Statements

### Variable Declaration

```hash
# Immutable variable
let x: i32 = 10

# Mutable variable
let mut y: i32 = 20
y = 30

# Pure-local variable
let pure_local z: i32 = 42
```

### Assignment

```hash
variable = expression
```

### If Statement

```hash
if condition:
    # then block
    statement1
    statement2
else:
    # else block
    statement3
```

### While Loop

```hash
while condition:
    # loop body
    statement1
    statement2
```

### Return Statement

```hash
return expression
return  # for void functions
```

## Functions

### Function Declaration

```hash
fn function_name(param1: type1, param2: type2) -> return_type:
    # function body
    return value
```

### Pure Function Declaration

```hash
pure fn pure_function(x: i32, y: i32) -> i32:
    return x + y
```

### Function Examples

```hash
# Function with no return value
fn print_number(n: i32) -> void:
    return

# Function with return value
fn add(a: i32, b: i32) -> i32:
    return a + b

# Pure function
pure fn multiply(a: i32, b: i32) -> i32:
    return a * b
```

## Behavior-Aware Features

### Pure Functions

A function marked with `pure` is guaranteed to have no side effects. Pure functions:

- Cannot modify global variables
- Cannot call non-pure functions
- Cannot modify their parameters
- Must return the same result for the same inputs

**Example:**

```hash
pure fn calculate_area(width: i32, height: i32) -> i32:
    return width * height  # OK - pure computation

pure fn invalid_pure() -> void:
    modify_global()  # ERROR - cannot call non-pure function
```

### Pure-Local Variables

Variables marked with `pure_local` can only be accessed by pure functions. This enforces separation between pure and impure code:

```hash
let pure_local constant: i32 = 42

pure fn get_constant() -> i32:
    return constant  # OK

fn impure_access() -> i32:
    return constant  # ERROR - non-pure function cannot access pure_local
```

### Benefits

1. **Compile-Time Guarantees**: The compiler enforces purity constraints
2. **Easier Reasoning**: Pure functions are deterministic
3. **Safe Parallelization**: Pure functions can be safely executed concurrently
4. **Better Testing**: Pure functions are easier to unit test

## Grammar (EBNF)

```ebnf
program         ::= (function_decl | global_var)*

function_decl   ::= "pure"? "fn" IDENTIFIER "(" parameters? ")" ("->" type)? ":" block

parameters      ::= parameter ("," parameter)*
parameter       ::= IDENTIFIER ":" type

global_var      ::= "let" ("mut" | "pure_local")? IDENTIFIER ":" type ("=" expression)?

block           ::= INDENT statement+ DEDENT

statement       ::= var_decl
                 |  assignment
                 |  if_stmt
                 |  while_stmt
                 |  return_stmt
                 |  expr_stmt

var_decl        ::= "let" ("mut" | "pure_local")? IDENTIFIER ":" type ("=" expression)?

assignment      ::= IDENTIFIER "=" expression

if_stmt         ::= "if" expression ":" block ("else" ":" block)?

while_stmt      ::= "while" expression ":" block

return_stmt     ::= "return" expression?

expr_stmt       ::= expression

expression      ::= logical_or

logical_or      ::= logical_and ("||" logical_and)*

logical_and     ::= bitwise_or ("&&" bitwise_or)*

bitwise_or      ::= bitwise_xor ("|" bitwise_xor)*

bitwise_xor     ::= bitwise_and ("^" bitwise_and)*

bitwise_and     ::= equality ("&" equality)*

equality        ::= comparison (("==" | "!=") comparison)*

comparison      ::= shift (("<" | "<=" | ">" | ">=") shift)*

shift           ::= term (("<<" | ">>") term)*

term            ::= factor (("+" | "-") factor)*

factor          ::= unary (("*" | "/" | "%") unary)*

unary           ::= ("-" | "!" | "~")? primary

primary         ::= INTEGER
                 |  FLOAT
                 |  STRING
                 |  "true"
                 |  "false"
                 |  IDENTIFIER ("(" arguments? ")")?
                 |  "(" expression ")"

arguments       ::= expression ("," expression)*

type            ::= "i8" | "i16" | "i32" | "i64"
                 |  "u8" | "u16" | "u32" | "u64"
                 |  "f32" | "f64"
                 |  "bool" | "void" | "str"
```

## Future Extensions

- Structs and methods
- Enums and pattern matching
- Generic types
- Module system
- Traits/Interfaces
- Array and slice types
- Pointer types
- Inline assembly
