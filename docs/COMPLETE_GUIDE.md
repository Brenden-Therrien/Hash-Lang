# Hash Language - Complete Installation & Usage Guide

> **Your one-stop guide for installing, using, and mastering Hash Language**

---

## Table of Contents

1. [Quick Install](#quick-install)
2. [What You Get](#what-you-get)
3. [First Steps](#first-steps)
4. [Using Hash](#using-hash)
5. [Development Workflow](#development-workflow)
6. [Documentation Index](#documentation-index)
7. [Troubleshooting](#troubleshooting)
8. [Support](#support)

---

##  Quick Install

### Step 1: Build

```powershell
cd HashLang
.\build.ps1
```

**What this does:**
- Compiles the Hash compiler (hashc.exe)
- Installs VS Code extension
- Takes ~30 seconds

### Step 2: Install

```powershell
.\install.ps1
```

**What this does:**
- Copies compiler to C:\HashLang\bin\
- Creates global `hash` command
- Adds C:\HashLang\bin to PATH
- Copies examples and documentation
- Installs VS Code extension
- Takes ~10 seconds

### Step 3: Restart

**IMPORTANT:** Close and reopen your terminal!

PATH changes only take effect in new terminal sessions.

### Step 4: Verify

```powershell
hash --version
```

**Expected:**
```
Hash Language Compiler
Version: 1.0.0
LLVM: 21.1.4
```

** If you see this, you're ready to go!**

---

## What You Get

After installation, you'll have:

### Global Command

```powershell
hash               # Works from any directory
```

### Professional CLI

- **-c** - Compile Hash files
- **-r** - Run executables
- **-cr** - Compile and run (most common!)
- **--version** - Show version
- **--help** - Full help system

### IDE Support

- **VS Code extension** with:
  - Syntax highlighting (PINK `pure`, ORANGE `pure_local`)
  - 30+ IntelliSense snippets
  - Auto-indentation
  - Bracket pairing

### Examples & Docs

- **23 working examples** in `C:\HashLang\examples\`
- **Complete documentation** in `C:\HashLang\docs\`
- **50 built-in functions** documented

---

##  First Steps

### 1. Test the Installation

Follow the checklist:

```powershell
# Open in your browser or text editor
code C:\HashLang\docs\POST_INSTALL_CHECKLIST.md
```

This will verify:
-  Command works globally
-  Compiler found
-  PATH updated
-  Examples accessible
-  VS Code extension working

### 2. Run Your First Example

```powershell
hash -cr C:\HashLang\examples\00_hello.hash
```

**You should see:**
```
→ Compiling: C:\HashLang\examples\00_hello.hash
 Compiled successfully: 00_hello.exe

→ Running: 00_hello.exe
─────────────────────────────────────────────────────
42
-17
3.141590
...
─────────────────────────────────────────────────────
 Program completed (exit code: 0)
```

### 3. Create Your First Program

```powershell
# Go to your preferred directory
cd $env:USERPROFILE\Documents

# Open VS Code
code myprogram.hash
```

**In VS Code:**

1. Type: `main` and press **Tab**
2. IntelliSense will expand to:
   ```hash
   fn main() -> i32:

       return 0
   ```
3. Fill in your code:
   ```hash
   fn main() -> i32:
       print_str("Hello from Hash!")
       println()
       return 0
   ```
4. Save (Ctrl+S)

**Run it:**

```powershell
hash -cr myprogram.hash
```

** You just wrote and ran your first Hash program!**

---

##  Using Hash

### Common Commands

#### Compile and Run (Most Common)

```powershell
hash -cr myprogram.hash
```

One command does it all!

#### Compile Only

```powershell
# Default output name
hash -c myprogram.hash      # Creates: myprogram.exe

# Custom output name
hash -c myprogram.hash -o MyApp.exe
```

#### Run Executable

```powershell
hash -r myprogram.exe
```

Runs with formatted output and exit code reporting.

#### Get Help

```powershell
hash --help      # Full command reference
hash --version   # Version info
```

### Advanced Options

#### Emit LLVM IR

```powershell
hash -c myprogram.hash --emit-llvm
```

Generates `myprogram.ll` (LLVM intermediate representation).

#### Verbose Output

```powershell
hash -c myprogram.hash -v
```

Shows detailed compilation steps.

---

## Development Workflow

### Quick Iteration

```powershell
# Edit in VS Code (with IntelliSense)
code myprogram.hash

# Quick test
hash -cr myprogram.hash

# Repeat
```

### Project Organization

```powershell
# Create project folder
mkdir MyHashProject
cd MyHashProject

# Create files
code main.hash
code utils.hash

# Compile and run
hash -cr main.hash
```

### VS Code Integration

**Snippets available (type + Tab):**

- `main` → Full main function
- `fn` → Function template
- `pure` → Pure function template
- `if` → If statement
- `while` → While loop
- `for` → For loop
- `let` → Variable declaration
- `print` → Print function
- And 20+ more!

**Color Scheme:**

- **PINK** → `pure` keyword
- **ORANGE** → `pure_local` keyword
- **Blue** → Keywords (fn, let, return)
- **Green** → Strings
- **Gray** → Comments

---

##  Documentation Index

### Getting Started

1. **[INSTALLATION.md](INSTALLATION.md)**
   - Detailed installation guide
   - Custom install paths
   - Uninstallation
   - Troubleshooting

2. **[POST_INSTALL_CHECKLIST.md](POST_INSTALL_CHECKLIST.md)**
   - Verify installation
   - 10-step checklist
   - Common issues

3. **[GETTING_STARTED.md](GETTING_STARTED.md)**
   - Language basics
   - Your first program
   - Core concepts

### Reference

4. **[QUICK_REFERENCE.md](QUICK_REFERENCE.md)**
   - One-page cheat sheet
   - All 50 built-in functions
   - Syntax at a glance

5. **[CLI_GUIDE.md](CLI_GUIDE.md)**
   - Complete CLI reference
   - All commands explained
   - Workflow examples

6. **[CODE_COMPLETION.md](CODE_COMPLETION.md)**
   - IntelliSense guide
   - All 30+ snippets
   - VS Code tips

### Advanced

7. **[SYNTAX_COLORS.md](SYNTAX_COLORS.md)**
   - Syntax highlighting guide
   - Color customization
   - Theme details

8. **[PURE_FUNCTIONS.md](PURE_FUNCTIONS.md)**
   - Pure function system
   - Best practices
   - Examples

### Development

9. **[DEVELOPMENT_SUMMARY.md](DEVELOPMENT_SUMMARY.md)**
   - Project history
   - Features added
   - Phase breakdown

10. **[CHANGELOG.md](CHANGELOG.md)**
    - Version history
    - New features
    - Bug fixes

---

##  Troubleshooting

### "hash" Command Not Found

**Problem:**
```
hash : The term 'hash' is not recognized as the name of a cmdlet...
```

**Solution:**
```powershell
# 1. Restart your terminal (most important)
# Close all terminal windows and open a new one

# 2. If still not working, check PATH:
$env:Path -split ';' | Select-String "HashLang"

# Should show: C:\HashLang\bin

# 3. If not there, reinstall:
.\install.ps1
```

### VS Code Extension Not Working

**Problem:** No syntax highlighting in .hash files

**Solution:**
```powershell
# 1. Close ALL VS Code windows
# 2. Reopen VS Code
# 3. Open a .hash file
# 4. Check file extension is .hash (not .txt)
# 5. If still not working:
.\install.ps1   # Reinstalls extension
```

### Compilation Errors

**Problem:** Compiler shows errors

**Solutions:**
```powershell
# Check syntax (Python-like indentation)
# - Use 4 spaces for indentation
# - No tabs
# - Colons after function definitions
# - Return types required

# Example:
fn main() -> i32:     # ← Colon here
    print_str("Hi")   # ← 4 spaces indent
    return 0          # ← Must return
```

### Examples Don't Run

**Problem:** Examples fail to compile or run

**Solution:**
```powershell
# Check installation
Test-Path "C:\HashLang\examples\00_hello.hash"

# Should be True

# If False, reinstall:
.\install.ps1

# Try absolute path:
hash -cr C:\HashLang\examples\00_hello.hash
```

### PATH Not Updated

**Problem:** Installation succeeds but `hash` doesn't work

**Solution:**
```powershell
# Manual PATH check:
[Environment]::GetEnvironmentVariable("Path", "User")

# Should contain: C:\HashLang\bin

# If not:
# 1. Log out and back in to Windows
# 2. Or restart computer
# 3. Or reinstall:
.\install.ps1
```

---

##  Support

### Quick Help

```powershell
hash --help          # Built-in help
hash --version       # Version info
```

### Documentation

All docs in: `C:\HashLang\docs\`

Key files:
- `INSTALLATION.md` - Installation issues
- `CLI_GUIDE.md` - Usage questions
- `QUICK_REFERENCE.md` - Language reference
- `POST_INSTALL_CHECKLIST.md` - Verify installation

### Examples

23 working examples in: `C:\HashLang\examples\`

Categories:
- **Basics**: 00-03
- **Pure Functions**: 04
- **Math**: 05-06
- **Control Flow**: 07-09
- **Functions**: 10-11
- **Error Handling**: 12-13
- **Arrays**: 14
- **User Input**: 15
- **Strings**: 16-19
- **File I/O**: 20-22

### Common Issues

| Issue | Quick Fix |
|-------|-----------|
| Command not found | Restart terminal |
| No syntax colors | Restart VS Code |
| Compilation fails | Check indentation (4 spaces) |
| Examples missing | Reinstall: `.\install.ps1` |
| PATH not updated | Log out and back in |

---

##  Learning Path

### Beginner (Day 1)

1. Install Hash (this guide!)
2. Run: `hash -cr C:\HashLang\examples\00_hello.hash`
3. Read: [GETTING_STARTED.md](GETTING_STARTED.md)
4. Create your first program (above)
5. Try examples: 00, 01, 02, 03

### Intermediate (Week 1)

1. Learn pure functions: Example 04
2. Master the CLI: [CLI_GUIDE.md](CLI_GUIDE.md)
3. Use VS Code snippets: [CODE_COMPLETION.md](CODE_COMPLETION.md)
4. Try all 50 built-in functions
5. Build a small project

### Advanced (Week 2+)

1. Deep dive: Pure function system
2. File I/O: Examples 20-22
3. Error handling: Examples 12-13
4. Optimize code with LLVM IR (`--emit-llvm`)
5. Build real applications!

---

##  What Makes Hash Special

### Python-like Syntax

```hash
# Clean, readable code
fn calculate_average(numbers: i32[], count: i32) -> f64:
    let sum: i32 = 0
    let i: i32 = 0
    while i < count:
        sum = sum + numbers[i]
        i = i + 1
    return float(sum) / float(count)
```

### C-Level Performance

- Compiles to native code via LLVM
- No runtime overhead
- Optimizations: -O3 by default
- As fast as C/C++!

### Pure Function System

```hash
# Enforced purity - no side effects
pure fn add(a: i32, b: i32) -> i32:
    return a + b

# Compiler guarantees safety
```

### Professional Tools

- Beautiful CLI with colors
- VS Code integration
- IntelliSense snippets
- Complete documentation
- 23 working examples

---

##  Ready to Build

You now have everything you need to start developing with Hash:

 **Installed**: Global `hash` command
 **Compiler**: LLVM-powered, C-level performance
 **IDE**: VS Code with IntelliSense
 **Examples**: 23 working programs
 **Docs**: Complete language reference
 **CLI**: Professional command-line interface

**Start coding:**

```powershell
cd $env:USERPROFILE\Documents
code myproject.hash
# Type 'main' + Tab to get started
hash -cr myproject.hash
```

---

**Hash Language** - Python-like syntax, C-level performance!

**
