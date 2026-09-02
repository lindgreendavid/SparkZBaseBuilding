<#
.SYNOPSIS
    Re-syncs the SparkZGroup and SparkZCore source snapshots in this repo from
    the main SparkZ modpack, so this repo never silently goes stale relative
    to the shared squad/RPC system those two mods provide.

.DESCRIPTION
    SparkZGroup and SparkZCore live in this repo only as a working snapshot -
    they are also used by other SparkZ mods outside this project (SparkZKOTH,
    SparkZAirdrop, etc.) and their real source of truth is the main modpack
    checkout. Whenever either mod changes there, run this script, review the
    diff, and commit it here with an entry in CHANGELOG.md (see
    docs/CONTRIBUTING.md "Cross-repo sync").

.PARAMETER ModpackRoot
    Path to the root of the main SparkZ_Modpack checkout (the folder that
    directly contains SparkZGroup/ and SparkZCore/).

.EXAMPLE
    ./tools/sync-dependencies.ps1 -ModpackRoot "C:\Users\dave7\Desktop\SparkZ_Modpack"
#>
param(
    [Parameter(Mandatory = $true)]
    [string]$ModpackRoot
)

$ErrorActionPreference = "Stop"
$repoRoot = Split-Path -Parent $PSScriptRoot

$mods = @("SparkZGroup", "SparkZCore")
$excludeDirs = @("addons", ".claude")
$excludeFiles = @("*.pbo", "*.bikey", "*.bisign", "*.biprivatekey")

foreach ($mod in $mods) {
    $source = Join-Path $ModpackRoot $mod
    $dest = Join-Path $repoRoot $mod

    if (-not (Test-Path $source)) {
        Write-Warning "Skipping $mod - not found at $source"
        continue
    }

    Write-Host "Syncing $mod from $source ..."
    $robocopyArgs = @($source, $dest, "/E", "/MIR", "/XD") + $excludeDirs + @("/XF") + $excludeFiles
    robocopy @robocopyArgs | Out-Null

    # robocopy exit codes 0-7 are all "success" (8+ indicates a real failure)
    if ($LASTEXITCODE -ge 8) {
        throw "robocopy failed syncing $mod (exit code $LASTEXITCODE)"
    }
}

Write-Host ""
Write-Host "Sync complete. Now:"
Write-Host "  1. git status / git diff to review what actually changed"
Write-Host "  2. Add a dated entry to CHANGELOG.md describing what changed in SparkZGroup/SparkZCore and why"
Write-Host "  3. If the change affects a class SparkZBase calls into, update docs/ARCHITECTURE.md's dependency section"
Write-Host "  4. Commit and push"
