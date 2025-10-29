# Hash Language v1.0.0 - Release Notes

> **First Official Release** - A complete, professional programming language!

---

```


HASH LANGUAGE v1.0.0
FIRST OFFICIAL RELEASE

Python-like Syntax • C-Level Performance • LLVM


```

**Release Date**: October 29, 2025
**LLVM Version**: 21.1.4
**Platform**: Windows 10/11 (x64)

---

## What is Hash?

Hash is a modern, statically typed programming language that combines:

- **Python-like Syntax** - Easy to read and write
- **C-Level Performance** - Compiles to native code via LLVM
- **Pure Functions** - Enforced side-effect free programming
- **Professional Tools** - CLI, VS Code extension, IntelliSense

---

## Key Features

### Language Core

- ✅ **50 Built-in Functions** across 8 categories
- ✅ **Pure Function System** with compile-time enforcement
- ✅ **Behavior-Aware Access Control** (pure_local variables)
- ✅ **Strong Type System** (i32, i64, f64, bool, str, arrays)
- ✅ **Python-like Indentation** (4 spaces, significant whitespace)
- ✅ **LLVM Backend** (optimized native code generation)

### Standard Library

**Console I/O**: 6 functions
- print_i32, print_i64, print_f64, print_bool, print_str, println

**User Input**: 2 functions
- read_int, read_line

**Type Conversions**: 8 functions
- float, int, bool_to_str, str_to_int, str_to_f64, int_to_str, f64_to_str, str_to_bool

**Math Operations**: 9 functions
- abs, min, max, pow, mod, clamp, sign, gcd, lcm

**Trigonometry**: 6 functions
- sin, cos, tan, asin, acos, atan

**Logarithms**: 4 functions
- sqrt, log, log10, exp

**System**: 6 functions
- exit, sleep, time, rand, rand_range, set_seed

**Strings**: 5 functions
- len, str_concat, str_eq, upper, lower

**File I/O**: 4 functions
- file_read, file_write, file_exists, file_delete

### Professional CLI

- ✅ **Beautiful Command-Line Interface** with colored output
- ✅ **hash -cr** - Compile and run in one command
- ✅ **Smart Path Handling** - Works from any directory
- ✅ **Formatted Output** - Boxed program output with borders
- ✅ **Exit Code Reporting** - Clear success/failure indicators
- ✅ **Global Command** - `hash` accessible system-wide

**Commands:**
```powershell
hash -c <file>         # Compile
hash -r <file>         # Run
hash -cr <file>        # Compile and run
hash --version         # Show version
hash --help            # Show help
```

**Advanced Options:**
```powershell
-o <name>              # Custom output name
--emit-llvm            # Generate LLVM IR
-v                     # Verbose output
```

### VS Code Extension

- ✅ **Syntax Highlighting** with vibrant colors
- ✅ **Custom Theme** - PINK `pure`, ORANGE `pure_local`
- ✅ **30+ IntelliSense Snippets** (main, fn, pure, if, while, etc.)
- ✅ **Auto-Indentation** - Smart 4-space indentation
- ✅ **Bracket Pairing** - Auto-close brackets and quotes
- ✅ **Comment Toggling** - Ctrl+/ for comments

### System Installation

- ✅ **One-Command Install** - `.\install.ps1`
- ✅ **Global Access** - `hash` command from anywhere
- ✅ **PATH Integration** - Automatic PATH updates
- ✅ **VS Code Integration** - Auto-install extension
- ✅ **Examples & Docs** - Complete installation
- ✅ **Clean Uninstall** - `.\install.ps1 -Uninstall`

### Documentation

- ✅ **150+ Pages** of comprehensive documentation
- ✅ **14 Guides** covering all aspects
- ✅ **23 Working Examples** with full source code
- ✅ **Complete Function Reference** for all 50 functions
- ✅ **Troubleshooting Guides** for common issues
- ✅ **Quick Reference Cards** for printing

---

## What's Included

### Compiler

- **hashc.exe** - LLVM-based compiler
- **Optimizations**: -O3 by default
- **Output**: Native x64 Windows executables
- **Error Messages**: Clear, helpful diagnostics

### CLI Tools

- **hash.ps1** - PowerShell CLI script
- **hash.cmd** - CMD/PowerShell wrapper
- **Colored Output**: Green ✓, Red ✗, Cyan →
- **Formatted Boxes**: Clean program output

### VS Code Extension

- **hash-lang-1.0.0** extension
- **Syntax Grammar**: Complete tmLanguage.json
- **Custom Theme**: Vibrant color scheme
- **Snippets**: 30+ code templates

