# Hash Programming Language

**Version 1.0.0** | [Release Notes](RELEASE_NOTES.md) | [Documentation](docs/README.md)

A modern, statically typed compiled programming language with Python-like syntax and C-level performance, featuring a pure function system and professional IDE support.

## Installation & Quick Start

### Option 1: System-Wide Install (Recommended)

```powershell
# 1. Build the compiler
.\build.ps1

# 2. Install globally
.\install.ps1

# 3. (Optional) Run interactive setup wizard
hash --setup

# 4. Restart your terminal

# 5. Use 'hash' from anywhere
hash --version
hash -cr C:\HashLang\examples\00_hello.hash
```

After installation, the `hash` command works from any directory.

The `hash --setup` wizard helps you:
- Create a workspace folder
- Copy starter examples
- Configure VS Code
- Run a quick tutorial

See [INSTALLATION.md](docs/INSTALLATION.md) for detailed installation guide.

### Option 2: Development Mode

```powershell
# Build and use locally (no installation)
.\build.ps1
.\hash.ps1 -cr examples/00_hello.hash
```

### Using Hash

Once installed, use Hash from anywhere:

```powershell
# Compile and run in one command
hash -cr myprogram.hash

# Or step by step
hash -c myprogram.hash           # Compile
hash -r myprogram.exe            # Run
hash --version                   # Show version
hash --help                      # Show help
```

## Features

- **Python-like Syntax**: Easy to read and write with significant indentation
- **C-Level Performance**: Compiles to native code via LLVM 21.1.4
- **Pure Functions**: Mark functions as `pure` to enforce no side effects
- **VS Code Extension**: Custom syntax highlighting with distinctive `pure` and `pure_local` keywords
- **IntelliSense**: 30+ code snippets for rapid development
- **50 Built-in Functions**: Console I/O, math, strings, file I/O, and more
- **Modern Error Messages**: Clear, helpful error messages
- **Professional CLI**: Command-line interface with colored output

## Syntax Examples

### Hello World
```hash
fn main() -> i32:
    print_i32(42)
    print_f64(3.14159)
    print_bool(true)

    # Type conversions
    let x: i32 = 10
    let y: f64 = float(x)
    print_f64(y)

    # Math functions
    print_i32(abs(-25))    # 25
    print_i32(min(10, 20)) # 10
    print_f64(sqrt(16.0))  # 4.0

    return 0
```

### Pure Functions
```hash
# Pure function - cannot have side effects
pure fn calculate_sum(a: i32, b: i32) -> i32:
    return a + b

# Pure-local variable - only accessible by pure functions
let pure_local counter: i32 = 0

pure fn get_counter() -> i32:
    return counter
```

### Control Flow
```hash
fn fibonacci(n: i32) -> i32:
    if n <= 1:
        return n
    else:
        return fibonacci(n - 1) + fibonacci(n - 2)

fn loop_example(max: i32) -> i32:
    let mut i: i32 = 0
    while i < max:
        i = i + 1
    return i
```

## Type System

Hash supports multiple numeric types and boolean types:

- **Signed integers**: `i8`, `i16`, `i32`, `i64`
- **Unsigned integers**: `u8`, `u16`, `u32`, `u64`
- **Floating point**: `f32`, `f64`
- **Boolean**: `bool`
- **String**: `str`
- **Void**: `void` (for functions with no return value)

## Built-in Functions

Hash includes 50 built-in functions across multiple categories:

**Console I/O (6 functions):**
- `print_i32()`, `print_i64()`, `print_f64()`, `print_bool()`, `print_str()`, `println()`

**User Input (2 functions):**
- `read_i32() -> i32` - Read integer from console
- `read_f64() -> f64` - Read float from console

**Type Conversions (8 functions):**
- Type system: `int()`, `float()`
- Explicit: `i32_to_i64()`, `i64_to_i32()`, `i32_to_f64()`, `f64_to_i32()`, `i64_to_f64()`, `f64_to_i64()`

**Basic Math (9 functions):**
- Core: `abs()`, `min()`, `max()`, `sqrt()`
- Advanced: `pow()`, `floor()`, `ceil()`, `round()`

**Trigonometry (6 functions):**
- `sin()`, `cos()`, `tan()`, `asin()`, `acos()`, `atan()`

**Logarithms & Exponentials (4 functions):**
- `exp()`, `log()`, `log2()`, `log10()`

**System Functions (6 functions):**
- `exit(i32)` - Exit program with status code
- `hash_time() -> i64` - Get Unix timestamp
- `hash_clock() -> f64` - Get execution time in seconds
- `random() -> f64` - Random float (0.0 to 1.0)
- `seed_random(i32)` - Set random seed
- `random_range(i32, i32) -> i32` - Random integer in range

**String Manipulation (5 functions):**
- `len(str) -> i32` - Get string length
- `str_concat(str, str) -> str` - Concatenate strings
- `str_eq(str, str) -> bool` - String equality
- `upper(str) -> str` - Convert to uppercase
- `lower(str) -> str` - Convert to lowercase

**File I/O (4 functions):**
- `file_read(str) -> str` - Read entire file as string
- `file_write(str, str) -> bool` - Write string to file
- `file_exists(str) -> bool` - Check if file exists
- `file_delete(str) -> bool` - Delete a file

See [`docs/BUILTINS.md`](docs/BUILTINS.md) for detailed documentation and examples.

