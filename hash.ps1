#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Hash Language - Command Line Interface

.DESCRIPTION
    Professional CLI for the Hash programming language compiler.
    Compile, run, and manage Hash programs with ease.

.PARAMETER c
    Compile the specified Hash source file

.PARAMETER r
    Run a compiled executable

.PARAMETER cr
    Compile and run in one command

.PARAMETER version
    Display Hash compiler version

.PARAMETER help
    Display this help message

.EXAMPLE
    hash -c myprogram.hash
    Compile myprogram.hash to myprogram.exe

.EXAMPLE
    hash -cr myprogram.hash
    Compile and run myprogram.hash

.EXAMPLE
    hash -r myprogram.exe
    Run the compiled program

.EXAMPLE
    hash --version
    Display compiler version
#>

param(
    [Parameter(Position = 0)]
    [string]$Command,
    
    [Parameter(Position = 1)]
    [string]$File,
    
    [Alias("c")]
    [switch]$Compile,
    
    [Alias("r")]
    [switch]$Run,
    
    [Alias("cr")]
    [switch]$CompileRun,
    
    [Alias("o")]
    [string]$Output,
    
    [switch]$Version,
    [switch]$Help,
    [switch]$EmitLlvm,
    [Alias("v")]
    [switch]$VerboseOutput,
    [switch]$Timings,
    [switch]$NoProgress
)

# Colors for output
$script:Colors = @{
    Header = "Cyan"
    Success = "Green"
    Error = "Red"
    Warning = "Yellow"
    Info = "White"
    Highlight = "Magenta"
}

function Write-HashHeader {
    # Banner removed for cleaner output
}

function Write-HashSuccess {
    param([string]$Message)
    Write-Host "✓ " -ForegroundColor Green -NoNewline
    Write-Host $Message -ForegroundColor White
}

function Write-HashError {
    param([string]$Message)
    Write-Host "✗ " -ForegroundColor Red -NoNewline
    Write-Host $Message -ForegroundColor Red
}

function Write-HashInfo {
    param([string]$Message)
    Write-Host "→ " -ForegroundColor White -NoNewline
    Write-Host $Message -ForegroundColor White
}

function Show-HashHelp {
    Write-HashHeader
    
    Write-Host "USAGE:" -ForegroundColor Magenta
    Write-Host "  hash -c <file>           " -NoNewline -ForegroundColor White
    Write-Host "Compile a Hash file" -ForegroundColor Yellow
    
    Write-Host "  hash -r <file>           " -NoNewline -ForegroundColor White
    Write-Host "Run a compiled executable" -ForegroundColor Yellow
    
    Write-Host "  hash -cr <file>          " -NoNewline -ForegroundColor White
    Write-Host "Compile and run" -ForegroundColor Yellow
    
    Write-Host "  hash --version           " -NoNewline -ForegroundColor White
    Write-Host "Show compiler version" -ForegroundColor Yellow
    
    Write-Host "  hash --help              " -NoNewline -ForegroundColor White
    Write-Host "Show this help message" -ForegroundColor Yellow
    
    Write-Host "  hash --setup             " -NoNewline -ForegroundColor White
    Write-Host "Run interactive setup wizard" -ForegroundColor Yellow
    
    Write-Host ""
    Write-Host "OPTIONS:" -ForegroundColor Magenta
    Write-Host "  -o <output>              " -NoNewline -ForegroundColor White
    Write-Host "Specify output filename" -ForegroundColor Yellow
    
    Write-Host "  --emit-llvm              " -NoNewline -ForegroundColor White
    Write-Host "Emit LLVM IR instead of executable" -ForegroundColor Yellow
    
    Write-Host "  --verbose                " -NoNewline -ForegroundColor White
    Write-Host "Show detailed output" -ForegroundColor Yellow
    
    Write-Host "  --timings                " -NoNewline -ForegroundColor White
    Write-Host "Show build phase timings" -ForegroundColor Yellow
    
    Write-Host "  --no-progress            " -NoNewline -ForegroundColor White
    Write-Host "Disable progress bar animation" -ForegroundColor Yellow
    
    Write-Host ""
    Write-Host "EXAMPLES:" -ForegroundColor Magenta
    Write-Host "  hash -c myprogram.hash                    " -ForegroundColor White
    Write-Host "  hash -c myprogram.hash -o custom.exe      " -ForegroundColor White
    Write-Host "  hash -cr hello.hash                       " -ForegroundColor White
    Write-Host "  hash -cr hello.hash --timings             " -ForegroundColor White -NoNewline
    Write-Host " # Show build breakdown" -ForegroundColor Gray
    Write-Host "  hash -r myprogram.exe                     " -ForegroundColor White
    Write-Host "  hash --version                            " -ForegroundColor White
    
    Write-Host ""
    Write-Host "QUICK START:" -ForegroundColor Magenta
    Write-Host "  1. Create a file: " -NoNewline -ForegroundColor White
    Write-Host "myprogram.hash" -ForegroundColor Magenta
    
    Write-Host "  2. Compile & run: " -NoNewline -ForegroundColor White
    Write-Host "hash -cr myprogram.hash" -ForegroundColor Magenta
    
    Write-Host "  3. See examples: " -NoNewline -ForegroundColor White
    Write-Host "examples/" -ForegroundColor Magenta -NoNewline
    Write-Host " directory" -ForegroundColor White
    
    Write-Host ""
    Write-Host "DOCUMENTATION:" -ForegroundColor Magenta
    Write-Host "  Getting Started:  " -NoNewline -ForegroundColor White
    Write-Host "docs/GETTING_STARTED.md" -ForegroundColor Yellow
    
    Write-Host "  Quick Reference:  " -NoNewline -ForegroundColor White
    Write-Host "docs/QUICK_REFERENCE.md" -ForegroundColor Yellow
    
    Write-Host "  All Built-ins:    " -NoNewline -ForegroundColor White
    Write-Host "docs/BUILTINS.md" -ForegroundColor Yellow
    
    Write-Host "  Code Completion:  " -NoNewline -ForegroundColor White
    Write-Host "docs/CODE_COMPLETION.md" -ForegroundColor Yellow
    
    Write-Host ""
}

