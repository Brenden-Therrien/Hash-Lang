# Setup Wizard Guide

> Interactive first-run experience for Hash Language

---

##  What is the Setup Wizard?

The Setup Wizard is an **interactive guide** that helps new users get started with Hash Language. It runs after installation and walks you through:

1.  **Verifying Installation** - Checks compiler, examples, docs
2. 📁 **Creating Workspace** - Sets up a project folder with starter examples
3.  **Configuring VS Code** - Opens your workspace in VS Code
4.  **Quick Tutorial** - 2-minute crash course in Hash basics
5.  **First Program** - Run Hello World and see Hash in action!

---

##  Running the Wizard

### Automatically (Recommended)

The wizard prompts you to run after installation:

```powershell
.\install.ps1
# ... installation completes ...
# Wizard prompt appears:
# "Would you like to run the interactive setup wizard now? (Y/N)"
```

### Manually

Run the wizard anytime:

```powershell
# From Hash directory
.\setup-wizard.ps1

# Or using hash command
hash --setup
```

---

## What the Wizard Does

### Step 1: Verify Installation

Checks that everything is properly installed:
-  Hash compiler (hashc.exe)
-  Global 'hash' command
-  Example programs (23 files)
-  Documentation (17+ guides)

**Interactive**: Just watch, no input needed

---

### Step 2: Set Up Workspace

Creates a project folder for your Hash programs:

**Options:**
1. Desktop/HashProjects (quick access)
2. Documents/HashProjects (organized)
3. Skip (set up your own)

**Example Choices:**
1. Just Hello World
2. Starter pack (4 essential examples)
3. All 23 examples
4. None (explore in C:\HashLang\examples\)

**Result:** Ready-to-use workspace with examples!

---

### Step 3: VS Code Configuration

Helps you set up VS Code for Hash development:

**Checks:**
- Is VS Code installed?
- Is Hash extension installed?
- Would you like to open workspace in VS Code?

**Interactive:** Choose whether to open VS Code now or later

**Benefits:**
- Syntax highlighting with vibrant colors
- IntelliSense (30+ snippets)
- Auto-indentation
- Bracket pairing

---

### Step 4: Quick Tutorial

**2-minute crash course** covering:

1. **Basic Commands**
   - `hash -cr myprogram.hash` (compile + run)
   - `hash --help` (get help)
   - `hash --version` (check version)

2. **Basic Syntax**
   - Python-like indentation
   - Type annotations
   - Function definitions
   - Return statements

3. **VS Code Magic**
   - Snippet shortcuts
   - Type `main` + Tab
   - Type `fn` + Tab
   - Type `pure` + Tab

4. **Try It Now**
   - Optionally run Hello World
   - See Hash in action!

**Interactive:** Choose to see tutorial or skip

---

### Step 5: What's Next

Final recommendations and next steps:

**Shows:**
- Your workspace location
- Quick command reference
- Learning resources
- Recommended first steps

**Options to Open:**
- WHATS_NEXT.md (project ideas)
- GETTING_STARTED.md (full tutorial)
- QUICK_REFERENCE.md (cheat sheet)

---

##  User Experience

### Visual Design

The wizard uses **colored output** for clarity:

- **Magenta Headers** - Section titles
- **Cyan Steps** - Current action
-  **Green Checks** - Success indicators
-  **Yellow Questions** - User prompts
- **Gray Info** - Additional details

### Progress Flow

```
Welcome Screen
    ↓
Verify Installation (automatic)
    ↓
Choose Workspace Location
    ↓
Select Examples to Copy
    ↓
Configure VS Code
    ↓
Quick Tutorial (optional)
    ↓
What's Next (recommendations)
    ↓
Ready to Code!
```

---

## ⏱️ Time Investment

- **Minimum** (skip all): ~30 seconds
- **Quick** (workspace only): ~2 minutes
- **Standard** (workspace + tutorial): ~5 minutes
- **Complete** (everything + examples): ~10 minutes

**Recommended:** Standard setup (~5 minutes)

---

##  Benefits

### For New Users

 **Removes Confusion** - Clear, guided setup
 **Quick Start** - Working environment in minutes
 **Learning Built-in** - Tutorial included
 **Confidence Builder** - See Hash work immediately

### For Experienced Users

 **Fast Setup** - Skip what you don't need
 **Flexible** - Choose your own path
 **Rerunnable** - Can run again anytime
 **Optional** - Not required, just helpful

---

##  Tips & Tricks

### Skip Welcome Screen

```powershell
.\setup-wizard.ps1 -SkipWelcome
```

Jumps straight to verification.

### Rerun for New Workspace

Already set up but want a new workspace?

```powershell
hash --setup
```

Creates a new workspace, doesn't affect existing one.

