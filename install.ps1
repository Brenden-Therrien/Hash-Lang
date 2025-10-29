#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Install Hash Language system-wide

.DESCRIPTION
    Installs Hash Language compiler and CLI to a system location
    and adds it to the user's PATH for global access.

.EXAMPLE
    .\install.ps1
    Install Hash to default location (C:\HashLang)

.EXAMPLE
    .\install.ps1 -InstallPath "C:\Program Files\Hash"
    Install to custom location
#>

param(
    [string]$InstallPath = "C:\HashLang",
    [switch]$SkipVSCode,
    [switch]$Uninstall
)

$ErrorActionPreference = "Stop"

# Colors
function Write-InstallHeader {
    Write-Host ""
    Write-Host "═══════════════════════════════════════════════════" -ForegroundColor Cyan
    Write-Host "  Hash Language Installer" -ForegroundColor Magenta
    Write-Host "═══════════════════════════════════════════════════" -ForegroundColor Cyan
    Write-Host ""
}

function Write-InstallSuccess {
    param([string]$Message)
    Write-Host "✓ " -ForegroundColor Green -NoNewline
    Write-Host $Message -ForegroundColor White
}

function Write-InstallError {
    param([string]$Message)
    Write-Host "✗ " -ForegroundColor Red -NoNewline
    Write-Host $Message -ForegroundColor Red
}

function Write-InstallInfo {
    param([string]$Message)
    Write-Host "→ " -ForegroundColor Cyan -NoNewline
    Write-Host $Message -ForegroundColor White
}

function Write-InstallWarning {
    param([string]$Message)
    Write-Host "! " -ForegroundColor Yellow -NoNewline
    Write-Host $Message -ForegroundColor Yellow
}

