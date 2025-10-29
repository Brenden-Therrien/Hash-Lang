# What's Next - Your Hash Journey

Hash is installed and ready to use.

You've successfully:
- Built the Hash compiler
- Installed Hash system-wide
- Added `hash` to your PATH
- Installed VS Code extension
- Verified your installation

If you haven't already, run the Interactive Setup Wizard:
```powershell
hash --setup
```
It will help you:
- Create a workspace folder
- Copy starter examples
- Configure VS Code
- Run a quick tutorial

## Choose Your Learning Path

Pick the learning path that fits your style:

### Quick Start (15 minutes)

For developers who learn by doing.

Steps:
1. Try examples:
   ```powershell
   hash -cr C:\HashLang\examples\00_hello.hash
   hash -cr C:\HashLang\examples\01_basics.hash
   hash -cr C:\HashLang\examples\04_pure_functions.hash
   ```

2. Create your first program:
   ```powershell
   code myprogram.hash
   # Type: main<Tab> to get template
   # Write some code
   hash -cr myprogram.hash
   ```

3. Check the [QUICK_START_CARD.md](docs/QUICK_START_CARD.md) for syntax reference

Next: Jump to [Build Your First Project](#build-your-first-project)

### Thorough Learning (2 hours)

For developers who want solid fundamentals.

Steps:
1. Read: [GETTING_STARTED.md](docs/GETTING_STARTED.md)
   - Language basics
   - Type system
   - Control flow
   - Functions

2. Try all basic examples (00-03):
   ```powershell
   hash -cr C:\HashLang\examples\00_hello.hash
   hash -cr C:\HashLang\examples\01_basics.hash
   hash -cr C:\HashLang\examples\02_control_flow.hash
   hash -cr C:\HashLang\examples\03_functions.hash
   ```

3. Study the [QUICK_REFERENCE.md](docs/QUICK_REFERENCE.md)
   - Print it out!
   - Keep it on your desk

4. Practice with intermediate examples (04-10)

Next: Move to [Master Advanced Features](#master-advanced-features)

### Deep Dive (1 week)

For developers who want to master Hash.

**Day 1-2: Foundations**
- Read all documentation in [docs/](docs/)
- Complete all 23 examples
- Understand every built-in function

**Day 3-4: Pure Functions**
- Deep dive: [PURE_FUNCTIONS.md](docs/PURE_FUNCTIONS.md)
- Study: Example 04 (pure functions)
- Experiment: Create pure function examples

**Day 5-6: Advanced Features**
- String manipulation (Examples 16-20)
- File I/O (Examples 21-22)
- Error handling (Example 23)

**Day 7: Build**
- Create a real project
- Use everything you learned

Next: See [Project Ideas](#project-ideas)

## Build Your First Project

Ready to build something real? Here are some ideas:

### Beginner Projects

#### 1. Calculator
```hash
# Simple calculator with +, -, *, /
# Use: read_line() for input
# Use: str_to_f64() for conversion
```

**Skills**: User input, type conversion, math operations

#### 2. Number Guessing Game
```hash
# Computer picks random number
# Player guesses until correct
# Track number of attempts
```

**Skills**: Random numbers, loops, conditionals
**Example**: Check `examples/19_guessing_game.hash`

#### 3. Temperature Converter
```hash
# Celsius ↔ Fahrenheit
# Fahrenheit ↔ Kelvin
# Clean formatted output
```

**Skills**: Pure functions, conversions, formatting

---

### Intermediate Projects

#### 4. File-Based Todo List
```hash
# Add/remove/list todos
# Save to file
# Load from file
```

**Skills**: File I/O, string manipulation, arrays
**Hint**: Use `file_read()` and `file_write()`

#### 5. Text Statistics
```hash
# Count words, characters, lines
# Find longest word
# Calculate average word length
```

**Skills**: String functions, file reading, math
**Example**: See `examples/21_text_processor.hash`

#### 6. Simple Encryption
```hash
# Caesar cipher
# Encrypt/decrypt messages
# Save encrypted files
```

**Skills**: String manipulation, pure functions, file I/O

---

### Advanced Projects

#### 7. CSV Data Analyzer
```hash
# Read CSV files
# Calculate statistics (min, max, average)
# Generate reports
```

**Skills**: File I/O, parsing, math operations, arrays

#### 8. Log File Parser
```hash
# Parse log files
# Extract patterns
# Generate summary reports
```

**Skills**: String functions, file reading, pattern matching

#### 9. Math Expression Evaluator
```hash
# Parse expressions: "2 + 3 * 4"
# Follow order of operations
# Support parentheses
```

**Skills**: Pure functions, recursion, parsing

---

## Master Advanced Features

### Pure Functions Mastery

**Why pure functions matter:**
- Guaranteed no side effects
- Easy to test
- Safe to parallelize (future feature!)
- Clear data flow

**Challenge:** Rewrite your projects using pure functions wherever possible!

**Study:**
1. Read: [PURE_FUNCTIONS.md](docs/PURE_FUNCTIONS.md)
2. Run: `hash -cr C:\HashLang\examples\04_pure_functions.hash`
3. Experiment: Mark functions as `pure` in your code
4. Learn: What triggers compiler errors?

**Practice:**
```hash
# Convert this:
fn calculate(x: i32) -> i32:
    print_i32(x)  # Side effect
    return x * 2

# To this:
pure fn calculate(x: i32) -> i32:
    return x * 2  # Pure

fn display_result(x: i32) -> void:
    print_i32(calculate(x))  # Side effects here
```

---

### String Manipulation

**Essential for text processing!**

**Learn:**
1. Run all string examples:
   ```powershell
   hash -cr C:\HashLang\examples\16_string_basics.hash
   hash -cr C:\HashLang\examples\17_string_advanced.hash
   hash -cr C:\HashLang\examples\18_string_validation.hash
   hash -cr C:\HashLang\examples\20_strings.hash
   ```

2. Build a text tool:
   - Word counter
   - Text formatter
   - String validator

**Functions to master:**
- `len()` - String length
- `str_concat()` - Combine strings
- `str_eq()` - Compare strings
- `upper()` / `lower()` - Case conversion

---

### File I/O

**Build programs that persist data!**

**Learn:**
1. Study: `examples/22_file_io.hash`
2. Build: Todo list with file storage
3. Create: Configuration file reader

**Functions:**
```hash
file_write("data.txt", "content")  # Write
let text: str = file_read("data.txt")  # Read
let exists: bool = file_exists("data.txt")  # Check
file_delete("data.txt")  # Remove
```

**Project ideas:**
- Note-taking app
- Contact manager
- Game save system

---

## Best Practices

### 1. Use Pure Functions

```hash
# Good
pure fn calculate_tax(price: f64, rate: f64) -> f64:
    return price * rate

fn show_total(price: f64) -> void:
    let tax: f64 = calculate_tax(price, 0.08)
    print_f64(price + tax)
```

### 2. Descriptive Names

```hash
# Bad
fn f(x: i32) -> i32:
    return x * 2

# Good
pure fn double_value(number: i32) -> i32:
    return number * 2
```

### 3. Consistent Indentation

```hash
# Always 4 spaces
fn main() -> i32:
    let x: i32 = 10
    if x > 5:
        print_str("Big")
    else:
        print_str("Small")
    return 0
```

### 4. Error Handling

```hash
# Check before use
let exists: bool = file_exists("config.txt")
if exists:
    let config: str = file_read("config.txt")
else:
    print_str("Config not found!")
```

### 5. Use VS Code

- IntelliSense saves time!
- Syntax colors catch errors
- Snippets boost productivity

**Tip:** Type snippet name + Tab:
- `main` → Main function
- `fn` → Function template
- `pure` → Pure function

---

## Workflow Tips

### Quick Iteration

```powershell
# 1. Edit in VS Code
code myprogram.hash

# 2. Save (Ctrl+S)

# 3. Run immediately
hash -cr myprogram.hash

# 4. See results instantly
```

### Debugging

```powershell
# Use print statements
print_str("Debug: x = ")
print_i32(x)
println()

# Check intermediate values
let result: i32 = calculate(x)
print_str("Result: ")
print_i32(result)
```

### Project Organization

```
MyProject/
├── src/
│   ├── main.hash
│   ├── utils.hash
│   └── data.hash
├── tests/
│   └── test_utils.hash
└── README.md
```

---

## Reference Materials

### Keep These Handy

1. **[QUICK_REFERENCE.md](docs/QUICK_REFERENCE.md)**
   - Print it out!
   - All 50 functions
   - Syntax at a glance

2. **[QUICK_START_CARD.md](docs/QUICK_START_CARD.md)**
   - Desk reference
   - Essential commands
   - Common patterns

3. **[CLI_GUIDE.md](docs/CLI_GUIDE.md)**
   - All CLI options
   - Workflow examples
   - Pro tips

### When You Get Stuck

1. **Check examples** in `C:\HashLang\examples\`
2. **Read docs** in `C:\HashLang\docs\`
3. **Use** `hash --help`
4. **Review** error messages (they're helpful!)

---

## Learning Checklist

Track your progress:

### Week 1: Basics
- [ ] Installed Hash successfully
- [ ] Ran all basic examples (00-03)
- [ ] Created first program
- [ ] Used VS Code with IntelliSense
- [ ] Read GETTING_STARTED.md
- [ ] Printed QUICK_REFERENCE.md

### Week 2: Core Features
- [ ] Mastered all 50 built-in functions
- [ ] Understand pure functions
- [ ] Used pure_local variables
- [ ] Built calculator or game
- [ ] Completed all 23 examples
- [ ] Read CLI_GUIDE.md

### Week 3: Advanced
- [ ] String manipulation mastery
- [ ] File I/O operations
- [ ] Error handling patterns
- [ ] Built real project
- [ ] Read PURE_FUNCTIONS.md
- [ ] Optimized code with pure functions

### Week 4: Mastery
- [ ] Built 3+ projects
- [ ] Used all function categories
- [ ] Understand LLVM IR (--emit-llvm)
- [ ] Can teach Hash to others
- [ ] Contributed example or doc
- [ ] Shared your project!

---

## Project Ideas by Category

### Console Applications
-  Calculator (math functions)
-  Quiz game (user input, logic)
-  Dice roller (random numbers)
-  Unit converter (conversions)

### Text Processing
-  Word counter
-  Text formatter
-  Log analyzer
-  String validator

### File-Based Tools
-  Todo list
-  Note taker
-  Contact manager
-  Config reader

### Games
-  Number guessing
-  Hangman
-  Adventure game
-  Quiz game

### Utilities
-  Time tracker
-  Password generator
-  CSV parser
-  Data validator

---

## Share Your Work

Built something with Hash? Consider sharing it through GitHub repos, blog posts, or tutorials.

## Summary

You now have everything you need to use Hash:

- Installed: Global `hash` command
- Documented: Complete guides and references
- Examples: 23 working programs
- Tools: VS Code with IntelliSense
- Support: Complete reference materials

## First Steps

```powershell
# Create your project
mkdir MyHashProject
cd MyHashProject

# Start coding
code main.hash

# Type: main<Tab> to get started
hash -cr main.hash
```
