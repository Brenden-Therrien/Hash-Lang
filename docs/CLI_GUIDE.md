# Hash CLI - Command Line Interface Guide

> Professional command-line interface for Hash Language

---

##  Quick Commands

```powershell
# Compile and run in one command (most common)
.\hash.ps1 -cr myprogram.hash

# Compile only
.\hash.ps1 -c myprogram.hash

# Run compiled program
.\hash.ps1 -r myprogram.exe

# Show version and features
.\hash.ps1 --version

# Show help
.\hash.ps1 --help
```

---

##  Command Reference

### Compile and Run (`-cr`)

**Fastest way to test your code!**

```powershell
.\hash.ps1 -cr examples\00_hello.hash
```

**What it does:**
1.  Compiles your `.hash` file
2.  Shows compilation success/errors
3.  Runs the program immediately
4.  Shows program output
5.  Reports exit code

**Output example:**
```

  Hash Language - Python-like syntax, C-level performance


→ Compiling: examples\00_hello.hash
 Compiled successfully: 00_hello.exe

→ Running: 00_hello.exe

─────────────────────────────────────────────────────
Hello, Hash!
42
─────────────────────────────────────────────────────

 Program completed (exit code: 0)
```

---

### Compile Only (`-c`)

**Compile without running** - useful for building executables.

```powershell
.\hash.ps1 -c myprogram.hash
```

**With custom output name:**
```powershell
.\hash.ps1 -c myprogram.hash -o custom_name.exe
```

**What it does:**
1.  Compiles your `.hash` file
2.  Creates an `.exe` file
3.  Shows next step (how to run it)

**Output example:**
```

  Hash Language - Python-like syntax, C-level performance


→ Compiling: myprogram.hash
 Compiled successfully: myprogram.exe

→ Next step: hash -r myprogram.exe
```

---

### Run Only (`-r`)

**Run a previously compiled program.**

```powershell
.\hash.ps1 -r myprogram.exe
```

**What it does:**
1.  Runs the executable
2.  Shows output in formatted box
3.  Reports exit code

---

### Version (`--version`)

**Show Hash compiler version and features.**

```powershell
.\hash.ps1 --version
```

**Output:**
```

  Hash Language - Python-like syntax, C-level performance


Hash Language Compiler
Version: 1.0.0
LLVM: 21.1.4

Features:
  • 50 built-in functions
  • Pure function system
  • String manipulation
  • File I/O operations
  • VS Code extension with IntelliSense
```

---

### Help (`--help`)

**Show all available commands.**

```powershell
.\hash.ps1 --help
```

Displays:
- Usage examples
- All commands
- Options
- Quick start guide
- Documentation links

---

##  Visual Features

The Hash CLI includes beautiful colors and symbols:

- **** Green checkmark for success
- **** Red X for errors
- **→** Arrow for informational messages
- **Cyan borders** for output sections
- **Magenta highlights** for important items
- **Yellow** for warnings and hints

---

##  Options

### Custom Output Name (`-o`)

Specify a custom name for the compiled executable:

```powershell
.\hash.ps1 -c myprogram.hash -o mycool_app.exe
```

### Emit LLVM IR (`--emit-llvm`)

Generate LLVM Intermediate Representation instead of executable:

```powershell
.\hash.ps1 -c myprogram.hash --emit-llvm
```

This creates `myprogram.ll` with the LLVM IR code.

### Verbose Output (`-v` or `--verbose`)

Show detailed compilation and execution commands:

```powershell
.\hash.ps1 -cr myprogram.hash -v
```

Shows:
- Full compiler command
- Executable path
- Detailed debug info

---

##  Workflow Examples

### Development Workflow

**Quick iterations:**
```powershell
# Edit myprogram.hash in VS Code with IntelliSense
# Save and test immediately:
.\hash.ps1 -cr myprogram.hash
```

**Build for distribution:**
```powershell
# Compile with custom name
.\hash.ps1 -c myprogram.hash -o MyApp.exe

# Test it
.\hash.ps1 -r MyApp.exe

# Distribute MyApp.exe
```

### Testing Examples

**Test all examples:**
```powershell
.\hash.ps1 -cr examples\00_hello.hash
.\hash.ps1 -cr examples\01_basics.hash
.\hash.ps1 -cr examples\02_control_flow.hash
# ... etc
```

