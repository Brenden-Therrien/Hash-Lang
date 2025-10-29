# Hash Language - Post-Installation Checklist

> Verify your Hash installation is working correctly

---

##  Installation Verification

After running `.\install.ps1` and **restarting your terminal**, follow this checklist:

### Step 1: Check Version

```powershell
hash --version
```

**Expected Output:**
```

                    Hash Language Compiler
                         Version: 1.0.0
                          LLVM: 21.1.4
───────────────────────────────────────────────────────────────

Features:
  • 50 built-in functions
  • Pure function system
  • String manipulation
  • File I/O operations
  • VS Code extension with IntelliSense


```

- [ ] Command runs successfully
- [ ] Shows version 1.0.0
- [ ] Shows LLVM 21.1.4
- [ ] Lists features

---

### Step 2: Check Help System 

```powershell
hash --help
```

**Expected Output:**
```

           Hash Language - Python-like syntax,
                C-level performance
───────────────────────────────────────────────────────────────

USAGE:
  hash -c <file>           Compile a Hash file
  hash -r <file>           Run a compiled executable
  hash -cr <file>          Compile and run
  hash --version           Show compiler version
  hash --help              Show this help message
  ...
```

- [ ] Help displays correctly
- [ ] Shows all commands
- [ ] Shows examples

---

### Step 3: Verify Installation Location 

```powershell
# Check compiler exists
Test-Path "C:\HashLang\bin\hashc.exe"

# Check CLI exists
Test-Path "C:\HashLang\bin\hash.ps1"

# Check examples exist
Test-Path "C:\HashLang\examples\00_hello.hash"
```

**Expected Output:**
```
True
True
True
```

- [ ] Compiler found at C:\HashLang\bin\hashc.exe
- [ ] CLI found at C:\HashLang\bin\hash.ps1
- [ ] Examples found in C:\HashLang\examples\

---

### Step 4: Verify PATH 

```powershell
# Check if C:\HashLang\bin is in PATH
$env:Path -split ';' | Select-String "HashLang"
```

**Expected Output:**
```
C:\HashLang\bin
```

- [ ] C:\HashLang\bin is in user PATH

---

### Step 5: Test from Different Directory 

```powershell
# Go to a completely different location
cd C:\

# Run hash command
hash --version
```

**Expected Result:**
- Version info displays correctly even from C:\

- [ ] `hash` command works from C:\
- [ ] Command accessible from any directory

---

### Step 6: Compile Example 

```powershell
hash -c C:\HashLang\examples\00_hello.hash
```

**Expected Output:**
```

           Hash Language - Python-like syntax,
                C-level performance
───────────────────────────────────────────────────────────────

→ Compiling: C:\HashLang\examples\00_hello.hash

 Compiled successfully: 00_hello.exe
  Output: C:\HashLang\examples\00_hello.exe


```

- [ ] Compilation succeeds
- [ ] Executable created (00_hello.exe)
- [ ] No errors shown

---

### Step 7: Run Executable 

```powershell
hash -r C:\HashLang\examples\00_hello.exe
```

**Expected Output:**
```

           Hash Language - Python-like syntax,
                C-level performance
───────────────────────────────────────────────────────────────

→ Running: C:\HashLang\examples\00_hello.exe

─────────────────────────────────────────────────────────────────
42
-17
3.141590
1
...
─────────────────────────────────────────────────────────────────

 Program completed (exit code: 0)


```

- [ ] Program runs successfully
- [ ] Output shows between border lines
- [ ] Exit code is 0

---

### Step 8: Compile and Run Together 

```powershell
hash -cr C:\HashLang\examples\01_basics.hash
```

**Expected Output:**
```
→ Compiling: C:\HashLang\examples\01_basics.hash
 Compiled successfully: 01_basics.exe

→ Running: 01_basics.exe
─────────────────────────────────────────────────────────────────
[program output]
─────────────────────────────────────────────────────────────────
 Program completed (exit code: 0)
```

- [ ] Compiles successfully
- [ ] Runs immediately after compilation
- [ ] Shows formatted output

---

### Step 9: Test VS Code Extension 