## VS Code Syntax Highlighting

Custom VS Code extension with syntax highlighting for Hash's unique features.

```powershell
# Install the custom VS Code extension
.\install_syntax.ps1
```

Then restart VS Code. Features:
- `pure` keyword: Distinctive pink highlighting
- `pure_local` keyword: Orange highlighting
- All 50 built-in functions recognized
- Custom "Hash Vibrant" color theme
- Auto-indentation and bracket pairing

See [`.vscode/extensions/hash-lang/README.md`](.vscode/extensions/hash-lang/README.md) for details.

## Building from Source

### Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler (MSVC, GCC, or Clang)
- LLVM 14.0 or higher installed

### Build Steps

```powershell
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build . --config Release

# Install (optional)
cmake --install .
```

## Usage

### Compile a Hash program

```powershell
# Compile to executable
.\hashc program.hash

# Specify output name
.\hashc -o myprogram program.hash

# Emit LLVM IR
.\hashc --emit-llvm program.hash

# Save LLVM IR to file
.\hashc --emit-ir program.hash
```

### Compiler Options

- `-o <output>` - Specify output file name
- `--emit-llvm` - Emit LLVM IR instead of object file
- `--emit-ir` - Save LLVM IR to .ll file
- `--tokens` - Print tokens and exit (debugging)
- `--ast` - Print AST and exit (debugging)
- `-h, --help` - Show help message

## Language Features

### Behavior-Aware Access Control

Hash introduces a novel approach to scope and access control based on function behavior:

**Pure Functions**: Functions marked with `pure` are guaranteed to have no side effects. They cannot:
- Modify global state
- Call non-pure functions
- Modify their parameters
- Perform I/O operations

**Pure-Local Variables**: Variables marked with `pure_local` can only be read or modified by pure functions. This prevents "spooky action at a distance" and makes data flow explicit.

```hash
let pure_local safe_data: i32 = 42

pure fn read_safe_data() -> i32:
    return safe_data  # OK - pure function

fn modify_data() -> void:
    safe_data = 100   # ERROR - non-pure function cannot access pure_local
```

### Benefits

1. **Predictability**: Pure functions always produce the same output for the same input
2. **Parallelization**: Pure functions can be safely executed in parallel
3. **Testing**: Pure functions are easier to test and reason about
4. **Debugging**: Behavior-aware access control prevents unexpected side effects

## Examples

The `examples/` directory contains 23 comprehensive sample programs:

**Featured Examples:**
- `examples/23_error_handling.hash` - Comprehensive error handling for all features
- `examples/22_file_io.hash` - File I/O operations (read, write, exists, delete)
- `examples/21_text_processor.hash` - Advanced text processing patterns
- `examples/20_strings.hash` - String manipulation functions
- `examples/19_guessing_game.hash` - Interactive number guessing game
- `examples/08_patterns.hash` - Advanced programming patterns (400+ lines)
- `examples/05_algorithms.hash` - Classic algorithms (330+ lines)

**Core Learning:**
- `examples/00_hello.hash` - Hello World
- `examples/01_basics.hash` - Language fundamentals
- `examples/02_control_flow.hash` - If/else and while loops
- `examples/03_functions.hash` - Function patterns
- `examples/04_pure_functions.hash` - Pure functions and pure_local
- `examples/13_edge_cases.hash` - Edge cases for all features

See [`examples/INDEX.md`](examples/INDEX.md) for the complete catalog of all 23 examples.

## Documentation

### Quick Start Guides
- [COMPLETE_GUIDE.md](docs/COMPLETE_GUIDE.md) - Comprehensive guide for Hash
- [INSTALLATION.md](docs/INSTALLATION.md) - Detailed installation guide with troubleshooting
- [POST_INSTALL_CHECKLIST.md](docs/POST_INSTALL_CHECKLIST.md) - Verify your installation
- [GETTING_STARTED.md](docs/GETTING_STARTED.md) - Language basics and first program

### CLI & IDE
- [CLI_GUIDE.md](docs/CLI_GUIDE.md) - Complete command-line reference
- [CODE_COMPLETION.md](docs/CODE_COMPLETION.md) - IntelliSense guide
- [SYNTAX_COLORS.md](docs/SYNTAX_COLORS.md) - Syntax highlighting reference

### Language Reference
- [QUICK_REFERENCE.md](docs/QUICK_REFERENCE.md) - One-page cheat sheet
- [PURE_FUNCTIONS.md](docs/PURE_FUNCTIONS.md) - Pure function system deep dive

### Examples Index
- [examples/INDEX.md](examples/INDEX.md) - Complete catalog of 23 examples

## Roadmap

- [x] Lexer and Parser
- [x] Type system
- [x] Semantic analyzer with pure function checking
- [x] LLVM code generation
- [ ] Standard library
- [ ] Module system
- [ ] Structs and enums
- [ ] Pattern matching
- [ ] Self-hosting compiler
- [ ] Advanced optimizations
- [ ] Package manager

## Community

Join our Discord server to discuss Hash, get help, and share your projects:

**[Join Hash Discord](https://discord.gg/wFJ2fqC6xt)**

## Contributing

Hash is in early development. Contributions, bug reports, and feature requests are welcome.

## License

MIT License - See LICENSE file for details

## Author

Created with the goal of combining low-level control with high-level expressiveness, while introducing novel behavior-aware programming paradigms.
