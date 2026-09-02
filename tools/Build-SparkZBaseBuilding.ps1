<#
.SYNOPSIS
    Packs every mod folder in this repo into a signed, ready-to-load DayZ
    mod structure (@ModName/addons/ModName.pbo + keys), the same way the
    output can be dropped straight into a DayZ client's or server's mod
    folder, or referenced via a -mod= launch parameter.

.DESCRIPTION
    A mod folder is anything at the repo root containing a mod.cpp. Today
    that's SparkZCore, SparkZGroup, and SparkZBase (the old BBP-hooked
    prototype, kept as reference - see docs/BRIEF.md). Once the new
    standalone construction mod exists as its own folder in this repo, it
    will be picked up automatically the same way - no change to this
    script should be needed.

    Requires DayZ Tools to be installed (Steam > Tools > DayZ Tools) and a
    signing key pair. Generate your own key pair once via DayZ Tools'
    DSCreateKey.exe if you don't already have one - NEVER commit the
    .biprivatekey to git. Share it with your collaborator over a private
    channel (Discord DM, etc.), not through this repo.

.PARAMETER DayZToolsPath
    Root of the DayZ Tools installation.

.PARAMETER PrivateKeyPath
    Path to your .biprivatekey signing key.

.PARAMETER PublicKeyPath
    Path to the matching .bikey (distributed alongside the built mods so a
    server/client can verify the signature).

.PARAMETER OutputRoot
    Where to write the built @ModName folders. Defaults to a timestamped
    folder under this repo (gitignored - never commit build output).

.EXAMPLE
    ./tools/Build-SparkZBaseBuilding.ps1 `
        -DayZToolsPath "C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools" `
        -PrivateKeyPath "C:\path\to\YourKey.biprivatekey" `
        -PublicKeyPath "C:\path\to\YourKey.bikey"
#>
param(
    [string]$DayZToolsPath = "C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools",
    [Parameter(Mandatory = $true)]
    [string]$PrivateKeyPath,
    [Parameter(Mandatory = $true)]
    [string]$PublicKeyPath,
    [string]$OutputRoot
)

$ErrorActionPreference = "Stop"
$repoRoot = Split-Path -Parent $PSScriptRoot
$fileBank = Join-Path $DayZToolsPath "Bin\PboUtils\FileBank.exe"
$signTool = Join-Path $DayZToolsPath "Bin\DsUtils\DSSignFile.exe"

foreach ($path in @($fileBank, $signTool, $PrivateKeyPath, $PublicKeyPath)) {
    if (!(Test-Path -LiteralPath $path)) {
        throw "Missing required build file: $path"
    }
}

if (-not $OutputRoot) {
    $stamp = Get-Date -Format "yyyyMMdd_HHmmss"
    $OutputRoot = Join-Path $repoRoot ("_build_output_" + $stamp)
}

$keyFileName = Split-Path -Leaf $PublicKeyPath

# Auto-detect every mod folder: anything at repo root with a mod.cpp.
$modDirs = Get-ChildItem -LiteralPath $repoRoot -Directory |
    Where-Object { Test-Path -LiteralPath (Join-Path $_.FullName "mod.cpp") }

if ($modDirs.Count -eq 0) {
    throw "No mod folders found (looked for a mod.cpp under each top-level directory in $repoRoot)"
}

Write-Host "Found mods: $($modDirs.Name -join ', ')"
New-Item -ItemType Directory -Force -Path $OutputRoot | Out-Null

foreach ($modDir in $modDirs) {
    $mod = $modDir.Name
    $source = $modDir.FullName
    $modOut = Join-Path $OutputRoot ("@" + $mod)
    $addonsOut = Join-Path $modOut "addons"
    $keysOut = Join-Path $modOut "keys"

    Write-Host ""
    Write-Host "Building $mod ..."
    New-Item -ItemType Directory -Force -Path $addonsOut, $keysOut | Out-Null
    Copy-Item -LiteralPath (Join-Path $source "mod.cpp") -Destination (Join-Path $modOut "mod.cpp") -Force
    Copy-Item -LiteralPath $PublicKeyPath -Destination (Join-Path $keysOut $keyFileName) -Force

    & $fileBank -property "product=dayz ugc" -property "prefix=$mod" -exclude "*.bak" -exclude "*.tmp" -exclude "mod.cpp" -dst $addonsOut $source | Out-Host

    $pboPath = Join-Path $addonsOut ($mod + ".pbo")
    if (!(Test-Path -LiteralPath $pboPath)) {
        throw "PBO was not created for $mod : $pboPath"
    }

    & $signTool $PrivateKeyPath $pboPath | Out-Host

    $bisignPath = Get-ChildItem -LiteralPath $addonsOut -Filter "*.bisign" | Select-Object -First 1
    if (-not $bisignPath) {
        throw "BISIGN was not created for $mod"
    }

    Write-Host "  -> $modOut"
}

Write-Host ""
Write-Host "Done. Load order matters - see README.md / docs/CONTRIBUTING.md for the required mod line order."
Write-Host "OUTPUT=$OutputRoot"
