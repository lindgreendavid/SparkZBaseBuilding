# SparkZBaseBuilding

**This is a standalone DayZ base-building mod with its own construction
system.** It does not require, hook into, or extend BaseBuildingPlus (BBP)
or any other workshop base-building mod — see
[`docs/BRIEF.md`](docs/BRIEF.md), which is the current source of truth for
project direction. **Read it before `docs/ARCHITECTURE.md`.**

BaseBuildingPlus was looked at once, purely as a reference for how a
base-building mod's addon is generally organized (part categories, a
kit-deploy-then-build flow, tiered upgrades, lock/raid concerns) — never as
a dependency, and never as a source of code or assets (it's distributed
compiled/packed, so there was nothing to copy even if we'd wanted to).

The `SparkZBase/` folder in this repo right now is a **previous prototype**
that *was* built as a permission layer hooked onto BBP. It's kept as design
reference (its squad-claim/upkeep/permission model is worth reusing) but its
BBP-hook code is not — full context in `docs/BRIEF.md` and the pivot entry
at the top of `CHANGELOG.md`.

## Repo contains three mods

The base-building mod's permission checks call directly into the squad
system, so this repo includes all three mods needed to actually understand
and build it, mirroring how they sit side-by-side on the live server:

| Folder | Role |
|---|---|
| `SparkZBase/` | The base-building mod itself — everything in this repo's `docs/` is primarily about this one. |
| `SparkZGroup/` | Squad membership and permission roles (Leader / Base Manager / Base Access). `SparkZBase` reads permissions from here on every gated action. |
| `SparkZCore/` | Shared RPC plumbing, constants, and logging used by every SparkZ mod, including the request/response flow behind `SparkZBase`'s property menu. |

**Important:** `SparkZGroup` and `SparkZCore` are also used by other SparkZ
mods that are *not* part of this repo (e.g. `SparkZKOTH`, `SparkZAirdrop` on
the live server). The copies here are a snapshot for building/understanding
this project, not the single source of truth for those two mods project-
wide. See "Cross-repo sync" in [`docs/CONTRIBUTING.md`](docs/CONTRIBUTING.md)
before editing anything in `SparkZGroup/` or `SparkZCore/`.

## Status

Pre-implementation. `docs/BRIEF.md` captures the direction (standalone
construction system) and the open design questions that need answers before
writing part/placement code. The `SparkZBase/` folder holds the old
BBP-hooked prototype, kept as reference only — see `docs/FEATURES.md` for
exactly what's being carried forward vs. rebuilt from scratch.

## For collaborators (human or AI)

Read these before touching code, in order:

1. [`docs/BRIEF.md`](docs/BRIEF.md) — **read this first.** What this project
   actually is, why it's standalone, what BBP was and wasn't used for, and
   the open design questions to resolve before writing construction code.
2. [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) — describes the old
   BBP-hooked prototype in `SparkZBase/` (permission model, module tiers,
   the SparkZGroup/SparkZCore integration points). Useful for the parts
   `docs/BRIEF.md` says are worth reusing; not a spec for the new
   construction system, which doesn't exist yet.
3. [`docs/CODING_STANDARDS.md`](docs/CODING_STANDARDS.md) — Enfusion Script
   conventions, and a list of **real bugs already hit** in this project so
   they aren't repeated.
4. [`docs/CONTRIBUTING.md`](docs/CONTRIBUTING.md) — git workflow, branch/file
   ownership rules for two people (or two AI sessions) working at once,
   commit conventions, cross-repo sync rules for `SparkZGroup`/`SparkZCore`,
   and the testing checklist before you call anything done.
5. [`docs/FEATURES.md`](docs/FEATURES.md) — current feature inventory (the
   old prototype's) and roadmap ideas.
6. [`CHANGELOG.md`](CHANGELOG.md) — dated log of what changed and why,
   including the pivot decision itself.

## Dependencies

The new standalone system's `requiredAddons[]` has not been written yet —
it will **not** include `BaseBuildingPlus`. For reference, the *old
prototype*'s `SparkZBase/config.cpp` currently declares:

- `DZ_Data`, `DZ_Gear_Camping`, `DZ_Gear_Containers`, `DZ_Gear_Tools` (vanilla)
- `SparkZCore`, `SparkZGroup` — included in this repo (see table above), and
  still the plan for the new system
- `BaseBuildingPlus`, `CodeLock`, `SparkZNote_Data` — the old prototype's
  dependencies; `BaseBuildingPlus` in particular goes away entirely once the
  standalone construction system replaces it

**Do not** add any other SparkZ mod (e.g. `SparkZStorage`) to any mod's own
`requiredAddons[]` even if it's a real, correctly-loading dependency — see
the "requiredAddons cross-mod" pitfall in `docs/CODING_STANDARDS.md`. It
silently breaks the DayZ Launcher's server-browser listing while the server
itself boots fine.

## Repo layout

```
SparkZBase/    - the base-building mod (config.cpp/mod.cpp, scripts/3_Game|4_World|5_Mission, gui/)
SparkZGroup/   - squad/permission system this mod depends on
SparkZCore/    - shared RPC/constants/logging infrastructure
docs/          - architecture, coding standards, contributing, features
```

Each mod folder follows the same internal layout:
`config.cpp` / `mod.cpp` (addon manifest), `scripts/3_Game/` (shared data,
client+server), `scripts/4_World/` (server-authoritative logic), `scripts/5_Mission/`
(client-side mission code, UI), `gui/` (layouts, branding, icons).
