# Hash Language - Code Completion & IntelliSense Guide

##  Code Snippets

The Hash VS Code extension includes **30+ intelligent code snippets** for rapid development.

### How to Use

1. Type the **prefix** (trigger word)
2. Press `Tab` or `Enter` to expand
3. Use `Tab` to jump between fields
4. Start typing to fill in placeholders

---

## Available Snippets

### Functions

| Prefix | Expands To | Description |
|--------|------------|-------------|
| `fn` | Function definition | Creates a standard function |
| `pure` | Pure function | Creates a pure function (PINK keyword!) |
| `main` | Main function | Creates the main entry point |

**Examples:**
```hash
# Type: fn<Tab>
fn function_name(params) -> i32:
    return 0

# Type: pure<Tab>
pure fn function_name(params) -> i32:
    return 0

# Type: main<Tab>
fn main() -> i32:
    # Your code here
    return 0
```

---

### Variables

| Prefix | Expands To | Description |
|--------|------------|-------------|
| `let` | Immutable variable | Standard variable |
| `mut` | Mutable variable | Variable that can change |
| `pure_local` | Pure local variable | ORANGE keyword! Only for pure functions |

**Examples:**
```hash
# Type: let<Tab>
let name: i32 = 0

# Type: mut<Tab>
let mut name: i32 = 0

# Type: pure_local<Tab>
let pure_local name: i32 = 0
```

---

### Control Flow

| Prefix | Expands To | Description |
|--------|------------|-------------|
| `if` | If statement | Simple if |
| `ife` | If-else | If with else block |
| `while` | While loop | While loop |
| `for` | For-style loop | Counter-based loop |

**Examples:**
```hash
# Type: if<Tab>
if condition:
    # true block

# Type: ife<Tab>
if condition:
    # true block
else:
    # false block

# Type: while<Tab>
while condition:
    # loop body

# Type: for<Tab>
let mut i: i32 = 0
while i < 10:
    # Loop body
    let mut i = i + 1
```

---

### Console I/O

| Prefix | Expands To | Description |
|--------|------------|-------------|
| `prints` | Print string | Print with println() |
| `printi` | Print integer | Print i32 with println() |
| `printf` | Print float | Print f64 with println() |
| `readi` | Read integer | Read i32 from user |
| `readf` | Read float | Read f64 from user |

**Examples:**
```hash
# Type: prints<Tab>
print_str("message")
println()

# Type: printi<Tab>
print_i32(value)
println()

# Type: readi<Tab>
let value: i32 = read_i32()
```

---

### File I/O

| Prefix | Expands To | Description |
|--------|------------|-------------|
| `fread` | Safe file read | Read with existence check |
| `fwrite` | Safe file write | Write with success check |

**Examples:**
```hash
# Type: fread<Tab>
if file_exists("filename"):
    let content: str = file_read("filename")
    # Process content

# Type: fwrite<Tab>
let success: bool = file_write("filename", "content")
if success:
    # File written successfully
```

---

### String Operations

| Prefix | Expands To | Description |
|--------|------------|-------------|
| `len` | String length | Get length of string |
| `concat` | String concatenation | Concatenate strings |
| `streq` | String equality | Compare strings |

**Examples:**
```hash
# Type: len<Tab>
let length: i32 = len(string)

# Type: concat<Tab>
let result: str = str_concat(str1, str2)

# Type: streq<Tab>
if str_eq(str1, str2):
    # Strings are equal
```

---

### Math & Random

| Prefix | Expands To | Description |
|--------|------------|-------------|
| `math` | Math function | Choose from math functions |
| `rand` | Random number | Generate random number |

**Examples:**
```hash
# Type: math<Tab>
let result: f64 = sqrt(value)  # Choose: sqrt, abs, sin, cos, etc.

# Type: rand<Tab>
seed_random(hash_time())
let value: i32 = random_range(1, 100)  # or random()
```

---

### Utilities

| Prefix | Expands To | Description |
|--------|------------|-------------|
| `template` | Full program | Complete Hash program template |
| `hdr` | Section header | Commented section divider |
| `safe` | Error handling | Error handling pattern |

**Examples:**
```hash
# Type: template<Tab>
# Program Title
# Description

fn main() -> i32:
    print_str("Hello, Hash!")
    println()

    # Your code here

    return 0

# Type: hdr<Tab>
# ===================================
# Section Title
# ===================================

# Type: safe<Tab>
let result = operation
if check_success:
    # Success
else:
    # Handle error
```

---

##  IntelliSense Features

### Auto-Completion

VS Code will automatically suggest:
- **All 50 built-in functions** with their signatures
- **Keywords** (`pure`, `pure_local`, `fn`, `let`, etc.)
- **Types** (`i32`, `f64`, `bool`, `str`, etc.)
- **User-defined functions** from your code

### Bracket Pairing

Automatically closes:
- `()` - Parentheses
- `[]` - Brackets
- `{}` - Braces
- `""` - Quotes

### Auto-Indentation

- Press `:` at end of `fn`, `if`, `while`, `else` → auto-indent next line
- Press `Enter` → maintains indentation
- Smart dedent for `return`, `break`, `continue`

### Comment Toggle

- `Ctrl+/` - Toggle line comment (`#`)
- Works on single lines or selections

---

##  Syntax Highlighting

The extension highlights:
- **`pure`** - BRIGHT PINK (bold)
- **`pure_local`** - VIBRANT ORANGE (bold)
- **Built-in functions** - Warm orange
- **Control flow** - Bright red
- **Types** - Cyan blue
- **Strings** - Light blue
- **Numbers** - Cyan
- **Comments** - Gray italic

---

##  Pro Tips

1. **Multi-cursor editing**: `Alt+Click` for multiple cursors
2. **Line duplication**: `Shift+Alt+Down` to duplicate line
3. **Move line**: `Alt+Up/Down` to move lines
4. **Format selection**: Indent/dedent with `Ctrl+[` and `Ctrl+]`
5. **Quick file open**: `Ctrl+P` then type filename

---

##  Quick Reference

### Most Used Snippets

```
main<Tab>     → Full main function
fn<Tab>       → New function
pure<Tab>     → New pure function
let<Tab>      → Variable
if<Tab>       → If statement
while<Tab>    → While loop
prints<Tab>   → Print string
fread<Tab>    → Read file safely
```

### Common Patterns

```hash
# Safe division
if denominator != 0:
    let result: i32 = numerator / denominator

# Config file handling
if file_exists("config.txt"):
    let config: str = file_read("config.txt")
    # Process config

# User input with validation
let value: i32 = read_i32()
if value > 0:
    # Valid input
```

---

##  Getting Started

1. Open any `.hash` file
2. Start typing a snippet prefix
3. Watch IntelliSense suggest completions
4. Press `Tab` to expand
5. Fill in the placeholders

**Try it:** Type `main` and press Tab in any `.hash` file.

---
