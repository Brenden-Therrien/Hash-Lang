# Hash Language - Interactive Setup Wizard
# Welcome new users and help them get started!

param(
    [switch]$SkipWelcome
)

# Color functions
function Write-WizardHeader {
    Write-Host ""
    Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Magenta
    Write-Host "              Welcome to Hash Language!" -ForegroundColor Magenta
    Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Magenta
    Write-Host ""
}

function Write-WizardStep {
    param([string]$Message)
    Write-Host "→ " -ForegroundColor Cyan -NoNewline
    Write-Host $Message -ForegroundColor White
}

function Write-WizardSuccess {
    param([string]$Message)
    Write-Host "✓ " -ForegroundColor Green -NoNewline
    Write-Host $Message -ForegroundColor White
}

function Write-WizardInfo {
    param([string]$Message)
    Write-Host "  " -NoNewline
    Write-Host $Message -ForegroundColor Gray
}

function Write-WizardQuestion {
    param([string]$Message)
    Write-Host ""
    Write-Host "? " -ForegroundColor Yellow -NoNewline
    Write-Host $Message -ForegroundColor White
}

function Write-WizardChoice {
    param([string]$Number, [string]$Message)
    Write-Host "  " -NoNewline
    Write-Host "$Number. " -ForegroundColor Cyan -NoNewline
    Write-Host $Message -ForegroundColor White
}

function Show-WizardSeparator {
    Write-Host ""
    Write-Host "───────────────────────────────────────────────────────────────" -ForegroundColor Gray
    Write-Host ""
}

# Get Hash installation path
$HashPath = "C:\HashLang"
if (-not (Test-Path "$HashPath\bin\hashc.exe")) {
    $HashPath = $PSScriptRoot
}

# Welcome screen
if (-not $SkipWelcome) {
    Clear-Host
    Write-WizardHeader
    
    Write-Host "  Thanks for installing Hash Language!" -ForegroundColor White
    Write-Host "  Let's get you set up in just a few quick steps." -ForegroundColor White
    Write-Host ""
    Write-Host "  This wizard will help you:" -ForegroundColor Gray
    Write-Host "    • Verify your installation" -ForegroundColor Gray
    Write-Host "    • Set up your workspace" -ForegroundColor Gray
    Write-Host "    • Configure VS Code (optional)" -ForegroundColor Gray
    Write-Host "    • Run your first program" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  Press any key to begin..." -ForegroundColor Cyan
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
}

Clear-Host
Write-WizardHeader

# Step 1: Verify Installation
Write-Host "STEP 1: Verifying Installation" -ForegroundColor Magenta
Write-Host ""

Write-WizardStep "Checking Hash compiler..."
if (Test-Path "$HashPath\bin\hashc.exe") {
    Write-WizardSuccess "Compiler found at $HashPath\bin\hashc.exe"
} else {
    Write-Host "✗ Compiler not found!" -ForegroundColor Red
    Write-Host "  Please run .\install.ps1 first" -ForegroundColor Yellow
    exit 1
}

Write-WizardStep "Checking 'hash' command..."
$hashCmd = Get-Command hash -ErrorAction SilentlyContinue
if ($hashCmd) {
    Write-WizardSuccess "'hash' command is globally available"
} else {
    Write-Host "! 'hash' command not found in PATH" -ForegroundColor Yellow
    Write-WizardInfo "You may need to restart your terminal"
}

Write-WizardStep "Checking examples..."
$exampleCount = (Get-ChildItem "$HashPath\examples\*.hash" -ErrorAction SilentlyContinue | Measure-Object).Count
if ($exampleCount -gt 0) {
    Write-WizardSuccess "$exampleCount example programs found"
} else {
    Write-Host "! Examples not found" -ForegroundColor Yellow
}

Write-WizardStep "Checking documentation..."
$docCount = (Get-ChildItem "$HashPath\docs\*.md" -ErrorAction SilentlyContinue | Measure-Object).Count
if ($docCount -gt 0) {
    Write-WizardSuccess "$docCount documentation files found"
} else {
    Write-Host "! Documentation not found" -ForegroundColor Yellow
}

Show-WizardSeparator

# Step 2: Choose Examples to Copy
Write-Host "STEP 2: Set Up Your Workspace" -ForegroundColor Magenta
Write-Host ""

Write-WizardQuestion "Would you like to create a workspace folder with starter examples?"
Write-WizardChoice "1" "Yes, create workspace on my Desktop"
Write-WizardChoice "2" "Yes, create workspace in Documents"
Write-WizardChoice "3" "No, I'll set up my own workspace"
Write-Host ""
Write-Host "  Enter choice (1-3): " -ForegroundColor Cyan -NoNewline
$workspaceChoice = Read-Host

