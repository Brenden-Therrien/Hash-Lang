# Hash Language Build Script for Windows
# This script automates the build process

param(
    [switch]$Clean,
    [switch]$Release,
    [switch]$Debug,
    [string]$LLVMDir = "",
    [switch]$Help
)

function Show-Help {
    Write-Host "Hash Language Build Script" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Usage: .\build.ps1 [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Clean          Clean build directory before building"
    Write-Host "  -Release        Build in Release mode (default)"
    Write-Host "  -Debug          Build in Debug mode"
    Write-Host "  -LLVMDir <path> Specify LLVM installation directory"
    Write-Host "  -Help           Show this help message"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\build.ps1                                    # Build in Release mode"
    Write-Host "  .\build.ps1 -Clean -Release                    # Clean and build Release"
    Write-Host "  .\build.ps1 -Debug                             # Build in Debug mode"
    Write-Host "  .\build.ps1 -LLVMDir 'C:\Program Files\LLVM'  # Specify LLVM path"
}

if ($Help) {
    Show-Help
    exit 0
}

# Determine build type
$BuildType = "Release"
if ($Debug) {
    $BuildType = "Debug"
}

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Hash Language Compiler Build Script  " -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check for CMake
Write-Host "Checking for CMake..." -ForegroundColor Yellow
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    Write-Host "ERROR: CMake not found. Please install CMake." -ForegroundColor Red
    Write-Host "Download from: https://cmake.org/" -ForegroundColor Red
    exit 1
}
Write-Host "✓ CMake found: $($cmake.Version)" -ForegroundColor Green
Write-Host ""

# Set LLVM directory if provided
if ($LLVMDir -ne "") {
    $env:LLVM_DIR = Join-Path $LLVMDir "lib\cmake\llvm"
    Write-Host "Using LLVM from: $LLVMDir" -ForegroundColor Yellow
    Write-Host ""
}

# Create build directory
$BuildDir = "build"
if ($Clean -and (Test-Path $BuildDir)) {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $BuildDir
    Write-Host "✓ Build directory cleaned" -ForegroundColor Green
    Write-Host ""
}

if (-not (Test-Path $BuildDir)) {
    Write-Host "Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
    Write-Host "✓ Build directory created" -ForegroundColor Green
    Write-Host ""
}

# Configure
Write-Host "Configuring CMake ($BuildType)..." -ForegroundColor Yellow
Push-Location $BuildDir

$ConfigResult = cmake .. -DCMAKE_BUILD_TYPE=$BuildType
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Common issues:" -ForegroundColor Yellow
    Write-Host "  1. LLVM not found - install LLVM or specify path with -LLVMDir" -ForegroundColor Yellow
    Write-Host "  2. Missing C++ compiler - install Visual Studio with C++ tools" -ForegroundColor Yellow
    Write-Host ""
    Pop-Location
    exit 1
}
Write-Host "✓ CMake configured successfully" -ForegroundColor Green
Write-Host ""

# Build
Write-Host "Building Hash compiler..." -ForegroundColor Yellow
$BuildResult = cmake --build . --config $BuildType
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "ERROR: Build failed!" -ForegroundColor Red
    Pop-Location
    exit 1
}
Write-Host "✓ Build completed successfully" -ForegroundColor Green
Write-Host ""

Pop-Location

# Show executable location
$ExePath = Join-Path $BuildDir "$BuildType\hashc.exe"
if (Test-Path $ExePath) {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "  Build Successful!  " -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Compiler location: $ExePath" -ForegroundColor Green
    Write-Host ""
} else {
    Write-Host "Warning: Executable not found at expected location" -ForegroundColor Yellow
}

# Install VS Code Extension
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Installing VS Code Extension          " -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$extensionsPath = "$env:USERPROFILE\.vscode\extensions"
$sourcePath = Join-Path $PSScriptRoot ".vscode\extensions\hash-lang"
$destPath = Join-Path $extensionsPath "hash-lang-1.0.0"

if (Test-Path $sourcePath) {
    # Create extensions directory if it doesn't exist
    if (-not (Test-Path $extensionsPath)) {
        Write-Host "Creating VS Code extensions directory..." -ForegroundColor Yellow
        New-Item -ItemType Directory -Path $extensionsPath -Force | Out-Null
    }
    
    # Remove old version if exists
    if (Test-Path $destPath) {
        Write-Host "Removing old version..." -ForegroundColor Yellow
        Remove-Item -Path $destPath -Recurse -Force
    }
    
    # Copy extension files
    Write-Host "Installing Hash Language extension..." -ForegroundColor Green
    Copy-Item -Path $sourcePath -Destination $destPath -Recurse -Force
    
    Write-Host "✓ VS Code extension installed!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Features:" -ForegroundColor Magenta
    Write-Host "  • Syntax highlighting with vibrant colors" -ForegroundColor White
    Write-Host "  • 'pure' keyword in BRIGHT PINK (bold)" -ForegroundColor Magenta
    Write-Host "  • 'pure_local' keyword in ORANGE (bold)" -ForegroundColor Yellow
    Write-Host "  • All 50 built-in functions recognized" -ForegroundColor Cyan
    Write-Host "  • IntelliSense and code completion" -ForegroundColor Cyan
    Write-Host "  • Auto-indentation and bracket pairing" -ForegroundColor Cyan
    Write-Host ""
} else {
    Write-Host "VS Code extension source not found, skipping..." -ForegroundColor Yellow
    Write-Host ""
}

Write-Host "========================================" -ForegroundColor Green
Write-Host "  Setup Complete!                       " -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "  1. Restart VS Code (if open)" -ForegroundColor White
Write-Host "  2. Open any .hash file for syntax highlighting" -ForegroundColor White
Write-Host "  3. Test the compiler: .\$ExePath examples\00_hello.hash" -ForegroundColor White
Write-Host "  4. Explore 23 examples in examples\ directory" -ForegroundColor White
Write-Host ""
