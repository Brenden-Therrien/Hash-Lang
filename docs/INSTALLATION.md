# Hash Language - Installation Guide

> Install Hash Language system-wide for easy access from anywhere

---

##  Quick Install

```powershell
# Navigate to Hash directory
cd HashLang

# Run the installer
.\install.ps1
```

**That's it.** After restarting your terminal, you can use `hash` from anywhere.

---

## What Gets Installed

### Installation Location
- **Default**: `C:\HashLang\`
- **Custom**: `.\install.ps1 -InstallPath "C:\Your\Custom\Path"`

### Components Installed

1. **Hash Compiler** (`hashc.exe`)
   - Location: `C:\HashLang\bin\hashc.exe`
   - LLVM-based compiler for Hash → native code

2. **Hash CLI** (`hash.cmd`, `hash.ps1`)
   - Location: `C:\HashLang\bin\`
   - Professional command-line interface
   - Accessible globally as `hash`

3. **Examples** (23 files)
   - Location: `C:\HashLang\examples\`
   - Working examples for all Hash features

4. **Documentation**
   - Location: `C:\HashLang\docs\`
   - Complete language and API reference

5. **VS Code Extension**
   - Location: `%USERPROFILE%\.vscode\extensions\hash-lang-1.0.0\`
   - Syntax highlighting with vibrant colors
   - 30+ IntelliSense snippets
   - Auto-indentation and bracket pairing

### System Changes

- **PATH Updated**: `C:\HashLang\bin` added to user PATH
- **Command Available**: `hash` command works globally
- **No Admin Required**: Installs to user directories only

---

##  Post-Installation

### Step 1: Restart Terminal

**Important!** You must restart your terminal/PowerShell for PATH changes to take effect.

Close all terminal windows and open a new one.

### Step 2: Verify Installation

```powershell
# Check version
hash --version

# Should output:
# Hash Language Compiler
# Version: 1.0.0
# LLVM: 21.1.4
# Features: ...
```

### Step 3: Test Compilation

```powershell
# From any directory
hash -cr C:\HashLang\examples\00_hello.hash

# Or create your own:
echo 'fn main() -> i32:
    print_str("Hello!")
    return 0' > test.hash

hash -cr test.hash
```

### Step 4: Restart VS Code

If you want syntax highlighting and IntelliSense:

1. Close all VS Code windows
2. Reopen VS Code
3. Open any `.hash` file
4. Enjoy **PINK** `pure` and **ORANGE** `pure_local` keywords!

---

##  Usage Examples

### Compile and Run (Most Common)

```powershell
# Create a Hash file anywhere
cd C:\Users\YourName\Documents
notepad myprogram.hash

# Compile and run
hash -cr myprogram.hash
```

### Compile Only

```powershell
# Compile to executable
hash -c myprogram.hash

# Or with custom name
hash -c myprogram.hash -o MyApp.exe
```

### Run Executable

```powershell
hash -r myprogram.exe
```

### Get Help

```powershell
hash --help
hash --version
```

---

##  Installation Options

### Default Installation

```powershell
.\install.ps1
```

Installs to `C:\HashLang` with all components.

### Custom Location

```powershell
.\install.ps1 -InstallPath "D:\MyTools\Hash"
```

Installs to your specified directory.

### Skip VS Code Extension

```powershell
.\install.ps1 -SkipVSCode
```

Installs compiler and CLI only, skips VS Code extension.

### Custom Location + Skip Extension

```powershell
.\install.ps1 -InstallPath "C:\Tools\Hash" -SkipVSCode
```

---

## Uninstallation

### Complete Removal

```powershell
cd HashLang
.\install.ps1 -Uninstall
```

This will:
- Remove `C:\HashLang` directory
- Remove Hash from PATH
- Remove VS Code extension
- Confirm before uninstalling

### Manual Removal

If you need to remove manually:

1. Delete installation directory: `C:\HashLang`
2. Remove from PATH:
   - Open: System Properties → Environment Variables
   - Edit User PATH
   - Remove: `C:\HashLang\bin`
3. Delete VS Code extension:
   - Delete: `%USERPROFILE%\.vscode\extensions\hash-lang-1.0.0`

---

## Troubleshooting

### "hash" Command Not Found

**Problem**: Terminal says `hash: command not found` or similar

**Solutions**:
1. **Restart your terminal** (most common fix!)
2. Verify PATH: `echo $env:Path` should contain `C:\HashLang\bin`
3. Reinstall: `.\install.ps1`

### PATH Not Updated

**Problem**: Installation completes but `hash` doesn't work

**Solutions**:
1. **Close ALL terminal windows** and open a new one
2. Log out and log back in (Windows)
3. Manually check: `[Environment]::GetEnvironmentVariable("Path", "User")`

### VS Code Extension Not Working

**Problem**: No syntax highlighting in VS Code

**Solutions**:
1. **Restart VS Code** (close all windows)
2. Check file extension is `.hash`
3. Reinstall extension: `.\install.ps1`
4. Check extension folder exists:
   ```powershell
   Test-Path "$env:USERPROFILE\.vscode\extensions\hash-lang-1.0.0"
   ```

### Compiler Not Found

**Problem**: `Compiler not found at: C:\HashLang\bin\hashc.exe`

**Solutions**:
1. Build first: `.\build.ps1`
2. Reinstall: `.\install.ps1`
3. Check compiler exists: `Test-Path "C:\HashLang\bin\hashc.exe"`

### Permission Errors

**Problem**: Installation fails with permission errors

**Solutions**:
1. Try custom location in your user folder:
   ```powershell
   .\install.ps1 -InstallPath "$env:USERPROFILE\HashLang"
   ```
2. Check disk space
3. Close programs that might lock files

---

##  Installation Summary

After successful installation:

```
C:\HashLang\
├── bin\
│   ├── hashc.exe          ← Compiler
│   ├── hash.ps1           ← CLI script
│   └── hash.cmd           ← CLI wrapper
├── examples\
│   ├── 00_hello.hash
│   ├── 01_basics.hash
│   └── ... (23 total)
├── docs\
│   ├── GETTING_STARTED.md
│   ├── CLI_GUIDE.md
│   ├── QUICK_REFERENCE.md
│   └── ... (10+ guides)
└── README.md

