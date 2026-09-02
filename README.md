# SparkZBaseBuilding

A custom DayZ base-building mod for the SparkZ modpack. It layers squad-aware
claims, upkeep, storage, and raid rules on top of **BaseBuildingPlus (BBP)**
construction, rather than reimplementing construction itself.

This is **our own mod** — BaseBuildingPlus (and other workshop base-building
mods) are referenced only for general conventions (how a DayZ base-building
addon is packaged, standard tier layout, etc.). No code, assets, or config
values are copied from any workshop mod. Functionality here is designed to be
different: squad-permission-gated building/claims/storage/upkeep is not
something BBP or similar mods provide natively.

## Status

Early-stage. The current source in this repo is the working implementation
that has been live-tested on a private DayZ server; it is being restructured
and extended into its own standalone feature set. See
[`docs/FEATURES.md`](docs/FEATURES.md) for what exists today vs. what's planned.

## For collaborators (human or AI)

Read these before touching code, in order:

1. [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) — how the mod is structured,
   what each file/class does, and how the pieces talk to each other.
2. [`docs/CODING_STANDARDS.md`](docs/CODING_STANDARDS.md) — Enfusion Script
   conventions used in this codebase, and a list of **real bugs we've already
   hit** so they aren't repeated.
3. [`docs/CONTRIBUTING.md`](docs/CONTRIBUTING.md) — git workflow, branch/file
   ownership rules for two people (or two AI sessions) working at once,
   commit conventions, and the testing checklist before you call anything done.
4. [`docs/FEATURES.md`](docs/FEATURES.md) — current feature inventory and the
   roadmap of planned functionality that differentiates this mod from BBP.
5. [`CHANGELOG.md`](CHANGELOG.md) — dated log of what changed and why.

## Dependencies

Declared in `config.cpp`'s `requiredAddons[]`:

- `DZ_Data`, `DZ_Gear_Camping`, `DZ_Gear_Containers`, `DZ_Gear_Tools` (vanilla)
- `SparkZCore`, `SparkZGroup` — sibling SparkZ mods (squad/permission system)
- `BaseBuildingPlus` — construction itself; this mod hooks into BBP's actions
  rather than replacing them
- `SparkZNote_Data`, `CodeLock`

**Do not** add any other SparkZ mod (e.g. `SparkZStorage`) to this list even
if it's a real, correctly-loading dependency — see the "requiredAddons
cross-mod" pitfall in `docs/CODING_STANDARDS.md`. It silently breaks the
DayZ Launcher's server-browser listing while the server itself boots fine.

## Repo layout

```
config.cpp / mod.cpp      - addon manifest (CfgPatches / CfgMods)
scripts/3_Game/           - shared data classes, network state (client+server)
scripts/4_World/          - server-authoritative logic, entity/action hooks
scripts/5_Mission/        - client-side mission-scope code, UI, network client
gui/                      - layouts and branding assets
```