$workspacePath = $null

if ($workspaceChoice -eq "1") {
    $workspacePath = "$env:USERPROFILE\Desktop\HashProjects"
} elseif ($workspaceChoice -eq "2") {
    $workspacePath = "$env:USERPROFILE\Documents\HashProjects"
}

if ($workspacePath) {
    Write-Host ""
    Write-WizardStep "Creating workspace at $workspacePath..."
    
    if (-not (Test-Path $workspacePath)) {
        New-Item -ItemType Directory -Path $workspacePath -Force | Out-Null
        Write-WizardSuccess "Workspace created"
    } else {
        Write-WizardSuccess "Using existing workspace"
    }
    
    # Ask which examples to copy
    Write-Host ""
    Write-WizardQuestion "Which examples would you like to copy to your workspace?"
    Write-WizardChoice "1" "Just Hello World (00_hello.hash)"
    Write-WizardChoice "2" "Starter pack (Hello World, Basics, Functions)"
    Write-WizardChoice "3" "All 23 examples"
    Write-WizardChoice "4" "None, I'll explore them in $HashPath\examples\"
    Write-Host ""
    Write-Host "  Enter choice (1-4): " -ForegroundColor Cyan -NoNewline
    $exampleChoice = Read-Host
    
    Write-Host ""
    
    switch ($exampleChoice) {
        "1" {
            Copy-Item "$HashPath\examples\00_hello.hash" -Destination $workspacePath -Force
            Write-WizardSuccess "Copied 00_hello.hash"
        }
        "2" {
            @("00_hello.hash", "01_basics.hash", "03_functions.hash", "04_pure_functions.hash") | ForEach-Object {
                Copy-Item "$HashPath\examples\$_" -Destination $workspacePath -Force
                Write-WizardSuccess "Copied $_"
            }
        }
        "3" {
            Get-ChildItem "$HashPath\examples\*.hash" | ForEach-Object {
                Copy-Item $_.FullName -Destination $workspacePath -Force
            }
            Write-WizardSuccess "Copied all 23 examples"
        }
        "4" {
            Write-WizardInfo "Skipping example copy"
        }
    }
}

Show-WizardSeparator

# Step 3: VS Code Setup
Write-Host "STEP 3: VS Code Configuration" -ForegroundColor Magenta
Write-Host ""

$vscodeInstalled = Get-Command code -ErrorAction SilentlyContinue
if ($vscodeInstalled) {
    Write-WizardSuccess "VS Code is installed"
    
    # Check if Hash extension is installed
    $hashExtPath = "$env:USERPROFILE\.vscode\extensions\hash-lang-1.0.0"
    if (Test-Path $hashExtPath) {
        Write-WizardSuccess "Hash extension is installed"
    } else {
        Write-Host "! Hash extension not found" -ForegroundColor Yellow
        Write-WizardInfo "Run .\install.ps1 to install the extension"
    }
    
    if ($workspacePath) {
        Write-Host ""
        Write-WizardQuestion "Would you like to open your workspace in VS Code now?"
        Write-WizardChoice "1" "Yes, open in VS Code"
        Write-WizardChoice "2" "No, I'll open it later"
        Write-Host ""
        Write-Host "  Enter choice (1-2): " -ForegroundColor Cyan -NoNewline
        $vscodeChoice = Read-Host
        
        if ($vscodeChoice -eq "1") {
            Write-Host ""
            Write-WizardStep "Opening VS Code..."
            & code $workspacePath
            Write-WizardSuccess "VS Code opened"
            Write-WizardInfo "The Hash extension will activate when you open a .hash file"
        }
    }
} else {
    Write-Host "! VS Code not found" -ForegroundColor Yellow
    Write-WizardInfo "Install VS Code from https://code.visualstudio.com"
    Write-WizardInfo "Then run .\install.ps1 to install the Hash extension"
}

Show-WizardSeparator

# Step 4: Quick Tutorial
Write-Host "STEP 4: Quick Tutorial" -ForegroundColor Magenta
Write-Host ""

Write-WizardQuestion "Would you like a quick tutorial on using Hash?"
Write-WizardChoice "1" "Yes, show me the basics!"
Write-WizardChoice "2" "No, I'll read the docs myself"
Write-Host ""
Write-Host "  Enter choice (1-2): " -ForegroundColor Cyan -NoNewline
$tutorialChoice = Read-Host