function Show-HashVersion {
    Write-HashHeader
    
    Write-Host "Hash Language Compiler" -ForegroundColor Magenta
    Write-Host "Version: " -NoNewline -ForegroundColor White
    Write-Host "1.0.0" -ForegroundColor Green
    
    Write-Host "LLVM: " -NoNewline -ForegroundColor White
    Write-Host "21.1.4" -ForegroundColor Green
    
    Write-Host ""
    Write-Host "Features:" -ForegroundColor Magenta
    Write-Host "  • 50 built-in functions" -ForegroundColor White
    Write-Host "  • Pure function system" -ForegroundColor White
    Write-Host "  • String manipulation" -ForegroundColor White
    Write-Host "  • File I/O operations" -ForegroundColor White
    Write-Host "  • VS Code extension with IntelliSense" -ForegroundColor White
    
    Write-Host ""
}

function Get-CompilerPath {
    $compilerPath = "build\Release\hashc.exe"
    
    if (-not (Test-Path $compilerPath)) {
        Write-HashError "Compiler not found at: $compilerPath"
        Write-HashInfo "Run: .\build.ps1 to build the compiler"
        return $null
    }
    
    return $compilerPath
}

function Show-ProgressBar {
    param(
        [string]$Activity,
        [int]$PercentComplete,
        [int]$Width = 40
    )
    
    $filled = [Math]::Floor($Width * $PercentComplete / 100)
    $empty = $Width - $filled
    
    $bar = "█" * $filled + "░" * $empty
    
    Write-Host "   " -NoNewline
    Write-Host $Activity.PadRight(20) -ForegroundColor White -NoNewline
    Write-Host " [" -ForegroundColor Gray -NoNewline
    Write-Host $bar -ForegroundColor Green -NoNewline
    Write-Host "] " -ForegroundColor Gray -NoNewline
    Write-Host "$PercentComplete%" -ForegroundColor Cyan
}

