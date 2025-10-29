# Hash Language - VS Code Extension Installer
# Installs custom syntax highlighting with vibrant colors

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Hash Language Extension Installer    " -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Get VS Code extensions directory
$extensionsPath = "$env:USERPROFILE\.vscode\extensions"

# Create extensions directory if it doesn't exist
if (-not (Test-Path $extensionsPath)) {
    Write-Host "Creating VS Code extensions directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $extensionsPath -Force | Out-Null
}

# Source and destination paths
$sourcePath = Join-Path $PSScriptRoot ".vscode\extensions\hash-lang"
$destPath = Join-Path $extensionsPath "hash-lang-1.0.0"

# Remove old version if exists
if (Test-Path $destPath) {
    Write-Host "Removing old version..." -ForegroundColor Yellow
    Remove-Item -Path $destPath -Recurse -Force
}

# Copy extension files
Write-Host "Installing Hash Language extension..." -ForegroundColor Green
Copy-Item -Path $sourcePath -Destination $destPath -Recurse -Force

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "  Installation Complete! ✓             " -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "1. Restart VS Code" -ForegroundColor White
Write-Host "2. Open any .hash file" -ForegroundColor White
Write-Host "3. (Optional) Set color theme: Ctrl+K Ctrl+T → 'Hash Vibrant'" -ForegroundColor White
Write-Host ""
Write-Host "Special Features:" -ForegroundColor Magenta
Write-Host "  • 'pure' keyword highlighted in PINK (bold)" -ForegroundColor Magenta
Write-Host "  • 'pure_local' keyword highlighted in ORANGE (bold)" -ForegroundColor Yellow
Write-Host "  • All 50 built-in functions recognized" -ForegroundColor Cyan
Write-Host "  • Custom vibrant color scheme" -ForegroundColor Cyan
Write-Host ""
Write-Host "Extension installed to:" -ForegroundColor Gray
Write-Host $destPath -ForegroundColor White
Write-Host ""