if ($tutorialChoice -eq "1") {
    Clear-Host
    Write-WizardHeader
    Write-Host "QUICK TUTORIAL: Hash in 2 Minutes" -ForegroundColor Magenta
    Write-Host ""
    
    # Tutorial Section 1: Basic Command
    Write-Host "1. USING THE HASH COMMAND" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "  The main command you'll use is:" -ForegroundColor White
    Write-Host "    hash -cr myprogram.hash" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "  This compiles AND runs your program in one step!" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  Other useful commands:" -ForegroundColor White
    Write-Host "    hash --help      " -ForegroundColor Yellow -NoNewline
    Write-Host "# Show all commands" -ForegroundColor Gray
    Write-Host "    hash --version   " -ForegroundColor Yellow -NoNewline
    Write-Host "# Show version info" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  Press any key to continue..." -ForegroundColor Cyan
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    
    # Tutorial Section 2: Basic Syntax
    Clear-Host
    Write-WizardHeader
    Write-Host "QUICK TUTORIAL: Hash in 2 Minutes" -ForegroundColor Magenta
    Write-Host ""
    Write-Host "2. BASIC SYNTAX" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "  Hash uses Python-like syntax with strong typing:" -ForegroundColor White
    Write-Host ""
    Write-Host "  fn main() -> i32:" -ForegroundColor Yellow
    Write-Host "      let x: i32 = 42" -ForegroundColor Yellow
    Write-Host "      print_i32(x)" -ForegroundColor Yellow
    Write-Host "      println()" -ForegroundColor Yellow
    Write-Host "      return 0" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "  Key points:" -ForegroundColor White
    Write-Host "    • Use 4 spaces for indentation (no tabs!)" -ForegroundColor Gray
    Write-Host "    • Colons : start code blocks" -ForegroundColor Gray
    Write-Host "    • main() function must return i32" -ForegroundColor Gray
    Write-Host "    • Strong typing: let x: i32 = 42" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  Press any key to continue..." -ForegroundColor Cyan
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    
    # Tutorial Section 3: VS Code Snippets
    Clear-Host
    Write-WizardHeader
    Write-Host "QUICK TUTORIAL: Hash in 2 Minutes" -ForegroundColor Magenta
    Write-Host ""
    Write-Host "3. VS CODE MAGIC" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "  In VS Code, type these shortcuts + Tab:" -ForegroundColor White
    Write-Host ""
    Write-Host "    main" -ForegroundColor Yellow -NoNewline
    Write-Host " + Tab  →  Complete main function template" -ForegroundColor Gray
    Write-Host "    fn" -ForegroundColor Yellow -NoNewline
    Write-Host "   + Tab  →  Function template" -ForegroundColor Gray
    Write-Host "    pure" -ForegroundColor Yellow -NoNewline
    Write-Host " + Tab  →  Pure function template" -ForegroundColor Gray
    Write-Host "    if" -ForegroundColor Yellow -NoNewline
    Write-Host "   + Tab  →  If statement" -ForegroundColor Gray
    Write-Host "    while" -ForegroundColor Yellow -NoNewline
    Write-Host "+ Tab  →  While loop" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  This saves tons of time!" -ForegroundColor White
    Write-Host ""
    Write-Host "  Press any key to continue..." -ForegroundColor Cyan
    $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    
    # Tutorial Section 4: Try It Now
    Clear-Host
    Write-WizardHeader
    Write-Host "QUICK TUTORIAL: Hash in 2 Minutes" -ForegroundColor Magenta
    Write-Host ""
    Write-Host "4. TRY IT NOW!" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "  Let's run your first Hash program!" -ForegroundColor White
    Write-Host ""
    Write-WizardQuestion "Would you like to run the Hello World example?"
    Write-WizardChoice "1" "Yes, let's do it!"
    Write-WizardChoice "2" "No, I'll try it later"
    Write-Host ""
    Write-Host "  Enter choice (1-2): " -ForegroundColor Cyan -NoNewline
    $runChoice = Read-Host
    
    if ($runChoice -eq "1") {
        Write-Host ""
        Write-WizardStep "Running 00_hello.hash..."
        Write-Host ""
        Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Gray
        
        # Run the example
        if ($hashCmd) {
            & hash -cr "$HashPath\examples\00_hello.hash"
        } else {
            # Fallback if hash command not in PATH
            & "$HashPath\bin\hash.ps1" -cr "$HashPath\examples\00_hello.hash"
        }
        
        Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Gray
        Write-Host ""
        Write-WizardSuccess "Congratulations! You just ran your first Hash program!"
        Write-Host ""
        Write-Host "  Press any key to continue..." -ForegroundColor Cyan
        $null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
    }
}

Show-WizardSeparator

# Step 5: What's Next
Clear-Host
Write-WizardHeader
Write-Host "SETUP COMPLETE!" -ForegroundColor Magenta
Write-Host ""

