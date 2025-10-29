# Hash Language - Changelog

All notable changes to the Hash language and tooling.

## [1.0.0] - 2025-10-29

### Official Release
First stable release of Hash Language.

### Added - VS Code Extension & IntelliSense

####  VS Code Syntax Highlighting
- **Custom TextMate grammar** for Hash language
- **Vibrant color theme** optimized for Hash code
  - `pure` keyword: bright pink (#ff6b9d) - Bold
  - `pure_local` keyword: vibrant orange (#ff9d00) - Bold
  - Built-in functions: Warm orange (#ffa657)
  - Control flow: Bright red (#ff7b72)
  - Types: Cyan (#79c0ff)
  - Strings: Light blue (#a5d6ff)
  - Comments: Gray italic
- All 50 built-in functions properly recognized
- Custom "Hash Vibrant Theme" for optimal viewing

####  IntelliSense & Code Completion
- **30+ intelligent code snippets** with tab stops
- **Function templates**: `fn`, `pure`, `main`
- **Variable declarations**: `let`, `mut`, `pure_local`
- **Control flow**: `if`, `ife`, `while`, `for`
- **I/O operations**: `prints`, `printi`, `printf`, `readi`, `readf`
- **File operations**: `fread` (safe), `fwrite` (safe)
- **String operations**: `len`, `concat`, `streq`
- **Math functions**: `math` (dropdown selector)
- **Random numbers**: `rand` (with auto-seed)
- **Utilities**: `template`, `hdr`, `safe`
- Dropdowns for common choices
- Best practices built into snippets

####  Smart Editor Features
- Auto-closing brackets: `()`, `[]`, `{}`, `""`
- Auto-indentation after `:`, `fn`, `if`, `while`, `else`
- Comment toggling with `Ctrl+/`
- Bracket pair highlighting
- Auto-surrounding selected text

#### Build Integration
- **One-command setup**: `.\build.ps1` now:
  1. Builds Hash compiler
  2. Installs VS Code extension
  3. Sets up IntelliSense
  4. Ready to code
- Automatic extension installation to `%USERPROFILE%\.vscode\extensions`
- Removes old versions automatically
- Feature summary displayed after install

####  Documentation
- **CODE_COMPLETION.md**: Complete IntelliSense guide
- **SYNTAX_COLORS.md**: Color reference and examples
- **Extension README**: Comprehensive extension documentation
- **syntax_showcase.hash**: Visual demonstration of all colors

## [0.9.0] - 2025-10-15

### Added - Professional CLI & Build Output
- Rust-inspired build output with progress bars
- Build phase timing and detailed breakdowns
- Enhanced CLI with colored output
- Setup wizard for first-run experience

## [0.8.0] - 2025-09-28

### Added - File I/O (Previous Update)

#### File I/O Functions (4 functions)
- `file_read(filename: str) -> str` - Read entire file
- `file_write(filename: str, content: str) -> bool` - Write to file
- `file_exists(filename: str) -> bool` - Check if file exists
- `file_delete(filename: str) -> bool` - Delete file

####  Examples
- **23_error_handling.hash** (NEW, 450+ lines)
  - Comprehensive error handling patterns
  - Math safety, string safety, file I/O safety
  - Input validation, error recovery
  - Real-world examples
- **13_edge_cases.hash** (ENHANCED, 350+ lines)
  - String edge cases
  - File I/O edge cases
  - Robust error handling

## [0.7.0] - 2025-09-10

### Added - String Functions (Previous Update)

#### String Manipulation (5 functions, Python-style)
- `len(s: str) -> i32` - String length
- `str_concat(s1: str, s2: str) -> str` - Concatenate strings
- `str_eq(s1: str, s2: str) -> bool` - String equality
- `upper(s: str) -> str` - Convert to uppercase
- `lower(s: str) -> str` - Convert to lowercase

####  Examples
- Enhanced existing examples with string operations
- String comparison patterns
- Case conversion usage

### Updated - Documentation (Previous Updates)

####  Complete Documentation Refresh
- **BUILTINS.md**: Updated with all 50 functions
- **LANGUAGE.md**: Updated with string and file I/O
- **EXAMPLES.md**: Now covers all 23 examples
- **README.md**: Updated feature list and quick start
- **QUICK_REFERENCE.md**: Added new functions

####  Current State
- **Total Built-in Functions**: 50
  - Console I/O: 6 functions
  - User Input: 2 functions
  - Type Conversions: 8 functions
  - Math: 9 functions
  - Trigonometry: 6 functions
  - Logarithms: 4 functions
  - System: 6 functions
  - Strings: 5 functions
  - File I/O: 4 functions
- **Total Examples**: 23 (all compile and run)

---

## [0.5.0] - 2025-08-05

### Core Language Features

#### Syntax
- Python-like indentation-based syntax
- Static typing with type inference
- Function declarations with `fn` keyword
- Pure functions with `pure` keyword
- Pure-local variables with `pure_local` keyword

#### Data Types
- **Integers**: `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`
- **Floats**: `f32`, `f64`
- **Other**: `bool`, `str`, `void`

####  Operators
- **Arithmetic**: `+`, `-`, `*`, `/`, `%`, unary `-`
- **Comparison**: `==`, `!=`, `<`, `<=`, `>`, `>=`
- **Logical**: `&&`, `||`, `!`
- **Bitwise**: `&`, `|`, `^`, `<<`, `>>`

#### Control Flow
- `if`/`else` statements
- `while` loops
- `return`, `break`, `continue`

#### Functions
- Standard functions with `fn`
- Pure functions with `pure` (no side effects)
- Return type declarations
- Parameter type declarations

####  Pure System
- Pure functions cannot:
  - Call non-pure functions
  - Access global mutable state
  - Perform I/O operations
- Pure-local variables:
  - Accessible only from pure functions
  - Enforces functional programming patterns

### Compiler Features

#### 🛠️ LLVM Backend
- LLVM 21.1.4 integration
- Optimized code generation
- Cross-platform support (Windows primary)

####  Error Handling
- Detailed error messages with line/column info
- Type checking
- Purity enforcement
- Helpful suggestions

#### Output Options
- Native executable generation
- LLVM IR emission (`--emit-llvm`)
- Custom output filename (`-o`)

### Built-in Functions (Initial Set)

#### Console I/O (6)
- `print_i32`, `print_f64`, `print_bool`, `print_str`, `print_char`, `println`

#### User Input (2)
- `read_i32`, `read_f64`

#### Type Conversions (8)
- `i32_to_f64`, `f64_to_i32`, `i32_to_str`, `f64_to_str`, `bool_to_str`
- `str_to_i32`, `str_to_f64`, `char_to_i32`

#### Math (9)
- `abs`, `abs_f`, `floor`, `ceil`, `round`
- `sqrt`, `pow`, `min`, `max`

#### Trigonometry (6)
- `sin`, `cos`, `tan`, `asin`, `acos`, `atan`

#### Logarithms (4)
- `log`, `log10`, `log2`, `exp`

#### System (6)
- `hash_time`, `hash_sleep`, `random`, `random_range`, `seed_random`, `hash_exit`

### Tooling

#### Build System
- PowerShell build script (`build.ps1`)
- CMake integration
- Release and Debug builds
- Dependency checking

#### Testing
- Example test script (`test_examples.ps1`)
- 10 initial examples covering core features
- All examples compile and run

####  Documentation
- Comprehensive language guide
- Built-in function reference
- Quick reference card
- Example documentation

### Initial Examples

1. **00_hello.hash** - Hello world
2. **01_basics.hash** - Variables and types
3. **02_control_flow.hash** - If/else, loops
4. **03_functions.hash** - Function patterns
5. **04_pure_functions.hash** - Pure functions
6. **05_algorithms.hash** - Algorithms
7. **06_types.hash** - Type system
8. **07_recursion.hash** - Recursion
9. **08_patterns.hash** - Advanced patterns
10. **09_builtins.hash** - Built-in functions

---

## Feature Timeline

### Phase 1: Core Language
- [x] Lexer and Parser
- [x] Type system
- [x] LLVM code generation
- [x] Basic built-in functions
- [x] Pure function system

### Phase 2: Enhanced Functions
- [x] String manipulation
- [x] File I/O
- [x] Error handling examples
- [x] Comprehensive documentation

### Phase 3: Developer Experience
- [x] VS Code syntax highlighting
- [x] Custom color theme
- [x] IntelliSense and snippets
- [x] Integrated build system
- [x] One-command setup

### Phase 4: Future Enhancements
- [ ] Hover documentation
- [ ] Go to definition
- [ ] Symbol search
- [ ] Real-time diagnostics
- [ ] Auto-formatter
- [ ] Language server protocol (LSP)
- [ ] More examples
- [ ] Standard library expansion

---

## Version Numbering

Hash follows [Semantic Versioning](https://semver.org/):
- **Major**: Breaking language changes
- **Minor**: New features, backwards compatible
- **Patch**: Bug fixes

Current: **1.0.0** (with unreleased enhancements)

---

## Platform Support

### Tested Platforms
-  Windows 10/11 (Primary)
- Linux (Planned)
- macOS (Planned)

### Requirements
- CMake 3.15+
- LLVM 21.1.4
- C++17 compiler
- VS Code (for extension)

---

## Migration Guide

### From 1.0.0 to Current

#### New Features Available
1. **String Functions**: Use `len()`, `upper()`, `lower()`, etc.
2. **File I/O**: Use `file_read()`, `file_write()`, `file_exists()`, `file_delete()`
3. **VS Code Extension**: Run `.\build.ps1` to install
4. **IntelliSense**: Use code snippets for rapid development

#### No Breaking Changes
All existing Hash 1.0.0 code remains compatible.

---

## Contributing

See the main README.md for contribution guidelines.

---

## Acknowledgments

- **LLVM Project**: For the compiler infrastructure
- **Python**: For syntax inspiration
- **Rust**: For the purity system inspiration
- **VS Code**: For the excellent extension API

---

*Last Updated: October 29, 2025*
