# Changelog

Dated entries, newest first. Bump `SparkZBaseConstants.MOD_VERSION` alongside
any entry that ships to the test server (see `docs/CONTRIBUTING.md`).

## 2026-09-02 — Pivot: standalone construction system, no BaseBuildingPlus dependency

**This supersedes the "hooks into BaseBuildingPlus" framing used everywhere
below and in the docs written earlier today.** Decision: `SparkZBaseBuilding`
will implement its **own** construction system from scratch — own part
classes (foundations, walls, floors, roofs, doors, windows, stairs), own
placement/build/dismantle actions, own persistence for built structures.
`requiredAddons[]` will **not** include `BaseBuildingPlus` in the final mod.

Why: the original ask was for a mod with genuinely different functionality,
not a permission layer bolted onto an existing workshop mod. The
BaseBuildingPlus install referenced earlier (Steam Workshop ID
`1710977250`) was pointed to purely as an example of *how a base-building
mod's addon is generally structured* (part categories, kit-based
build-then-upgrade flow, tiered walls/doors, lock compatibility, raid
balance) — no code or assets from it were or will be copied; it's also
distributed as a compiled/packed PBO so there was nothing to copy even if
we'd wanted to (see `docs/BRIEF.md`).

Status of the existing `SparkZBase/` folder in this repo: it is the
**previous, BBP-dependent implementation** (squad claims/upkeep/permissions
hooked onto BBP's own actions). It is kept as a **reference/prototype only**
— its permission-model ideas (claims, upkeep, Leader/Manager/Access roles)
are the part worth carrying forward into the new system; its actual hook
code (everything in `SparkZBase/scripts/4_World/SparkZBase/BBP/`,
`.../CodeLock/`, `.../Storage/`) will not be. See `docs/BRIEF.md` for the
new project's scope and `docs/FEATURES.md` for what's being carried over
vs. rebuilt.

`SparkZGroup/` and `SparkZCore/` are unaffected by this pivot — the squad/
permission model and shared RPC infrastructure are still exactly what the
new construction system will integrate with.

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