function Test-Administrator {
    $currentUser = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($currentUser)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

function Add-ToUserPath {
    param([string]$PathToAdd)
    
    # Get current user PATH
    $currentPath = [Environment]::GetEnvironmentVariable("Path", "User")
    
    # Check if already in PATH
    if ($currentPath -split ';' | Where-Object { $_ -eq $PathToAdd }) {
        Write-InstallInfo "Already in PATH: $PathToAdd"
        return $true
    }
    
    # Add to PATH
    $newPath = "$currentPath;$PathToAdd"
    [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
    
    # Update current session
    $env:Path = "$env:Path;$PathToAdd"
    
    Write-InstallSuccess "Added to PATH: $PathToAdd"
    return $true
}

function Remove-FromUserPath {
    param([string]$PathToRemove)
    
    $currentPath = [Environment]::GetEnvironmentVariable("Path", "User")
    $pathArray = $currentPath -split ';' | Where-Object { $_ -ne $PathToRemove -and $_ -ne "" }
    $newPath = $pathArray -join ';'
    
    [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
    Write-InstallSuccess "Removed from PATH: $PathToRemove"
}

function Install-Hash {
    Write-InstallHeader
    Write-Host "Installing Hash Language to: " -NoNewline -ForegroundColor White
    Write-Host $InstallPath -ForegroundColor Cyan
    Write-Host ""
    
    # Check if compiler is built
    if (-not (Test-Path "build\Release\hashc.exe")) {
        Write-InstallError "Compiler not found! Building now..."
        Write-InstallInfo "Running build script..."
        
        try {
            & .\build.ps1
            if ($LASTEXITCODE -ne 0) {
                Write-InstallError "Build failed!"
                exit 1
            }
        } catch {
            Write-InstallError "Build error: $_"
            exit 1
        }
    }
    
    Write-InstallSuccess "Compiler found"
    
    # Create install directory
    Write-InstallInfo "Creating installation directory..."
    if (-not (Test-Path $InstallPath)) {
        New-Item -ItemType Directory -Path $InstallPath -Force | Out-Null
    }
    Write-InstallSuccess "Installation directory ready"
    
    # Create bin directory
    $binPath = Join-Path $InstallPath "bin"
    if (-not (Test-Path $binPath)) {
        New-Item -ItemType Directory -Path $binPath -Force | Out-Null
    }
    
    # Copy compiler
    Write-InstallInfo "Installing compiler..."
    Copy-Item "build\Release\hashc.exe" -Destination (Join-Path $binPath "hashc.exe") -Force
    Write-InstallSuccess "Compiler installed"
    
    # Create hash.ps1 wrapper in bin
    Write-InstallInfo "Installing Hash CLI..."
    $cliContent = Get-Content "hash.ps1" -Raw
    # Update the compiler path in the CLI
    $cliContent = $cliContent -replace 'build\\Release\\hashc\.exe', (Join-Path $binPath "hashc.exe")
    $cliContent | Set-Content -Path (Join-Path $binPath "hash.ps1") -Force
    Write-InstallSuccess "Hash CLI installed"
    
    # Create hash.cmd wrapper for direct 'hash' command
    Write-InstallInfo "Creating 'hash' command..."
    $cmdWrapper = @"
@echo off
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "$binPath\hash.ps1" %*
"@
    $cmdWrapper | Set-Content -Path (Join-Path $binPath "hash.cmd") -Force
    Write-InstallSuccess "'hash' command created"
    
    # Copy examples (optional)
    $examplesPath = Join-Path $InstallPath "examples"
    if (Test-Path "examples") {
        Write-InstallInfo "Installing examples..."
        if (-not (Test-Path $examplesPath)) {
            New-Item -ItemType Directory -Path $examplesPath -Force | Out-Null
        }
        Copy-Item "examples\*" -Destination $examplesPath -Force -Recurse
        Write-InstallSuccess "Examples installed"
    }
    
    # Copy documentation
    $docsPath = Join-Path $InstallPath "docs"
    if (Test-Path "docs") {
        Write-InstallInfo "Installing documentation..."
        if (-not (Test-Path $docsPath)) {
            New-Item -ItemType Directory -Path $docsPath -Force | Out-Null
        }
        Copy-Item "docs\*" -Destination $docsPath -Force -Recurse
        Write-InstallSuccess "Documentation installed"
    }
    
    # Add to PATH
    Write-InstallInfo "Adding Hash to system PATH..."
    Add-ToUserPath -PathToAdd $binPath
    
    # Install VS Code extension
    if (-not $SkipVSCode) {
        Write-InstallInfo "Installing VS Code extension..."
        
        $vsCodeExtPath = Join-Path $env:USERPROFILE ".vscode\extensions"
        if (Test-Path $vsCodeExtPath) {
            $hashExtPath = Join-Path $vsCodeExtPath "hash-lang-1.0.0"
            
            # Remove old version
            if (Test-Path $hashExtPath) {
                Remove-Item $hashExtPath -Recurse -Force
            }
            
            # Copy extension
            Copy-Item ".vscode\extensions\hash-lang" -Destination $hashExtPath -Recurse -Force
            Write-InstallSuccess "VS Code extension installed"
        } else {
            Write-InstallWarning "VS Code extensions folder not found, skipping extension install"
        }
    }
    
    # Installation complete
    Write-Host ""
    Write-Host "═══════════════════════════════════════════════════" -ForegroundColor Green
    Write-Host "  Installation Complete!" -ForegroundColor Green
    Write-Host "═══════════════════════════════════════════════════" -ForegroundColor Green
    Write-Host ""
    
    Write-Host "Hash has been installed to: " -ForegroundColor White -NoNewline
    Write-Host $InstallPath -ForegroundColor Cyan
    Write-Host ""
    
    Write-Host "IMPORTANT: " -ForegroundColor Yellow -NoNewline
    Write-Host "Restart your terminal for PATH changes to take effect!" -ForegroundColor White
    Write-Host ""
    
    Write-Host "Try these commands in a " -ForegroundColor White -NoNewline
    Write-Host "NEW" -ForegroundColor Yellow -NoNewline
    Write-Host " terminal:" -ForegroundColor White
    Write-Host "  hash --version" -ForegroundColor Cyan
    Write-Host "  hash --help" -ForegroundColor Cyan
    Write-Host "  hash -cr $InstallPath\examples\00_hello.hash" -ForegroundColor Cyan
    Write-Host ""
    
    Write-Host "VS Code Extension:" -ForegroundColor White
    if (-not $SkipVSCode) {
        Write-Host "  ✓ Installed! Restart VS Code to activate" -ForegroundColor Green
        Write-Host "  • Syntax highlighting with PINK 'pure' and ORANGE 'pure_local'" -ForegroundColor White
        Write-Host "  • 30+ IntelliSense snippets (type 'main' + Tab)" -ForegroundColor White
        Write-Host "  • Auto-indentation and bracket pairing" -ForegroundColor White
    } else {
        Write-Host "  ⊘ Skipped (use without -SkipVSCode to install)" -ForegroundColor Yellow
    }
    Write-Host ""
    
    Write-Host "Documentation:" -ForegroundColor White
    Write-Host "  $InstallPath\docs\INSTALLATION.md" -ForegroundColor Cyan
    Write-Host "  $InstallPath\docs\POST_INSTALL_CHECKLIST.md" -ForegroundColor Cyan
    Write-Host "  $InstallPath\docs\GETTING_STARTED.md" -ForegroundColor Cyan
    Write-Host "  $InstallPath\docs\CLI_GUIDE.md" -ForegroundColor Cyan
    Write-Host ""
    
    Write-Host "Next Steps:" -ForegroundColor White
    Write-Host "  1. Close and reopen your terminal" -ForegroundColor Cyan
    Write-Host "  2. Run: " -ForegroundColor White -NoNewline
    Write-Host "hash --version" -ForegroundColor Yellow
    Write-Host "  3. (Optional) Run: " -ForegroundColor White -NoNewline
    Write-Host ".\setup-wizard.ps1" -ForegroundColor Yellow -NoNewline
    Write-Host " for interactive setup" -ForegroundColor White
    Write-Host ""
    
    # Ask if they want to run the setup wizard now
    Write-Host "═══════════════════════════════════════════════════" -ForegroundColor Magenta
    Write-Host ""
    Write-Host "Would you like to run the interactive setup wizard now?" -ForegroundColor White
    Write-Host "  (Helps you set up workspace, VS Code, and run your first program)" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  Y - Yes, let's do it!" -ForegroundColor Green
    Write-Host "  N - No, I'll set up later" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  Enter choice (Y/N): " -ForegroundColor Cyan -NoNewline
    $wizardChoice = Read-Host
    
    Write-Host ""
    
    if ($wizardChoice -eq "Y" -or $wizardChoice -eq "y") {
        Write-Host "Launching setup wizard..." -ForegroundColor Cyan
        Write-Host ""
        Start-Sleep -Seconds 1
        
        # Run the setup wizard
        if (Test-Path "$PSScriptRoot\setup-wizard.ps1") {
            & "$PSScriptRoot\setup-wizard.ps1"
        } else {
            Write-Host "⚠ Setup wizard not found at $PSScriptRoot\setup-wizard.ps1" -ForegroundColor Yellow
            Write-Host "  You can run it later from the Hash directory" -ForegroundColor Gray
        }
    } else {
        Write-Host "Setup wizard skipped. You can run it anytime with:" -ForegroundColor Gray
        Write-Host "  .\setup-wizard.ps1" -ForegroundColor Yellow
        Write-Host ""
        Write-Host "Happy Hashing! 🚀" -ForegroundColor Magenta
        Write-Host ""
    }
}

function Uninstall-Hash {
    Write-InstallHeader
    Write-Host "Uninstalling Hash Language from: " -NoNewline -ForegroundColor White
    Write-Host $InstallPath -ForegroundColor Cyan
    Write-Host ""
    
    # Confirm
    $confirm = Read-Host "Are you sure you want to uninstall Hash? (yes/no)"
    if ($confirm -ne "yes") {
        Write-InstallInfo "Uninstall cancelled"
        exit 0
    }
    
    # Remove from PATH
    $binPath = Join-Path $InstallPath "bin"
    Write-InstallInfo "Removing from PATH..."
    Remove-FromUserPath -PathToRemove $binPath
    
    # Remove VS Code extension
    Write-InstallInfo "Removing VS Code extension..."
    $vsCodeExtPath = Join-Path $env:USERPROFILE ".vscode\extensions\hash-lang-1.0.0"
    if (Test-Path $vsCodeExtPath) {
        Remove-Item $vsCodeExtPath -Recurse -Force
        Write-InstallSuccess "VS Code extension removed"
    }
    
    # Remove installation directory
    Write-InstallInfo "Removing installation directory..."
    if (Test-Path $InstallPath) {
        Remove-Item $InstallPath -Recurse -Force
        Write-InstallSuccess "Installation directory removed"
    }
    
    Write-Host ""
    Write-Host "═══════════════════════════════════════════════════" -ForegroundColor Yellow
    Write-Host "  Hash Language Uninstalled" -ForegroundColor Yellow
    Write-Host "═══════════════════════════════════════════════════" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Hash has been removed from your system." -ForegroundColor White
    Write-Host "Restart your terminal for PATH changes to take effect." -ForegroundColor White
    Write-Host ""
}

# ═══════════════════════════════════════════════════════
# Main
# ═══════════════════════════════════════════════════════

if ($Uninstall) {
    Uninstall-Hash
} else {
    Install-Hash
}
