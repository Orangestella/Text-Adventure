param([string]$Compiler = "")

$ErrorActionPreference = "Stop"
if (-not $Compiler) {
    $portableCompiler = Join-Path $PSScriptRoot "..\.tools\mingw64\bin\g++.exe"
    $Compiler = if (Test-Path -LiteralPath $portableCompiler) { $portableCompiler } else { "g++" }
}
Push-Location $PSScriptRoot
try {
    New-Item -ItemType Directory -Force build | Out-Null
    & $Compiler -std=c++17 -Wall -Wextra -Wpedantic -Werror -g -O0 -static `
        main.cpp State.cpp Room.cpp GameObject.cpp wordwrap.cpp -o build/text-adventure.exe
    if ($LASTEXITCODE -ne 0) { throw "Compilation failed." }
    Write-Output "Built build/text-adventure.exe"
} finally {
    Pop-Location
}