### Examples (23 Total)

**Basics:**
- 00_hello.hash - Hello World
- 01_basics.hash - Fundamentals
- 02_control_flow.hash - If/While
- 03_functions.hash - Function patterns

**Core Features:**
- 04_pure_functions.hash - Pure function system
- 05_algorithms.hash - Classic algorithms (330+ lines)
- 06_math_showcase.hash - All math functions

**Advanced:**
- 08_patterns.hash - Programming patterns (400+ lines)
- 13_edge_cases.hash - Edge case handling
- 14_arrays.hash - Array operations
- 15_user_input.hash - Interactive programs

**Strings:**
- 16_string_basics.hash - Basic string ops
- 17_string_advanced.hash - Advanced patterns
- 18_string_validation.hash - Validation
- 19_guessing_game.hash - Interactive game
- 20_strings.hash - All string functions

**File I/O:**
- 21_text_processor.hash - Text processing
- 22_file_io.hash - All file operations

**Error Handling:**
- 23_error_handling.hash - Comprehensive errors (400+ lines)

### Documentation (14 Guides)

**Quick Start:**
1. COMPLETE_GUIDE.md - Everything in one place!
2. INSTALLATION.md - Detailed install guide
3. POST_INSTALL_CHECKLIST.md - 10-step verification
4. GETTING_STARTED.md - Language tutorial

**CLI & Tools:**
5. CLI_GUIDE.md - Command reference
6. CODE_COMPLETION.md - IntelliSense guide
7. SYNTAX_COLORS.md - Highlighting reference

**Language:**
8. QUICK_REFERENCE.md - One-page cheat sheet
9. QUICK_START_CARD.md - Printable reference card
10. PURE_FUNCTIONS.md - Deep dive
11. FUNCTION_REFERENCE.md - All 50 functions

**Development:**
12. DEVELOPMENT_SUMMARY.md - Project history
13. CHANGELOG.md - Version history
14. INSTALLATION_CHECKLIST.md - Build verification

---

## Installation

### Quick Install (5 minutes)

```powershell
# 1. Clone/download Hash
git clone <repository>
cd HashLang

# 2. Build compiler
.\build.ps1

# 3. Install system-wide
.\install.ps1

# 4. Restart terminal

# 5. Verify
hash --version
```

**That's it.** Hash is now installed globally.

### What Gets Installed

- **C:\HashLang\bin\** - Compiler and CLI
- **C:\HashLang\examples\** - 23 example programs
- **C:\HashLang\docs\** - Complete documentation
- **%USERPROFILE%\.vscode\extensions\** - VS Code extension
- **PATH** - C:\HashLang\bin added

---

## Quick Start Guide

### Your First Program

```powershell
# 1. Create a file
code hello.hash

# 2. Write code (in VS Code, type: main<Tab>)
fn main() -> i32:
    print_str("Hello from Hash!")
    println()
    return 0

# 3. Compile and run
hash -cr hello.hash
```

**Output:**
```
→ Compiling: hello.hash
✓ Compiled successfully: hello.exe

→ Running: .\hello.exe
─────────────────────────────────────────────────────
Hello from Hash!

─────────────────────────────────────────────────────
✓ Program completed (exit code: 0)
```

### Try the Examples

```powershell
# Basic example
hash -cr C:\HashLang\examples\00_hello.hash

# Pure functions
hash -cr C:\HashLang\examples\04_pure_functions.hash

# String manipulation
hash -cr C:\HashLang\examples\20_strings.hash

# File I/O
hash -cr C:\HashLang\examples\22_file_io.hash

# Error handling
hash -cr C:\HashLang\examples\23_error_handling.hash
```

---

## Learning Path

### Day 1 - Getting Started

1. Install Hash (see above)
2. Read: `docs/GETTING_STARTED.md`
3. Try: Examples 00-03 (basics)
4. Create: Your first program

### Week 1 - Core Features

1. Master: CLI commands (`docs/CLI_GUIDE.md`)
2. Study: Pure functions (Example 04)
3. Practice: All 23 examples
4. Reference: `docs/QUICK_REFERENCE.md`

### Week 2 - Advanced

1. Deep dive: `docs/PURE_FUNCTIONS.md`
2. Advanced: String & File I/O (Examples 20-22)
3. Error handling: Example 23
4. Build: Real project!

---

## Highlights

### Pure Function System

Hash's killer feature! Enforced at compile-time:

