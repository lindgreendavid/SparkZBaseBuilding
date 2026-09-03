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

.PARAMETER OnlyMods
    Optional list of mod folder names to build (e.g. -OnlyMods SparkZBaseBuilding).
    Omit to build every detected mod folder.

.EXAMPLE
    ./tools/Build-SparkZBaseBuilding.ps1 `
        -DayZToolsPath "C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools" `
        -PrivateKeyPath "C:\path\to\YourKey.biprivatekey" `
        -PublicKeyPath "C:\path\to\YourKey.bikey" `
        -OnlyMods SparkZBaseBuilding
#>
param(
    [string]$DayZToolsPath = "C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools",
    [Parameter(Mandatory = $true)]
    [string]$PrivateKeyPath,
    [Parameter(Mandatory = $true)]
    [string]$PublicKeyPath,
    [string]$OutputRoot,
    [string[]]$OnlyMods
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

if ($OnlyMods) {
    $modDirs = $modDirs | Where-Object { $OnlyMods -contains $_.Name }
}

if ($modDirs.Count -eq 0) {
    throw "No matching mod folders found (looked for a mod.cpp under each top-level directory in $repoRoot)"
}

Write-Host "Found mods: $($modDirs.Name -join ', ')"
New-Item -ItemType Directory -Force -Path $OutputRoot | Out-Null

$stagingRoot = Join-Path $OutputRoot "_staging"

foreach ($modDir in $modDirs) {
    $mod = $modDir.Name
    $source = $modDir.FullName
    $modOut = Join-Path $OutputRoot ("@" + $mod)
    $addonsOut = Join-Path $modOut "addons"
    $keysOut = Join-Path $modOut "keys"
    $staged = Join-Path $stagingRoot $mod

    Write-Host ""
    Write-Host "Building $mod ..."
    New-Item -ItemType Directory -Force -Path $addonsOut, $keysOut | Out-Null
    Copy-Item -LiteralPath (Join-Path $source "mod.cpp") -Destination (Join-Path $modOut "mod.cpp") -Force
    Copy-Item -LiteralPath $PublicKeyPath -Destination (Join-Path $keysOut $keyFileName) -Force

    # FileBank's own -exclude flag does not reliably match these patterns, so
    # stage a filtered copy first (guaranteed exclusion) and pack that
    # instead of the real source tree. Excludes match docs/WORKFLOW.md's
    # release-packing rule: ship P3D, PAA, RVMAT, BISURF,
    # config/scripts/layouts/inputs and the prefix - never Source/ (editable
    # Blender files), PNG/PSD source textures, or dev/backup files.
    if (Test-Path -LiteralPath $staged) {
        Remove-Item -LiteralPath $staged -Recurse -Force
    }
    New-Item -ItemType Directory -Force -Path $staged | Out-Null
    $robocopyArgs = @(
        $source, $staged, "/E",
        "/XD", "Source",
        "/XF", "*.blend", "*.blend1", "*.blend2", "*.png", "*.psd", "*.bak", "*.tmp", "mod.cpp"
    )
    robocopy @robocopyArgs | Out-Null
    if ($LASTEXITCODE -ge 8) {
        throw "robocopy failed staging $mod for packing (exit code $LASTEXITCODE)"
    }

    & $fileBank -property "product=dayz ugc" -property "prefix=$mod" -dst $addonsOut $staged | Out-Host

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

if (Test-Path -LiteralPath $stagingRoot) {
    Remove-Item -LiteralPath $stagingRoot -Recurse -Force
}

Write-Host ""
Write-Host "Done. Load order matters - see README.md / docs/CONTRIBUTING.md for the required mod line order."
Write-Host "OUTPUT=$OutputRoot"