Write-Host "  You're all set up and ready to use Hash!" -ForegroundColor Green
Write-Host ""

Write-Host "WHAT'S NEXT?" -ForegroundColor Cyan
Write-Host ""

if ($workspacePath) {
    Write-Host "Your Workspace:" -ForegroundColor White
    Write-Host "  $workspacePath" -ForegroundColor Yellow
    Write-Host ""
}

Write-Host "Quick Commands:" -ForegroundColor White
Write-Host "  hash --help              " -ForegroundColor Yellow -NoNewline
Write-Host "# Show all commands" -ForegroundColor Gray
Write-Host "  hash --version           " -ForegroundColor Yellow -NoNewline
Write-Host "# Check installation" -ForegroundColor Gray
if ($workspacePath) {
    Write-Host "  cd $workspacePath" -ForegroundColor Yellow
    Write-Host "  hash -cr 00_hello.hash   " -ForegroundColor Yellow -NoNewline
    Write-Host "# Compile and run" -ForegroundColor Gray
} else {
    Write-Host "  hash -cr C:\HashLang\examples\00_hello.hash" -ForegroundColor Yellow
}
Write-Host ""

Write-Host "Learning Resources:" -ForegroundColor White
Write-Host "  1. Read WHATS_NEXT.md for project ideas" -ForegroundColor Gray
Write-Host "  2. Check QUICK_START_CARD.md for syntax reference" -ForegroundColor Gray
Write-Host "  3. Browse all 23 examples in C:\HashLang\examples\" -ForegroundColor Gray
Write-Host "  4. Read docs in C:\HashLang\docs\" -ForegroundColor Gray
Write-Host ""

Write-Host "Recommended First Steps:" -ForegroundColor White
Write-Host "  1. " -ForegroundColor Cyan -NoNewline
Write-Host "Try the examples: hash -cr <example>.hash" -ForegroundColor White
Write-Host "  2. " -ForegroundColor Cyan -NoNewline
Write-Host "Read GETTING_STARTED.md" -ForegroundColor White
Write-Host "  3. " -ForegroundColor Cyan -NoNewline
Write-Host "Create your first program in VS Code" -ForegroundColor White
Write-Host "  4. " -ForegroundColor Cyan -NoNewline
Write-Host "Print QUICK_REFERENCE.md for your desk" -ForegroundColor White
Write-Host ""

Write-WizardQuestion "Would you like to open the documentation now?"
Write-WizardChoice "1" "Open WHATS_NEXT.md (project ideas)"
Write-WizardChoice "2" "Open GETTING_STARTED.md (language tutorial)"
Write-WizardChoice "3" "Open QUICK_REFERENCE.md (cheat sheet)"
Write-WizardChoice "4" "No thanks, I'm ready to code!"
Write-Host ""
Write-Host "  Enter choice (1-4): " -ForegroundColor Cyan -NoNewline
$docChoice = Read-Host

Write-Host ""

switch ($docChoice) {
    "1" {
        if (Test-Path "$HashPath\WHATS_NEXT.md") {
            if ($vscodeInstalled) {
                & code "$HashPath\WHATS_NEXT.md"
                Write-WizardSuccess "Opened WHATS_NEXT.md in VS Code"
            } else {
                & notepad "$HashPath\WHATS_NEXT.md"
                Write-WizardSuccess "Opened WHATS_NEXT.md in Notepad"
            }
        }
    }
    "2" {
        if (Test-Path "$HashPath\docs\GETTING_STARTED.md") {
            if ($vscodeInstalled) {
                & code "$HashPath\docs\GETTING_STARTED.md"
                Write-WizardSuccess "Opened GETTING_STARTED.md in VS Code"
            } else {
                & notepad "$HashPath\docs\GETTING_STARTED.md"
                Write-WizardSuccess "Opened GETTING_STARTED.md in Notepad"
            }
        }
    }
    "3" {
        if (Test-Path "$HashPath\docs\QUICK_REFERENCE.md") {
            if ($vscodeInstalled) {
                & code "$HashPath\docs\QUICK_REFERENCE.md"
                Write-WizardSuccess "Opened QUICK_REFERENCE.md in VS Code"
            } else {
                & notepad "$HashPath\docs\QUICK_REFERENCE.md"
                Write-WizardSuccess "Opened QUICK_REFERENCE.md in Notepad"
            }
        }
    }
}

Write-Host ""
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Magenta
Write-Host ""
Write-Host "              Happy Hashing!" -ForegroundColor Magenta
Write-Host ""
Write-Host "         Start building something amazing!" -ForegroundColor White
Write-Host ""
Write-Host "═══════════════════════════════════════════════════════════════" -ForegroundColor Magenta
Write-Host ""