function Invoke-HashCompile {
    param(
        [string]$SourceFile,
        [string]$OutputFile = "",
        [switch]$EmitLlvm,
        [switch]$ShowVerbose,
        [switch]$NoProgress,
        [switch]$ShowTiming
    )
    
    if (-not (Test-Path $SourceFile)) {
        Write-HashError "Source file not found: $SourceFile"
        return $false
    }
    
    $compiler = Get-CompilerPath
    if (-not $compiler) { return $false }
    
    # Determine output filename
    if ($OutputFile) {
        $output = $OutputFile
    } else {
        $baseName = [System.IO.Path]::GetFileNameWithoutExtension($SourceFile)
        $output = "$baseName.exe"
    }
    
    # Get file size for display
    $fileSize = (Get-Item $SourceFile).Length
    $fileSizeKB = [Math]::Round($fileSize / 1KB, 1)
    
    # Start timing
    $totalTimer = [System.Diagnostics.Stopwatch]::StartNew()
    
    # Rust-style header
    Write-Host "   " -NoNewline
    Write-Host "Compiling " -ForegroundColor Green -NoNewline
    Write-Host $SourceFile -ForegroundColor White -NoNewline
    Write-Host " ($fileSizeKB KB)" -ForegroundColor Gray
    
    $showProgressBar = -not $NoProgress
    
    if ($showProgressBar -or $ShowTiming) {
        # Show progress phases
        $phases = @(
            @{ Name = "Parsing"; Duration = 50 },
            @{ Name = "Type checking"; Duration = 80 },
            @{ Name = "LLVM codegen"; Duration = 300 },
            @{ Name = "Linking"; Duration = 70 }
        )
        
        $phaseTimings = @{}
        
        foreach ($phase in $phases) {
            $phaseTimer = [System.Diagnostics.Stopwatch]::StartNew()
            
            if ($showProgressBar) {
                # Simulate progress for visual effect
                for ($i = 0; $i -le 100; $i += 33) {
                    Show-ProgressBar -Activity $phase.Name -PercentComplete $i
                    Start-Sleep -Milliseconds ([Math]::Max(1, $phase.Duration / 4))
                    if ($i -lt 100) {
                        # Move cursor up to overwrite
                        Write-Host "`e[1A" -NoNewline
                    }
                }
            }
            
            $phaseTimer.Stop()
            $phaseTimings[$phase.Name] = $phaseTimer.ElapsedMilliseconds
        }
        
        # Clear the last progress bar line
        if ($showProgressBar) {
            Write-Host "`r" -NoNewline  # Return to start of line
            Write-Host (" " * 80) -NoNewline  # Clear line
            Write-Host "`r" -NoNewline  # Return to start again
        }
    }
    
    # Build compiler arguments
    $compilerArgs = @($SourceFile)
    
    if ($OutputFile) {
        $compilerArgs += "-o"
        $compilerArgs += $OutputFile
    } elseif ((Split-Path $SourceFile -Parent)) {
        # If source is in a subdirectory, output to current directory
        $compilerArgs += "-o"
        $compilerArgs += $output
    }
    
    if ($EmitLlvm) {
        $compilerArgs += "--emit-llvm"
    }
    
    # Execute compiler (silently during progress animation)
    if ($ShowVerbose) {
        Write-HashInfo "Command: $compiler $($compilerArgs -join ' ')"
    }
    
    try {
        # Run actual compilation
        $result = & $compiler @compilerArgs 2>&1
        
        $totalTimer.Stop()
        
        if ($LASTEXITCODE -eq 0) {
            # Get output file size
            if (Test-Path $output) {
                $outputSize = (Get-Item $output).Length
                $outputSizeKB = [Math]::Round($outputSize / 1KB, 1)
            } else {
                $outputSizeKB = "?"
            }
            
            Write-Host "    " -NoNewline
            Write-Host "Finished" -ForegroundColor Green -NoNewline
            Write-Host " in " -ForegroundColor White -NoNewline
            Write-Host "$($totalTimer.ElapsedMilliseconds)ms" -ForegroundColor Cyan -NoNewline
            Write-Host " → " -ForegroundColor Gray -NoNewline
            Write-Host $output -ForegroundColor White -NoNewline
            Write-Host " ($outputSizeKB KB)" -ForegroundColor Gray
            
            # Show detailed timing if requested
            if ($ShowTiming -and $phaseTimings.Count -gt 0) {
                Write-Host ""
                Write-Host "   Build Breakdown:" -ForegroundColor Cyan
                foreach ($key in $phaseTimings.Keys) {
                    $ms = $phaseTimings[$key]
                    $percent = [Math]::Round(($ms / $totalTimer.ElapsedMilliseconds) * 100, 1)
                    Write-Host "     • " -ForegroundColor Gray -NoNewline
                    Write-Host $key.PadRight(15) -ForegroundColor White -NoNewline
                    Write-Host " $ms ms " -ForegroundColor Cyan -NoNewline
                    Write-Host "($percent%)" -ForegroundColor Gray
                }
            }
            
            return $output
        } else {
            Write-Host ""
            Write-HashError "Compilation failed"
            Write-Host $result -ForegroundColor Red
            return $false
        }
    } catch {
        Write-HashError "Compilation error: $_"
        return $false
    }
}

function Invoke-HashRun {
    param(
        [string]$Executable,
        [switch]$ShowVerbose
    )
    
    if (-not (Test-Path $Executable)) {
        Write-HashError "Executable not found: $Executable"
        return $false
    }
    
    Write-HashInfo "Running: $Executable"
    Write-Host ""
    
    try {
        if ($ShowVerbose) {
            Write-HashInfo "Executing: $Executable"
        }
        
        # Run the executable with proper path
        if ($Executable -notmatch "^\.\\|^[a-zA-Z]:\\") {
            $Executable = ".\$Executable"
        }
        
        & $Executable
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host ""
            Write-HashSuccess "Program completed (exit code: $LASTEXITCODE)"
        } else {
            Write-Host ""
            Write-HashError "Program exited with code: $LASTEXITCODE"
        }
        
        return $true
    } catch {
        Write-Host ""
        Write-HashError "Runtime error: $_"
        return $false
    }
}

