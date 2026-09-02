# Changelog

Dated entries, newest first. Bump `SparkZBaseConstants.MOD_VERSION` alongside
any entry that ships to the test server (see `docs/CONTRIBUTING.md`).

## 2026-09-02 — Repo setup & documentation

- Moved SparkZBase into its own standalone repo (`SparkZBaseBuilding`),
  separate from the wider SparkZ modpack, to build out as its own project
  with squad-based base-building functionality distinct from
  BaseBuildingPlus and other workshop base mods.
- Added `docs/ARCHITECTURE.md`, `docs/CODING_STANDARDS.md`,
  `docs/CONTRIBUTING.md`, `docs/FEATURES.md` documenting the current
  implementation, real bugs already hit and their fixes, and the workflow
  for two people/AI sessions collaborating on this codebase.

## Prior history (from the SparkZ modpack, current `0.3.3-STORAGE-DISMANTLE-FIX`)

Summarized from the modpack's development history; individual dated commits
were not preserved when this repo was split out.

- Squad-scoped base claims, upkeep billing, and abandonment implemented.
- BBP build/dismantle, CodeLock, and SparkZStorage integrations hooked in.
- `RequireClaimForBBPBuild` config flag added (default: claim required).
- Storage dismantle permission fixed to bypass the storage mod's native
  single-owner check, so any squad member with Base Access can dismantle
  squad-owned storage (previously blocked everyone but the original placer).
- Give Up Base leader-check hardened against a group-payload/role-flag
  desync.
- `SparkZBaseBreachCompat.c` integration attempt disabled after its guessed
  door/window class names broke breaching-charge placement entirely; BBP's
  own `BBP_DisableDestroy` setting already covers native tool-based raiding.
