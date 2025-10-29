# Hash Language - Complete Setup & Developer Guide

## What is Hash?

Hash is a modern systems programming language that combines:
- Python-like syntax - Clean and readable
- C-level performance - LLVM-powered compilation
- Pure functional features - Optional purity system
- Professional IDE support - VS Code extension with IntelliSense

### Hash in 30 Seconds

```hash
# Type this and watch it compile to native code
fn main() -> i32:
    print_str("Hello, Hash!")
    println()
    return 0
```

Features:
- 50 built-in functions
- Static typing with inference
- Pure functions for safety
- String manipulation
- File I/O
- Math, trigonometry, logarithms

## Quick Start (5 Minutes)

### Step 1: Build Everything (2 minutes)

```powershell
# Clone or navigate to Hash directory
cd HashLang

# This one command does everything:
.\build.ps1

# It will:
# - Build the Hash compiler
# - Install VS Code extension
# - Set up IntelliSense
# - Configure syntax highlighting
```

### Step 2: Restart VS Code (1 minute)

Close VS Code completely and reopen it.

### Step 3: Write Your First Program (2 minutes)

1. Create a new file: `hello.hash`
2. Type `main` and press `Tab` for instant template
3. Add your code:

```hash
fn main() -> i32:
    print_str("Hello from Hash!")
    println()
    return 0
```

4. Notice the vibrant colors:
   - Keywords highlighted
   - Built-in functions in warm orange
   - Strings in light blue

### Step 4: Compile & Run

```powershell
.\build\Release\hashc.exe hello.hash
.\hello.exe
# Output: Hello from Hash
```

You're now a Hash developer.

## VS Code Extension Features

### Syntax Highlighting

The Hash extension provides clear syntax highlighting:

| Feature | Color | Why |
|---------|-------|-----|
| `pure` keyword | Bright pink (bold) | Highlights pure functions |
| `pure_local` keyword | Vibrant orange (bold) | Distinguishes from regular vars |
| Built-in functions | Warm orange | Easy to spot standard library |
| Control flow (`if`, `while`) | Bright red | Flow control is obvious |
| Types (`i32`, `f64`, `bool`) | Cyan | Type safety at a glance |
| Strings | Light blue | Readable and distinct |

**See it in action:** Open `examples/syntax_showcase.hash`

### IntelliSense & Code Completion

**30+ intelligent snippets** for rapid coding:

#### Most Popular Snippets

| Type | Press Tab | Get |
|------|-----------|-----|
| `main` | Tab | Full main function template |
| `fn` | Tab | Function with return type |
| `pure` | Tab | Pure function (PINK!) |
| `let` | Tab | Variable declaration |
| `if` | Tab | If statement |
| `while` | Tab | While loop |
| `prints` | Tab | Print string + newline |
| `fread` | Tab | Safe file read (with checks) |

#### Advanced Features

- **Tab stops**: Jump between fields with Tab
- **Dropdowns**: Choose from common options
- **Best practices**: Built-in error handling
- **Smart defaults**: Type hints included

**Try it:** Type `fread` + Tab in any `.hash` file.

```hash
# You get this automatically:
if file_exists("filename"):
    let content: str = file_read("filename")
    # Process content
```

### Smart Editor Behaviors

-  Auto-close brackets, quotes, braces
-  Auto-indent after `:`, `fn`, `if`, `while`
-  Comment toggle with `Ctrl+/`
-  Bracket pair highlighting
-  Smart dedent for `return`, `break`

---

##  Language Features

### Pure Functions

Hash has a **compile-time purity system**:

```hash
# This keyword is BRIGHT PINK - you can't miss it
pure fn calculate(x: i32) -> i32:
    # This keyword is VIBRANT ORANGE
    let pure_local result: i32 = x * 2
    return result

# Pure functions:
#  Can't have side effects
#  Can't call non-pure functions
#  Can access pure_local variables
#  Always return the same output for same input
```

**Why use pure functions?**
- **Safety**: No hidden side effects
- **Testing**: Predictable and testable
- **Optimization**: Compiler can optimize aggressively
- **Reasoning**: Easy to understand behavior

### String Operations (Python-style)

```hash
let s: str = "Hello, Hash!"
let length: i32 = len(s)              # Python-style
let upper_s: str = upper(s)           # HELLO, HASH
let lower_s: str = lower(s)           # hello, hash
let combined: str = str_concat("Hi", "!") # Hi
let equal: bool = str_eq("a", "b")    # false
```

### File I/O (Safe by Design)

```hash
# Always check first
if file_exists("config.txt"):
    let content: str = file_read("config.txt")
    print_str(content)
    
# Write with success checking
let success: bool = file_write("output.txt", "data")
if success:
    print_str("File written!")
    
# Delete files
let deleted: bool = file_delete("temp.txt")
```