```hash
# Pure functions are guaranteed side-effect free!
pure fn calculate(x: i32, y: i32) -> i32:
    return x * y + 10

# Access control based on behavior
let pure_local safe_data: i32 = 42

pure fn read_safe() -> i32:
    return safe_data  # ✓ OK

fn modify_unsafe() -> void:
    safe_data = 100   # ✗ ERROR - prevented at compile time!
```

**Benefits:**
- ✅ Predictable behavior
- ✅ Safe parallelization
- ✅ Easier testing
- ✅ Clear data flow

### Professional CLI

Beautiful, colored output with clear indicators:

```
═══════════════════════════════════════════════════
     Hash Language - Python-like syntax,
          C-level performance
───────────────────────────────────────────────────

→ Compiling: myprogram.hash

✓ Compiled successfully: myprogram.exe
  Output: myprogram.exe

═══════════════════════════════════════════════════
```

### VS Code Integration

**PINK** pure functions stand out!

```hash
pure fn add(a: i32, b: i32) -> i32:  # ← PINK!
    return a + b

pure_local let data: i32 = 42        # ← ORANGE!
```

Plus 30+ snippets:
- `main` → Full main function template
- `fn` → Function template
- `pure` → Pure function template
- And many more!

---

## By the Numbers

- **50** Built-in functions
- **23** Working examples
- **14** Documentation guides
- **150+** Pages of documentation
- **30+** IntelliSense snippets
- **8** Function categories
- **1** Command to install: `.\install.ps1`
- **0** Dependencies (LLVM included!)

---

## System Requirements

### Minimum Requirements

- **OS**: Windows 10/11 (64-bit)
- **RAM**: 2 GB
- **Disk**: 500 MB free space
- **PowerShell**: 5.1 or later

### Recommended

- **OS**: Windows 11
- **RAM**: 4 GB+
- **Disk**: 1 GB free space
- **Editor**: VS Code (for IntelliSense)

### Dependencies

- **LLVM 21.1.4** - Included with Hash!
- **PowerShell** - Built into Windows
- **VS Code** - Optional (for IDE features)

---

## Known Issues

### v1.0.0

**None!** All major issues resolved.

**Tested:**
- ✅ Installation on fresh Windows 10/11
- ✅ All 23 examples compile and run
- ✅ VS Code extension activates
- ✅ Global `hash` command works
- ✅ PATH integration successful
- ✅ Uninstall clean

---

## Roadmap

### Future Features

**v1.1.0** (Planned)
- [ ] Structs and enums
- [ ] Pattern matching
- [ ] Module system
- [ ] Package manager

**v1.2.0** (Planned)
- [ ] Standard library expansion
- [ ] Linux/macOS support
- [ ] Advanced optimizations
- [ ] Language server protocol

**v2.0.0** (Vision)
- [ ] Self-hosting compiler
- [ ] Native package registry
- [ ] Web assembly target
- [ ] Integrated debugger

---

## Contributing

Hash is open source! Contributions welcome:

- **Bug Reports**: Found a bug? Let us know!
- **Feature Requests**: Have an idea? Share it!
- **Documentation**: Help improve the docs
- **Code**: Submit a pull request
- **Feedback**: Tell us what you think!

---

## License

Hash Language is released under the **MIT License**.

Free to use, modify, and distribute.

---

## Acknowledgments

- **LLVM Project** - Compiler infrastructure
- **VS Code** - Excellent editor platform
- **PowerShell** - Powerful scripting environment
- **Community** - Thank you for using Hash

---

## Support & Resources

### Documentation

- **Complete Guide**: `docs/COMPLETE_GUIDE.md`
- **Quick Reference**: `docs/QUICK_REFERENCE.md`
- **CLI Guide**: `docs/CLI_GUIDE.md`
- **All Docs**: `C:\HashLang\docs\`

### Examples

- **23 Examples**: `C:\HashLang\examples\`
- **Index**: `examples/INDEX.md`

### Help

```powershell
hash --help      # Built-in help
hash --version   # Version info
```

### Quick Links

- Installation: `docs/INSTALLATION.md`
- Troubleshooting: `docs/POST_INSTALL_CHECKLIST.md`
- Getting Started: `docs/GETTING_STARTED.md`

---

## Thank You!

Thank you for trying Hash Language v1.0.0.

We've worked hard to create a language that's:
- **Easy to learn** (Python-like syntax)
- **Fast to run** (C-level performance)
- **Safe to use** (pure function system)
- **Professional** (complete tooling)

We hope you enjoy using Hash as much as we enjoyed building it!

---

```


Hash Language v1.0.0 - October 29, 2025


```

---

**Hash Language v1.0.0**
*Released: October 29, 2025*

*Python-like syntax, C-level performance!*
