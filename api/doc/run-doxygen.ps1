# Generate Board API Doxygen HTML (run once before committing doc/API changes).
# Usage:  .\run-doxygen.ps1
# Exit 1 if Doxygen is missing or generation fails.

$ErrorActionPreference = 'Stop'
$docDir = $PSScriptRoot
$doxyfile = Join-Path $docDir 'Doxyfile'

function Find-Doxygen {
    if ($env:DOXYGEN_EXECUTABLE -and (Test-Path $env:DOXYGEN_EXECUTABLE)) {
        return $env:DOXYGEN_EXECUTABLE
    }
    $cmd = Get-Command doxygen -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    $portable = 'C:\tmp\doxygen-portable\doxygen.exe'
    if (Test-Path $portable) { return $portable }
    return $null
}

$doxygen = Find-Doxygen
if (-not $doxygen) {
    Write-Error @"
Doxygen not found. Install via winget/choco or set DOXYGEN_EXECUTABLE.
  winget install DimitriVanHeesch.Doxygen
  choco install doxygen.install
"@
    exit 1
}

Push-Location $docDir
try {
    & $doxygen $doxyfile
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    Write-Host "OK: $docDir\html\index.html"
} finally {
    Pop-Location
}