### Math Functions (Comprehensive)

```hash
# Basic math
abs(-5)          # 5
min(10, 20)      # 10
max(10, 20)      # 20
sqrt(16.0)       # 4.0
pow(2.0, 3.0)    # 8.0

# Rounding
floor(3.7)       # 3.0
ceil(3.2)        # 4.0
round(3.5)       # 4.0

# Trigonometry
sin(0.0)         # 0.0
cos(0.0)         # 1.0
tan(0.0)         # 0.0

# Logarithms
log(2.718)       # ~1.0 (natural log)
log10(100.0)     # 2.0
log2(8.0)        # 3.0
exp(1.0)         # ~2.718 (e^x)
```

### Random Numbers

```hash
seed_random(hash_time())           # Seed with timestamp
let r1: i32 = random()             # 0-32767
let r2: i32 = random_range(1, 100) # 1-100 (inclusive)
```

---

##  Documentation Guide

Hash has **comprehensive documentation**:

### For Beginners

1. **README.md** - Start here for overview
2. **docs/QUICK_REFERENCE.md** - One-page cheat sheet
3. **docs/CODE_COMPLETION.md** - All IntelliSense snippets
4. **examples/00_hello.hash** - Your first program

### For Learning

5. **examples/01_basics.hash** - Variables and types
6. **examples/02_control_flow.hash** - If/else, loops
7. **examples/03_functions.hash** - Function patterns
8. **examples/04_pure_functions.hash** - Pure system
9. All 23 examples in order

### For Reference

10. **docs/BUILTINS.md** - All 50 built-in functions
11. **docs/LANGUAGE.md** - Complete language spec
12. **docs/SYNTAX_COLORS.md** - Color reference
13. **CHANGELOG.md** - Feature timeline

### For Advanced Users

14. **docs/EXAMPLES.md** - Example index
15. **examples/23_error_handling.hash** - Error patterns
16. **examples/08_patterns.hash** - Advanced patterns

---

##  Example Workflow

### Creating a New Project

```powershell
# 1. Create a project file
# Type in VS Code: template<Tab>
# You get a complete program template

# 2. Use IntelliSense
# Type: fn<Tab>  → Function template
# Type: let<Tab> → Variable declaration
# Type: if<Tab>  → If statement

# 3. Compile
.\build\Release\hashc.exe myproject.hash

# 4. Run
.\myproject.exe
```

### Working with Files

```hash
fn main() -> i32:
    # Type: fread<Tab> to get this automatically:
    if file_exists("input.txt"):
        let data: str = file_read("input.txt")
        let upper_data: str = upper(data)
        
        # Type: fwrite<Tab>
        let success: bool = file_write("output.txt", upper_data)
        if success:
            print_str("Processed successfully!")
    
    return 0
```

### Using Pure Functions

```hash
# Global constants (ORANGE keyword)
let pure_local PI: f64 = 3.14159

# Pure function (PINK keyword)
pure fn circle_area(radius: f64) -> f64:
    return PI * radius * radius

fn main() -> i32:
    let area: f64 = circle_area(5.0)
    print_f64(area)
    println()
    return 0
```

---

##  Pro Tips

### VS Code Tips

1. **Trigger IntelliSense**: `Ctrl+Space` anytime
2. **Navigate snippets**: Use `Tab` to jump between fields
3. **Multi-cursor**: `Alt+Click` to add cursors
4. **Duplicate line**: `Shift+Alt+Down`
5. **Move line**: `Alt+Up/Down`
6. **Comment toggle**: `Ctrl+/`
7. **Format**: `Ctrl+[` and `Ctrl+]` for indent/dedent

### Hash Tips

1. **Use pure functions** for logic without side effects
2. **Check file existence** before reading
3. **Type conversions** are explicit (good for safety!)
4. **Print for debugging** - use `print_*` functions liberally
5. **Explore examples** - All 23 compile and run

### Workflow Tips

1. **One build command**: `.\build.ps1` does everything
2. **Test suite**: `.\test_examples.ps1` runs all examples
3. **Syntax showcase**: Open `examples/syntax_showcase.hash`
4. **Color theme**: Switch to "Hash Vibrant Theme" in VS Code
5. **Quick reference**: Keep `docs/QUICK_REFERENCE.md` handy

---

## Troubleshooting

### Extension Not Working?

**Problem**: Colors not showing, IntelliSense not working

**Solution**:
1. Check file has `.hash` extension
2. Restart VS Code (close all windows)
3. Run `.\build.ps1` again
4. Check bottom-right of VS Code says "Hash"

### Compile Errors?

**Problem**: Code won't compile