**Or use the test script:**
```powershell
.\test_examples.ps1
```

### Debug Workflow

**Check LLVM IR:**
```powershell
.\hash.ps1 -c myprogram.hash --emit-llvm
notepad myprogram.ll
```

**Verbose compilation:**
```powershell
.\hash.ps1 -c myprogram.hash -v
```

---

##  Common Use Cases

### 1. Learning Hash

```powershell
# Start with hello world
.\hash.ps1 -cr examples\00_hello.hash

# Try basic features
.\hash.ps1 -cr examples\01_basics.hash

# Learn control flow
.\hash.ps1 -cr examples\02_control_flow.hash

# Explore pure functions
.\hash.ps1 -cr examples\04_pure_functions.hash
```

### 2. Building a Project

```powershell
# Create your main file
code myproject.hash

# Use IntelliSense: main<Tab> to get template

# Compile and test
.\hash.ps1 -cr myproject.hash

# Build final version
.\hash.ps1 -c myproject.hash -o MyProject.exe
```

### 3. Quick Experiments

```powershell
# Create test file
code test.hash

# Use snippets to quickly write code
# fn<Tab>, let<Tab>, if<Tab>, prints<Tab>

# Run immediately
.\hash.ps1 -cr test.hash
```

---

## Troubleshooting

### Compiler Not Found

**Error:** `Compiler not found at: build\Release\hashc.exe`

**Solution:**
```powershell
# Build the compiler first
.\build.ps1
```

### Source File Not Found

**Error:** `Source file not found: myprogram.hash`

**Solutions:**
- Check the file exists: `ls myprogram.hash`
- Use correct path: `.\hash.ps1 -cr examples\myprogram.hash`
- Check spelling

### Executable Not Found

**Error:** `Executable not found: myprogram.exe`

**Solutions:**
- Compile first: `.\hash.ps1 -c myprogram.hash`
- Check if compilation succeeded
- Use correct filename

---

##  Comparison: Old vs New

### Old Way
```powershell
# Multiple steps, manual paths
.\build\Release\hashc.exe myprogram.hash
.\myprogram.exe

# No colors, no formatting
# Manual error checking
```

### New Way (Hash CLI)
```powershell
# One command
.\hash.ps1 -cr myprogram.hash

#  Beautiful colors
#  Success/error indicators
#  Formatted output
#  Exit code reporting
#  Professional experience
```

---

##  Pro Tips

1. **Alias for Speed**
   ```powershell
   # Add to your PowerShell profile
   Set-Alias hash "C:\Users\YourName\Desktop\HashLang\hash.ps1"

   # Then use:
   hash -cr myprogram.hash
   ```

2. **Development Mode**
   ```powershell
   # Keep a terminal open and run after each save
   .\hash.ps1 -cr myprogram.hash
   # Hit Up Arrow + Enter to repeat
   ```

3. **VS Code Integration**
   - Use the VS Code terminal
   - IntelliSense while coding
   - Immediate testing with CLI

4. **Batch Testing**
   ```powershell
   # Test multiple files
   ls examples\*.hash | ForEach-Object {
       Write-Host "Testing: $($_.Name)" -ForegroundColor Cyan
       .\hash.ps1 -cr $_.FullName
   }
   ```

---

##  Related Documentation

- **Getting Started**: `docs/GETTING_STARTED.md`
- **Quick Reference**: `docs/QUICK_REFERENCE.md`
- **Code Completion**: `docs/CODE_COMPLETION.md`
- **Installation Checklist**: `INSTALLATION_CHECKLIST.md`

---

##  Next Steps

1. **Try the CLI**
   ```powershell
   .\hash.ps1 --help
   .\hash.ps1 --version
   .\hash.ps1 -cr examples\00_hello.hash
   ```

2. **Create Your First Program**
   ```powershell
   code myprogram.hash
   # Use main<Tab> snippet
   # Write your code
   .\hash.ps1 -cr myprogram.hash
   ```

3. **Explore Examples**
   ```powershell
   ls examples\*.hash
   .\hash.ps1 -cr examples\<filename>
   ```

---

**Hash Language** - Professional development experience.

*The CLI makes Hash a joy to use!*
