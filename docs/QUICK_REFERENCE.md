# Hash Language - Quick Reference

## Syntax Overview

```hash
# Comments start with #

# Variable declarations
let x: i32 = 10          # Immutable (default)
let mut y: i32 = 20      # Mutable
let pure_local z: i32 = 5  # Pure-local (only accessible by pure functions)

# Function declarations
fn function_name(param: i32) -> i32:
    return param + 1

# Pure function (no side effects allowed)
pure fn pure_function(x: i32) -> i32:
    return x * 2

# Control flow
if condition:
    # code
else:
    # code

while condition:
    # code
```

## Data Types

| Type | Description | Example |
|------|-------------|---------|
| `i8`, `i16`, `i32`, `i64` | Signed integers | `let x: i32 = -42` |
| `u8`, `u16`, `u32`, `u64` | Unsigned integers | `let x: u32 = 42` |
| `f32`, `f64` | Floating point | `let x: f64 = 3.14` |
| `bool` | Boolean | `let x: bool = true` |
| `str` | String | (string literals not yet supported) |
| `void` | No return value | `fn print() -> void:` |

**Note:** Integer literals default to `i32`, float literals default to `f64`.

## Operators

### Arithmetic
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division
- `%` Modulo
- `-x` Negation

### Comparison
- `==` Equal
- `!=` Not equal
- `<` Less than
- `<=` Less than or equal
- `>` Greater than
- `>=` Greater than or equal

### Logical
- `&&` Logical AND
- `||` Logical OR
- `!` Logical NOT

### Bitwise
- `&` Bitwise AND
- `|` Bitwise OR
- `^` Bitwise XOR
- `<<` Left shift
- `>>` Right shift

## Built-in Functions

### Console I/O
```hash
print_i32(42)           # Print integer
print_i64(big_val)      # Print 64-bit integer
print_f64(3.14)         # Print float
print_bool(true)        # Print boolean (true/false)
println()               # Print blank line
```

### Type Conversions (Python-style)
```hash
int(3.14)               # float → int (same as f64_to_i32)
float(42)               # int → float (same as i32_to_f64)

# Verbose versions still work:
i32_to_i64(x)           # i32 → i64
i64_to_i32(x)           # i64 → i32 (truncates)
i64_to_f64(x)           # i64 → f64
f64_to_i64(x)           # f64 → i64 (truncates decimal)
```

### Math Functions (Python-style)
```hash
abs(-42)                # Absolute value (same as abs_i32)
min(10, 20)             # Minimum (same as min_i32)
max(10, 20)             # Maximum (same as max_i32)
sqrt(16.0)              # Square root (same as sqrt_f64)

# Verbose versions still work:
abs_i32(x)              # Absolute value for i32
min_i32(a, b)           # Minimum of two i32 values
max_i32(a, b)           # Maximum of two i32 values
sqrt_f64(x)             # Square root for f64
```

## Common Patterns

### Simple Calculation
```hash
fn main() -> i32:
    let x: i32 = 10
    let y: i32 = 5
    let sum: i32 = x + y
    print_i32(sum)
    return 0
```

### Using Type Conversions (Clean Python-style)
```hash
fn main() -> i32:
    let int_val: i32 = 100
    let float_val: f64 = float(int_val)  # 
    print_f64(float_val)  # 100.000000
    
    let pi: f64 = 3.14
    let truncated: i32 = int(pi)  # Simple and clear
    print_i32(truncated)  # 3
    
    return 0
```

### Using Math Functions (Python-like)
```hash
fn main() -> i32:
    let a: i32 = 3
    let b: i32 = 4
    let c_sq: i32 = a*a + b*b
    let c: f64 = sqrt(float(c_sq))  # Clean and readable
    print_f64(c)  # 5.000000
    return 0
```

### Pure Functions
```hash
let pure_local CONST: i32 = 100

pure fn double(x: i32) -> i32:
    return x * 2

pure fn use_constant() -> i32:
    return CONST  # OK: pure function can access pure_local

fn regular_function() -> i32:
    # return CONST  # ERROR: regular function can't access pure_local
    return 0
```

### Loops and Conditionals
```hash
fn factorial(n: i32) -> i32:
    let mut result: i32 = 1
    let mut i: i32 = 1
    while i <= n:
        result = result * i
        i = i + 1
    return result

fn absolute(x: i32) -> i32:
    if x < 0:
        return -x
    else:
        return x
```

## Compilation

```powershell
# Compile a program
.\build\Release\hashc.exe program.hash

# Run the compiled program
.\a.exe

# Specify output filename
.\build\Release\hashc.exe program.hash -o myprogram.exe
.\myprogram.exe

# Generate LLVM IR for inspection
.\build\Release\hashc.exe program.hash --emit-llvm
```

## Examples

Hash comes with 10 comprehensive examples:

1. **00_hello.hash** - First program with I/O
2. **01_basics.hash** - Variables, types, operators
3. **02_control_flow.hash** - If/else, while loops
4. **03_functions.hash** - Function patterns
5. **04_pure_functions.hash** - Pure functions, pure_local
6. **05_algorithms.hash** - Classic algorithms
7. **06_types.hash** - Type system deep dive
8. **07_recursion.hash** - Recursive patterns
9. **08_patterns.hash** - Advanced programming patterns
10. **09_builtins.hash** - All built-in functions

Run all examples:
```powershell
.\test_examples.ps1
```

## Error Messages

Hash provides helpful error messages:

```
Error: Cannot access pure-local variable 'counter' from a non-pure function
  --> example.hash:15:12
   |
15 |     return counter
   |            ^^^^^^^
   |
Suggestion: Mark this function as 'pure', or remove the 'pure_local' modifier from 'counter'
```

## Best Practices

1. **Use pure functions for logic** - Mark functions `pure` when they don't need side effects
2. **Explicit types** - Always declare variable types for clarity
3. **Use built-in functions** - Prefer `abs_i32()`, `min_i32()`, etc. over manual implementations
4. **Type conversions** - Use conversion functions when mixing i32 and f64
5. **Print for debugging** - Use `print_i32()`, `print_f64()` to see what your code is doing

## Further Reading

- [Full Built-ins Documentation](BUILTINS.md)
- [Language Guide](../examples/GUIDE.md)
- [Examples Index](../examples/INDEX.md)