1. Open VS Code:
   ```powershell
   code C:\HashLang\examples\00_hello.hash
   ```

2. Check for:
   - [ ] `.hash` file opens with syntax highlighting
   - [ ] `pure` keyword appears in **PINK/Magenta**
   - [ ] `pure_local` keyword appears in **ORANGE**
   - [ ] Keywords like `fn`, `let`, `return` are highlighted
   - [ ] Comments appear in green/gray
   - [ ] Strings are highlighted

3. Test IntelliSense:
   - [ ] Type `main` and press Tab → should expand to full function template
   - [ ] Type `fn` and press Tab → should expand to function template
   - [ ] Type `pure` and press Tab → should expand to pure function template

---

### Step 10: Create Your Own Program 

```powershell
# Go to your Documents
cd $env:USERPROFILE\Documents

# Create a test file
@"
fn main() -> i32:
    print_str("Hash is installed!")
    println()
    return 0
"@ | Out-File -Encoding utf8 test.hash

# Compile and run
hash -cr test.hash
```

**Expected Output:**
```
→ Compiling: test.hash
 Compiled successfully: test.exe

→ Running: .\test.exe
─────────────────────────────────────────────────────────────────
Hash is installed!

─────────────────────────────────────────────────────────────────
 Program completed (exit code: 0)
```

- [ ] Can create .hash file in any directory
- [ ] Can compile from any directory
- [ ] Program runs correctly

---

##  Installation Complete

If all checkboxes are checked, your Hash installation is **fully functional**!

### What You Verified

 **Hash CLI**: Working globally (`hash` command)
 **Compiler**: hashc.exe found and functional
 **PATH**: C:\HashLang\bin in system PATH
 **Examples**: All 23 examples accessible
 **Documentation**: Available in C:\HashLang\docs\
 **VS Code Extension**: Syntax highlighting and IntelliSense working
 **Compilation**: Can compile .hash files
 **Execution**: Can run compiled programs
 **Global Access**: Works from any directory

---

##  Troubleshooting

### If Any Step Failed

#### Command Not Found
```
hash : The term 'hash' is not recognized...
```

**Fix:**
1. Restart your terminal (close all windows!)
2. Log out and back in to Windows
3. Re-run installer: `.\install.ps1`

#### Compiler Not Found
```
Compiler not found at: C:\HashLang\bin\hashc.exe
```

**Fix:**
1. Build first: `.\build.ps1`
2. Reinstall: `.\install.ps1`

#### VS Code Extension Not Working

**Fix:**
1. Close ALL VS Code windows
2. Reopen VS Code
3. Open a .hash file
4. If still not working: `.\install.ps1`

#### PATH Not Updated

**Fix:**
```powershell
# Manually check PATH
[Environment]::GetEnvironmentVariable("Path", "User")

# Should contain: C:\HashLang\bin

# If not, re-run installer
.\install.ps1
```

---

##  Next Steps

Now that Hash is installed, you can:

1. **Learn the CLI**: `hash --help`
2. **Try examples**: `hash -cr C:\HashLang\examples\04_pure_functions.hash`
3. **Read docs**: `C:\HashLang\docs\GETTING_STARTED.md`
4. **Create programs**: Use VS Code with IntelliSense
5. **Explore features**: Check all 23 examples

### Recommended Reading Order

1. [GETTING_STARTED.md](GETTING_STARTED.md) - Language basics
2. [CLI_GUIDE.md](CLI_GUIDE.md) - Command reference
3. [CODE_COMPLETION.md](CODE_COMPLETION.md) - IntelliSense guide
4. [QUICK_REFERENCE.md](QUICK_REFERENCE.md) - One-page cheat sheet

---

##  Installation Summary

```
 Hash Language v1.0.0 - FULLY INSTALLED

Location:     C:\HashLang\
Command:      hash (global)
Compiler:     LLVM 21.1.4
Examples:     23 files
Docs:         11 guides
VS Code:      Extension installed
IntelliSense: 30+ snippets

Ready to compile and run Hash programs. 
```

---

**Congratulations.** You can now use Hash from anywhere in your system. 

Start creating with: `hash -cr myprogram.hash`