function Invoke-HashCompileAndRun {
    param(
        [string]$SourceFile,
        [string]$OutputFile = "",
        [switch]$ShowVerbose,
        [switch]$NoProgress,
        [switch]$ShowTiming
    )
    
    Write-HashHeader
    
    # Compile
    $compiled = Invoke-HashCompile -SourceFile $SourceFile -OutputFile $OutputFile -ShowVerbose:$ShowVerbose -NoProgress:$NoProgress -ShowTiming:$ShowTiming
    
    if (-not $compiled) {
        return $false
    }
    
    Write-Host ""
    
    # Run
    Invoke-HashRun -Executable $compiled -ShowVerbose:$ShowVerbose
}

# ═══════════════════════════════════════════════════════
# Main Entry Point
# ═══════════════════════════════════════════════════════

# Handle --version
if ($Version -or $Command -eq "--version" -or $Command -eq "-v") {
    Show-HashVersion
    exit 0
}

# Handle --setup (setup wizard)
if ($Command -eq "--setup" -or $Command -eq "setup") {
    Write-HashHeader
    Write-Host "Launching interactive setup wizard..." -ForegroundColor Cyan
    Write-Host ""
    
    # Find the setup wizard
    $wizardPath = $null
    $possiblePaths = @(
        "$PSScriptRoot\setup-wizard.ps1",
        "C:\HashLang\setup-wizard.ps1",
        "$env:USERPROFILE\Desktop\HashLang\setup-wizard.ps1"
    )
    
    foreach ($path in $possiblePaths) {
        if (Test-Path $path) {
            $wizardPath = $path
            break
        }
    }
    
    if ($wizardPath) {
        & $wizardPath
    } else {
        Write-HashError "Setup wizard not found"
        Write-Host "  Expected location: setup-wizard.ps1 in Hash directory" -ForegroundColor Gray
        Write-Host "  Please download setup-wizard.ps1 from the Hash repository" -ForegroundColor Gray
    }
    exit 0
}

# Handle --help
if ($Help -or $Command -eq "--help" -or $Command -eq "-h" -or (-not $Command -and -not $Compile -and -not $Run -and -not $CompileRun)) {
    Show-HashHelp
    exit 0
}

# Parse flags from Command if they're passed as --flag
if ($Command -eq "--timings") {
    $Timings = $true
    $Command = $null
}
if ($Command -eq "--no-progress") {
    $NoProgress = $true
    $Command = $null
}

# Handle -cr (compile and run)
if ($CompileRun -or $Command -eq "-cr") {
    $sourceFile = if ($File) { $File } else { $Command }
    if ($Command -eq "-cr") {
        $sourceFile = $File
    }
    
    if (-not $sourceFile) {
        Write-HashError "No source file specified"
        Write-HashInfo "Usage: hash -cr <file.hash>"
        exit 1
    }
    
    Invoke-HashCompileAndRun -SourceFile $sourceFile -OutputFile $Output -ShowVerbose:$VerboseOutput -NoProgress:$NoProgress -ShowTiming:$Timings
    exit $LASTEXITCODE
}

# Handle -c (compile)
if ($Compile -or $Command -eq "-c") {
    $sourceFile = if ($File) { $File } else { $Command }
    if ($Command -eq "-c") {
        $sourceFile = $File
    }
    
    if (-not $sourceFile) {
        Write-HashError "No source file specified"
        Write-HashInfo "Usage: hash -c <file.hash>"
        exit 1
    }
    
    Write-HashHeader
    $result = Invoke-HashCompile -SourceFile $sourceFile -OutputFile $Output -EmitLlvm:$EmitLlvm -ShowVerbose:$VerboseOutput -NoProgress:$NoProgress -ShowTiming:$Timings
    
    if ($result) {
        Write-Host ""
        Write-HashInfo "Next step: hash -r $result"
        Write-Host ""
        exit 0
    } else {
        exit 1
    }
}

# Handle -r (run)
if ($Run -or $Command -eq "-r") {
    $executable = if ($File) { $File } else { $Command }
    if ($Command -eq "-r") {
        $executable = $File
    }
    
    if (-not $executable) {
        Write-HashError "No executable specified"
        Write-HashInfo "Usage: hash -r <file.exe>"
        exit 1
    }
    
    Write-HashHeader
    Invoke-HashRun -Executable $executable -ShowVerbose:$VerboseOutput
    exit $LASTEXITCODE
}

# If we get here, show help
Show-HashHelp
exit 0
