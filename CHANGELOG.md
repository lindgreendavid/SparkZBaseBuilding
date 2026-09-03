# Changelog

## 2026-09-03 — Fix another duplicate-declaration compile error, audit whole codebase

- Fix `Multiple declaration of variable 'response'` in `SPKZ_Workbench.OnRPC`
  - two `if`/`else if` branches each declared a `response` local of a
  different type. Same root cause as the earlier `cost`/`clearIndex` fixes,
  but confirms the rule is broader than just `for` loops: **any** local
  variable is scoped to the whole method, not the `if`/`for`/nested block
  it's declared in. Expanded Pitfall #7 in `docs/CODING_STANDARDS.md`
  accordingly.
- Manually audited every method in every file added for the workbench
  feature for the same pattern; no further instances found.

## 2026-09-03 — Fix Mission-module compile error: World/Mission can't cross-reference custom classes

- Fix `Unknown type 'SPKZ_Workbench'` compiling the Mission module -
  `SPKZ_WorkbenchNetworking.c` tried to `modded class SPKZ_Workbench` from
  `5_Mission`, but `4_World` and `5_Mission` are sibling modules that cannot
  reference each other's own newly-declared classes at all (only pre-
  existing vanilla classes, which are globally known regardless of tier).
  Documented as Pitfall #8 in `docs/CODING_STANDARDS.md`.
- Fix: added `SPKZ_WorkbenchClientBridge` (3_Game - visible to both tiers),
  using only the universally-known `Object` type at its connection points.
  `SPKZ_Workbench` (World) writes into it; `SPKZ_WorkbenchMenu` (Mission)
  polls it via `Update()`, and the existing `MissionGameplay.OnUpdate` in
  `SPKZ_PlacementLegend.c` polls it to open the menu - the same
  poll-don't-call-into pattern that file already used successfully for
  `Hologram` state. Deleted the now-obsolete `SPKZ_WorkbenchNetworking.c`.
- `SPKZ_WorkbenchMenu` now stores its workbench reference as `Object`
  instead of the concrete `SPKZ_Workbench` type, since Mission cannot name
  that type either.

## 2026-09-03 — Fix real compile error found by first in-game test

- Fix `Multiple declaration of variable 'cost'` compile error in
  `SPKZ_Workbench.c` (two separate `for` loops in `SPKZ_HandleBuildRequest`
  both declared a loop variable named `cost`) - Enfusion Script's `for`
  variables are function-scoped, not block-scoped like C/C++; see new
  Pitfall #7 in `docs/CODING_STANDARDS.md`. Also fixed the same pattern
  (`clearIndex` declared twice in `SPKZ_RefreshDetailPanel`) found by
  auditing the rest of the new code for the same mistake.
- Sharpening-stone classname corrected from the placeholder
  `SPKZ_SharpeningStone` to the real vanilla item `Whetstone` (confirmed
  against the installed game's `gear_tools.pbo`) - the tools (Hammer,
  Pliers, Hacksaw, etc.) were already real vanilla classnames.
- Simplified all seeded recipe material costs to a flat 1-of-each (one log,
  one plank, one nail, etc.) instead of tuned quantities, for easier testing
  - per direction, exact costs are still a placeholder to retune later.

## 2026-09-03 — Workbench tools, nails, sharpening stone (not yet in-game tested)

- Recipes can now require tools (`SPKZ_WorkbenchToolRequirement`): present
  and non-ruined, not consumed, takes a flat health-point loss per craft.
  Seed data varies the tool per recipe (Hammer/Pliers/Hacksaw) to
  demonstrate the system, pending real tool-to-recipe design.
- Nail added as a required material to every seeded recipe.
- Placeholder sharpening-stone mechanic: an item of class
  `SPKZ_SharpeningStone` (doesn't exist yet) anywhere in the workbench fully
  offsets tool durability loss for that craft.
- All of the above anticipates the real workbench model (500 cargo cells +
  8 designated single-item slots: hacksaw/saw/hammer/shovel/screwdriver/
  pliers/sledgehammer/sharpening stone, only those render on the model) -
  logic is written against "anywhere in cargo" for now and is designed to
  swap to "the named slot" with no schema change once that model exists.

## 2026-09-03 — Workbench crafting (not yet in-game tested)

- Add `SPKZ_WorkbenchKit` / `SPKZ_Workbench`: a deployable 500-cell (20x25)
  cargo container using a temporary real-vanilla model
  (`DZ\structures\furniture\various\workbench_dz.p3d`) as a placeholder until
  a custom model exists.
- Add a build menu (category tabs, recipe list with red/green affordability,
  cost breakdown, Build Now) opened via the vanilla interact action - no new
  keybind needed.
- Add a server-admin-tunable JSON recipe catalog
  (`$profile:SparkZBaseBuilding/WorkbenchRecipes.json`) with placeholder
  costs, and a self-contained RPC pair for the open/build round trip (this
  addon still has no SparkZGroup/SparkZCore dependency, so these RPC ids are
  scoped to `SPKZ_Workbench`'s own `OnRPC`, not a shared registry).
- Server independently re-validates the recipe and current cargo contents
  before consuming any material or spawning a kit - the client's menu
  display is never trusted.
- Access/build is not yet squad-permission-gated (no plot-pole/squad system
  exists in this addon yet - anyone can use any workbench, matching the
  existing rule that ordinary interactions like opening a door aren't
  owner-restricted either); only dismantle is owner-restricted, and only
  once the workbench's cargo is empty.
- Statically reviewed against the actual installed DayZ vanilla scripts
  (extracted `scripts.pbo`/`gui.pbo` via DayZ Tools) for every new API used,
  but **not yet compiled or tested in-game** - do not treat this as verified
  working until that happens.

## 2026-09-03 — Standalone building kits and development handover

- Add current addon, six editable models, game P3Ds, textures and materials.
- Add custom axis placement, live legend, full freeze, optional ground/free
  placement, edge snapping and attachment-pivot rotation.
- Add owner-only screwdriver dismantling and animated doors/shutters/garage.
- Add walkable 4.5 m floors/roofs, wider wall boards and fixed transparent glass.
- Correct glass rendering after in-game tests; preserve owner's garage edits.
- Reverse native garage tilt direction (pending in-game confirmation).
- Add native CE persistence types/install files and lifetime renewal (pending
  mission installation and a clean restart test). No storage wipe or resurrection.
- Add portable saved-Blender-to-MLOD exporter and detailed workflow/test handover.
- Preserve legacy prototype folders; do not pack them for this standalone build.



Dated entries, newest first. Bump `SparkZBaseConstants.MOD_VERSION` alongside
any entry that ships to the test server (see `docs/CONTRIBUTING.md`).

## 2026-09-02 — First standalone model prototype

- Added the editable Blender prototype for the standalone wooden door wall,
  using a provisional 4.00 m × 4.25 m square module envelope.
- Built the visible wall and animated door from separate UV-mapped wooden
  boards with a generated clean wood texture.
- Added preliminary Geometry, Fire Geometry, View Geometry, hinge-axis, and
  snap-point source collections plus closed/open preview renders.
- Added a repeatable Blender generator and structural validator. This is not
  yet a DayZ P3D or an in-game object, so no runtime version was bumped.

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
  for two contributors collaborating on this codebase.

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