### Use for Teaching

Great for workshops or teaching Hash:

```powershell
# Run wizard for each student
hash --setup
```

Everyone gets the same setup experience!

---

##  Customization

### Workspace Location

The wizard suggests:
- Desktop/HashProjects
- Documents/HashProjects

But you can create your own:
```powershell
mkdir C:\Dev\MyHashStuff
cd C:\Dev\MyHashStuff
```

### Example Selection

- **Option 1**: Just hello.hash (smallest)
- **Option 2**: 4 starter examples (recommended)
- **Option 3**: All 23 examples (complete)
- **Option 4**: None (explore from C:\HashLang\)

Choose based on your learning style!

---

##  What Gets Created

After running the wizard with default options:

```
C:\Users\YourName\Desktop\HashProjects\
├── 00_hello.hash           # Hello World
├── 01_basics.hash          # Fundamentals
├── 03_functions.hash       # Function patterns
└── 04_pure_functions.hash  # Pure function system
```

**Ready to open in VS Code and start coding!**

---

##  For Teachers & Workshop Leaders

### Running in Classrooms

The wizard is perfect for workshops:

1. **Pre-install Hash** on all machines
2. **Run wizard** as first activity
3. **Everyone starts together** with same setup
4. **Jump into coding** immediately!

### Teaching Flow

```
1. Students install Hash (.\install.ps1)
2. Wizard runs automatically (or run hash --setup)
3. Everyone creates workspace on Desktop
4. Copy starter examples
5. Tutorial runs (all see same content)
6. Everyone opens VS Code together
7. Start first lesson with working environment!
```

### Benefits for Teaching

-  **Standardized setup** - Everyone has same environment
-  **Fast onboarding** - 5 minutes to productive
-  **Visual feedback** - Students see success indicators
-  **Reduces questions** - Wizard handles common setup issues

---

## FAQ

### Q: Do I have to run the wizard?

**A:** No! It's completely optional. You can:
- Skip it during installation
- Set up manually
- Use Hash without a workspace

### Q: Can I run it more than once?

**A:** Yes! Run `hash --setup` anytime. It won't affect:
- Existing workspaces
- Your Hash installation
- Any files you've created

### Q: What if I skip it?

**A:** No problem! You can:
- Browse examples in C:\HashLang\examples\
- Read docs in C:\HashLang\docs\
- Create programs anywhere
- Run `hash --setup` later if you change your mind

### Q: Can I customize the workspace location?

**A:** The wizard offers Desktop or Documents. For custom locations:
```powershell
mkdir C:\Your\Custom\Path
cd C:\Your\Custom\Path
# Copy examples manually if desired
```

### Q: Does it work without VS Code?

**A:** Yes! The wizard:
- Detects if VS Code is installed
- Skips VS Code steps if not found
- Still creates workspace
- Still runs tutorial
- You can use any editor

---

##  Best Practices

### For First-Time Users

1. **Run the full wizard** - See everything Hash offers
2. **Choose starter pack** - 4 examples is perfect
3. **Do the tutorial** - Only 2 minutes!
4. **Open in VS Code** - Best experience
5. **Try Hello World** - See it work!

### For Returning Users

1. **Skip welcome** - `.\setup-wizard.ps1 -SkipWelcome`
2. **Create new workspace** - For new projects
3. **Copy specific examples** - Only what you need
4. **Skip tutorial** - You know Hash already

---

## Success Metrics

What a successful wizard run looks like:

 All installation checks pass
 Workspace created
 Examples copied
 VS Code opens workspace
 Tutorial completed
 Hello World runs successfully
 User knows next steps

**Result:** User is productive in < 10 minutes!

---

##  Final Words

The Setup Wizard transforms the first-run experience from:

**Before:**
- "What do I do now?"
- "Where are the examples?"
- "How do I run a program?"
- Searching documentation

**After:**
- Working workspace in 5 minutes
- Examples ready to run
- Tutorial completed
- First program executed
- Clear next steps

**It's like having a friendly guide welcoming you to Hash!**

---

## Support

### Wizard Issues?

If the wizard doesn't work:

```powershell
# Check Hash is installed
hash --version

# Check wizard exists
Test-Path .\setup-wizard.ps1

# Run with verbose output (future feature)
# .\setup-wizard.ps1 -Verbose
```

### Still Stuck?

1. Read [INSTALLATION.md](INSTALLATION.md)
2. Check [POST_INSTALL_CHECKLIST.md](POST_INSTALL_CHECKLIST.md)
3. Skip wizard and set up manually
4. Read [GETTING_STARTED.md](GETTING_STARTED.md)

---

**Setup Wizard** - Making Hash welcoming for everyone!

*Run it with: `hash --setup`*