User PATH:
  C:\HashLang\bin  ← Added

VS Code:
  %USERPROFILE%\.vscode\extensions\hash-lang-1.0.0\
```

---

##  Next Steps After Installation

### 1. Learn the CLI

```powershell
hash --help           # View all commands
hash --version        # Check installation
```

### 2. Try Examples

```powershell
hash -cr C:\HashLang\examples\00_hello.hash
hash -cr C:\HashLang\examples\01_basics.hash
hash -cr C:\HashLang\examples\04_pure_functions.hash
```

### 3. Create Your First Program

```powershell
# Go to your preferred directory
cd C:\Users\YourName\Documents

# Create a Hash file (use VS Code for IntelliSense)
code myprogram.hash

# Type: main<Tab> to get template (if using VS Code)
# Or write manually:
fn main() -> i32:
    print_str("My first Hash program!")
    println()
    return 0

# Compile and run
hash -cr myprogram.hash
```

### 4. Read Documentation

Open in your browser or text editor:
- `C:\HashLang\docs\GETTING_STARTED.md` - Complete guide
- `C:\HashLang\docs\CLI_GUIDE.md` - CLI reference
- `C:\HashLang\docs\QUICK_REFERENCE.md` - One-page cheat sheet

---

##  Pro Tips

### Add Alias (Optional)

For even shorter commands, add to your PowerShell profile:

```powershell
# Edit profile
notepad $PROFILE

# Add this line:
Set-Alias h hash

# Now use:
h -cr myprogram.hash
h --version
```

### VS Code Workspace

Create a workspace for your Hash projects:

```powershell
mkdir C:\Users\YourName\HashProjects
code C:\Users\YourName\HashProjects
```

Benefits:
- IntelliSense works automatically
- Syntax highlighting with vibrant colors
- 30+ snippets (main, fn, pure, if, while, etc.)

### Quick Test Workflow

```powershell
# Create test.hash
code test.hash

# Edit in VS Code with IntelliSense
# Save (Ctrl+S)

# Run immediately
hash -cr test.hash

# Iterate quickly
```

---

##  What You Can Do Now

With Hash installed globally, you can:

 **Compile from anywhere**: `hash -c myfile.hash`
 **One-command run**: `hash -cr myfile.hash`
 **Professional output**: Colored, formatted, beautiful
 **VS Code integration**: Full IDE experience
 **Access examples**: `C:\HashLang\examples\`
 **Read docs**: `C:\HashLang\docs\`
 **Quick help**: `hash --help`

---

## Support

- **Installation issues**: Check troubleshooting section above
- **Usage questions**: See `C:\HashLang\docs\CLI_GUIDE.md`
- **Language questions**: See `C:\HashLang\docs\QUICK_REFERENCE.md`
- **Examples**: Browse `C:\HashLang\examples\`

---

## Updating Hash

To update to a newer version:

```powershell
# 1. Navigate to Hash source
cd C:\Path\To\Hash\Source

# 2. Pull latest changes (if using git)
git pull

# 3. Rebuild
.\build.ps1

# 4. Reinstall
.\install.ps1

# Hash is now updated
```

---

**Hash Language** - Installed and ready to use. 

*Now you can use `hash` from anywhere in your system!* 