**Solution**:
1. Check syntax (colons after `fn`, `if`, `while`)
2. Check types (all variables need types)
3. Check pure functions (can't call non-pure)
4. Look at error message (it's helpful!)

### IntelliSense Not Triggering?

**Problem**: Snippets not appearing

**Solution**:
1. Press `Ctrl+Space` manually
2. Wait a second (first activation takes time)
3. Make sure file is saved as `.hash`
4. Restart VS Code

---

##  Hash at a Glance

### Language Stats

- **Built-in Functions**: 50
- **Data Types**: 12 (integers, floats, bool, str)
- **Operators**: 20+ (arithmetic, logical, bitwise)
- **Keywords**: 15+ (fn, pure, let, if, while, etc.)
- **Examples**: 23 (all tested and working)

### Tooling Stats

- **VS Code Snippets**: 30+
- **Syntax Highlighting**: All language constructs
- **Documentation Pages**: 10+
- **Build Time**: ~30 seconds
- **Setup Time**: ~5 minutes total

### Performance Stats

- **Compiler**: LLVM 21.1.4
- **Output**: Native x86-64 code
- **Optimization**: Full LLVM optimizations
- **Speed**: C-level performance

---

##  What's Next?

### Immediate (You)

1.  Run `.\build.ps1`
2.  Open a `.hash` file
3.  Try the snippets (`main<Tab>`, `fn<Tab>`)
4.  Compile and run `examples/00_hello.hash`
5.  Write your first Hash program

### Short Term (Next Features)

- [ ] Hover documentation (see function signatures on hover)
- [ ] Go to definition (jump to functions)
- [ ] Symbol search (find all functions)
- [ ] Real-time diagnostics (errors as you type)
- [ ] Auto-formatter (format Hash code)

### Long Term (Future)

- [ ] Language Server Protocol (LSP)
- [ ] Debugger integration
- [ ] Package manager
- [ ] Standard library expansion
- [ ] Cross-platform support (Linux, macOS)

---

## Contributing

Want to help make Hash better?

1. **Report bugs**: Issues in the repo
2. **Suggest features**: We're listening!
3. **Write examples**: Share your Hash code
4. **Improve docs**: Help others learn
5. **Share Hash**: Tell other developers!

---

## Resources

### Documentation
- `README.md` - Project overview
- `docs/QUICK_REFERENCE.md` - One-page reference
- `docs/BUILTINS.md` - Function reference
- `docs/CODE_COMPLETION.md` - Snippet guide
- `docs/SYNTAX_COLORS.md` - Color guide
- `CHANGELOG.md` - What's new

### Examples
- `examples/` - 23 working examples
- `examples/syntax_showcase.hash` - See all colors
- `examples/23_error_handling.hash` - Error patterns
- `test_examples.ps1` - Run all examples

### VS Code Extension
- `.vscode/extensions/hash-lang/` - Extension source
- `.vscode/extensions/hash-lang/README.md` - Extension docs
- `.vscode/extensions/hash-lang/snippets/hash.json` - All snippets

---

##  Why Hash?

### The Problem

Most languages force you to choose:
-  **Easy to write** (Python) but slow
-  **Fast** (C/C++) but complex syntax
- **Safe** (Rust) but steep learning curve

### The Hash Solution

-  **Easy to write** - Python-like syntax
-  **Fast** - LLVM compilation to native code
-  **Safe** - Optional purity system
-  **Professional** - Full IDE support

### Perfect For

- Systems programming beginners
- Python developers wanting performance
- C developers wanting better syntax
- Functional programming enthusiasts
- Anyone who values clean code

---

##  Learning Path

### Day 1: Basics
1. Run `.\build.ps1`
2. Open `examples/00_hello.hash`
3. Try snippets: `main<Tab>`, `fn<Tab>`, `let<Tab>`
4. Compile and run a simple program

### Day 2: Core Features
5. Read `docs/QUICK_REFERENCE.md`
6. Run examples 01-05
7. Write a calculator program
8. Experiment with functions

### Day 3: Advanced
9. Learn pure functions (`examples/04_pure_functions.hash`)
10. Try file I/O (`fread<Tab>`, `fwrite<Tab>`)
11. Study error handling (`examples/23_error_handling.hash`)
12. Write a file processor

### Week 1: Mastery
13. Complete all 23 examples
14. Write a real program
15. Share your code

---

##  Ready to Start?

```powershell
# Build everything
.\build.ps1

# Restart VS Code

# Create hello.hash and type:
main<Tab>

# Watch the magic happen
```

---

**Hash Language** - Python-like syntax, C-level performance, pure functional power.

*Welcome to the Hash community.*

---

Questions? Check the documentation or explore the examples.

Want to contribute? See CONTRIBUTING.md for guidelines.
