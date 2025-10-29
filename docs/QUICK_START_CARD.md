# Hash Language - Quick Start Card

> Print this! Keep it by your desk!

---

```


HASH LANGUAGE - QUICK START
Version 1.0.0


```

## Installation (One-Time)

```powershell
cd HashLang
.\build.ps1      # Build compiler
.\install.ps1    # Install system-wide
# Restart terminal
```

---

## Essential Commands

```powershell
hash -cr file.hash     # Compile and run
hash -c file.hash      # Compile only
hash -r file.exe       # Run executable
hash --help            # Get help
hash --version         # Show version
```

---

## Basic Syntax

```hash
# Variables
let x: i32 = 42
let y: f64 = 3.14
let name: str = "Hash"

# Function
fn greet(name: str) -> void:
    print_str("Hello, ")
    print_str(name)
    println()

# Pure Function (no side effects)
pure fn add(a: i32, b: i32) -> i32:
    return a + b

# Main Function (required)
fn main() -> i32:
    greet("World")
    let sum: i32 = add(10, 20)
    print_i32(sum)
    return 0
```

---

## Common Types

```hash
i32        # 32-bit integer
i64        # 64-bit integer
f64        # 64-bit float
bool       # true or false
str        # String literal
void       # No return value
i32[]      # Array of i32
```

---

## Print Functions

```hash
print_i32(42)           # Print integer
print_f64(3.14)         # Print float
print_bool(true)        # Print boolean
print_str("Hello")      # Print string
println()               # Print newline
```

---

## Control Flow

```hash
# If/Else
if x > 10:
    print_str("Big")
else:
    print_str("Small")

# While Loop
let i: i32 = 0
while i < 10:
    print_i32(i)
    i = i + 1
```

---

## Math Functions

```hash
abs(-5)          # → 5
min(10, 20)      # → 10
max(10, 20)      # → 20
sqrt(16.0)       # → 4.0
pow(2.0, 3.0)    # → 8.0
```

---

## String Functions

```hash
len("Hello")              # → 5
str_concat("Hi", " ")     # → "Hi "
str_eq("a", "a")          # → true
upper("hello")            # → "HELLO"
lower("WORLD")            # → "world"
```

---

## File I/O

```hash
file_write("test.txt", "Hello")
let content: str = file_read("test.txt")
let exists: bool = file_exists("test.txt")
file_delete("test.txt")
```

---

## User Input

```hash
print_str("Enter number: ")
let num: i32 = read_int()

print_str("Enter text: ")
let text: str = read_line()
```

---

## Type Conversions

```hash
float(42)        # i32 → f64
int(3.14)        # f64 → i32
str_to_int("42") # str → i32
bool_to_str(true)# bool → str
```

---

## VS Code Snippets

```
Type + Tab to expand:

main    → Full main function
fn      → Function template
pure    → Pure function template
if      → If statement
while   → While loop
let     → Variable declaration
print   → Print function
```

---

## CLI Options

```powershell
-c <file>         # Compile
-r <file>         # Run
-cr <file>        # Compile + run
-o <name>         # Output name
--emit-llvm       # Generate LLVM IR
-v                # Verbose output
--version         # Show version
--help            # Show help
```

---

## Examples Location

```
C:\HashLang\examples\

00_hello.hash           - Hello World
01_basics.hash          - Fundamentals
04_pure_functions.hash  - Pure functions
20_strings.hash         - String manipulation
22_file_io.hash         - File operations
23_error_handling.hash  - Error handling
```

---

## Common Errors

```
ERROR: Indentation must be 4 spaces
→ Use 4 spaces, not tabs!

ERROR: Function must return a value
→ Add 'return 0' at end of main()

ERROR: Cannot access pure_local from non-pure function
→ Function must be marked 'pure'
```

---

## Quick Workflow

```powershell
# 1. Create file
code myprogram.hash

# 2. Write code (use snippets)
# Type: main<Tab>

# 3. Compile and run
hash -cr myprogram.hash

# 4. Iterate
```

---

## Documentation

```
docs/COMPLETE_GUIDE.md         - Everything!
docs/QUICK_REFERENCE.md        - One-page reference
docs/CLI_GUIDE.md              - CLI reference
docs/GETTING_STARTED.md        - Language tutorial
docs/CODE_COMPLETION.md        - VS Code guide
```

---

## Troubleshooting

```
Command not found?
→ Restart terminal!

No syntax colors?
→ Restart VS Code!

Compilation errors?
→ Check indentation (4 spaces)

Need help?
→ hash --help
```

---

## Pro Tips

 Use VS Code for IntelliSense
 Type snippets + Tab to expand
 Mark functions 'pure' when possible
 Use 4 spaces for indentation
 Check examples/ for patterns
 Run with -v for verbose output

---

```


Hash: Python-like syntax, C-level performance!

Questions? → docs/COMPLETE_GUIDE.md


```

---

**Print this card and keep it handy!** 

*Last Updated: v1.0.0*
