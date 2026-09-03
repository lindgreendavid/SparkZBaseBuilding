# Project Brief: Standalone Base-Building System

Read this before `docs/ARCHITECTURE.md` if there's ever a conflict between
the two — this file states current intent; `ARCHITECTURE.md` describes the
older BBP-hooked implementation that's being superseded (see
`CHANGELOG.md`'s 2026-09-02 pivot entry for why).

## What this project is

A **standalone DayZ base-building mod**. It implements its own construction
system end to end:

- its own placeable/buildable part classes (foundations, walls, floors,
  roofs, doors, windows, stairs, and whatever else the design calls for)
- its own kit items and placement/build/upgrade/dismantle actions
- its own persistence for built structures (surviving server restarts)
- its own raid/destruction rules

It does **not** require, hook into, or extend **BaseBuildingPlus (BBP)** or
any other workshop base-building mod. No code or assets from any workshop
mod are used.

It **does** integrate with our own existing SparkZ mods for the parts that
aren't about construction itself:

- `SparkZGroup` — squad membership and permission roles (who can build,
  manage, or just access a base)
- `SparkZCore` — shared RPC plumbing, constants, logging

## Why "standalone," and what BBP was actually used for

BaseBuildingPlus was looked at for exactly one reason: as a **reference for
how a base-building mod is generally organized** — not its code (it's
distributed as a compiled/packed PBO, so there was nothing to read even if
we'd wanted to), but the *shape* of the problem: things like

- a small taxonomy of part types (foundation → wall → floor → roof, plus
  doors/windows/stairs as attachments to those)
- a kit-item-in-hand → deploy → progress-bar-build flow for placing a part
- tiered wall/door upgrades (e.g. wood frame → reinforced → metal) rather
  than one fixed part per slot
- lock/access compatibility as a separate concern from construction itself
- a raid-balance lever (some parts easier to breach than others)

These are **concepts to design our own version of**, not implementation
details to copy. Nothing here is a spec — it's a starting vocabulary for
design discussions between the two of you before
committing to a schema.

## What's being carried over from the old `SparkZBase/` prototype

The folder `SparkZBase/` in this repo is the previous implementation, built
as a permission layer on top of BBP. Its actual hook code
(`scripts/4_World/SparkZBase/BBP/`, `.../CodeLock/`, `.../Storage/`) is
BBP-specific and will not carry forward. What *is* worth reusing as a
starting point:

- The **squad-ownership model**: one active claimed base per squad, tied to
  a flagpole/anchor object, with a radius.
- The **permission tiers**: Leader / Base Manager / Base Access, already
  implemented in `SparkZGroup` and independent of construction.
- The **upkeep/billing shape**: a base cost plus a per-structure-count rate,
  a grace period, and automatic abandonment — the actual object-scanning
  logic (`BuildQuoteForRecord` in `SparkZBaseManagerServer.c`) will need to
  scan *our own* part classes instead of `BBP_BASE`/`SPKZ_MilitaryCrate`,
  but the billing model itself is sound.
- The **persistence pattern**: JSON-backed records under `$profile:SparkZ/`,
  loaded on boot and saved after every mutation.

Treat `SparkZBase/` as a design reference and a source of lessons (see
`docs/CODING_STANDARDS.md`), not as code to build directly on top of.

## Open questions to resolve before writing construction code

Discuss and record the decision (in this file or a new `docs/DESIGN.md`)
before implementing, so both sessions build against the same schema:

1. **Part granularity**: fixed-size grid pieces (foundation/wall/floor
   modules of one size) vs. free placement? Grid is dramatically simpler to
   implement correctly (sync, snapping, raid math) — recommend starting
   here.
2. **Assets**: new part models need to come from somewhere — vanilla DayZ
   asset reuse/retexture, a paid/free asset pack with a license that permits
   this use, or custom models. This is likely the single biggest unknown in
   the whole project and should be settled early since it blocks everything
   downstream of "place a part."
3. **Build flow**: instant placement vs. kit-deploy-then-progress-bar-build
   (closer to vanilla/BBP-style) vs. something else.
4. **Tiering**: does a wall have upgrade tiers (more HP, better materials)
   from day one, or is that a v2 feature?
5. **Raid model**: what destroys a part, and how — tools, explosives, both?
   How does this interact with the squad-permission model (a locked-out
   raider needs a path in; a squad member should never need one).
6. **Sync/persistence approach**: server-authoritative object list (like the
   old `SparkZBaseRecord` pattern) vs. per-entity `OnStoreSave`/`OnStoreLoad`
   (like the pattern already used in the sibling `SparkZAirdrop` mod for its
   crates) vs. both for different data.

## Status

This brief exists; construction implementation has not started. Next step
is resolving the open questions above, then defining the part-class schema
in `docs/ARCHITECTURE.md` (which should be rewritten around the standalone
system once that schema exists — right now it still describes the old
BBP-hooked prototype).
