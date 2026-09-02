# Features

## What this mod is not

It is not a construction system. **BaseBuildingPlus (BBP)** provides the
parts, walls, doors, and the actual building/dismantling tool actions. This
mod never reimplements or copies that — it hooks into BBP's actions to add
rules BBP doesn't have: squad ownership, permissions, and upkeep.

## Current (implemented, live-tested)

- **Squad claims**: a squad Leader or Base Manager claims a flagpole area
  (`SparkZBaseTerritoryFlag`) for their `SparkZGroup` squad; one active base
  per squad at a time.
- **Daily upkeep billing**: a flat base cost plus a per-object rate (BBP
  parts, doors, storage containers) scanned live within the claim radius,
  billed in an in-game currency item. Unpaid past a configurable grace
  period auto-abandons the base (doors/windows deleted, fences opened).
- **Give Up Base**: the squad Leader can voluntarily abandon a base early.
- **Permission-gated building**: build/dismantle BBP parts requires Base
  Access for the owning squad within a claimed area; outside any claim,
  a config flag (`RequireClaimForBBPBuild`) decides whether building is
  free-for-all or claim-required.
- **CodeLock integration**: squad members with Base Access can open a locked
  door on their own base without the passcode; non-squad players still need
  the real code. Placing a new CodeLock is gated to Base Managers.
- **SparkZStorage integration**: placing storage kits requires Base Manager
  status within a claim; opening/closing requires Base Access; dismantling
  is squad-Base-Access-gated (bypassing the storage mod's own native
  single-owner check — see `docs/CODING_STANDARDS.md` Pitfall #4) and
  blocked outright for non-squad players (raid it with explosives instead).
- **Property menu UI**: client-side menu showing base status, days owed,
  and buttons to pay upkeep or give up the base.

## Known gaps / open issues

- Breach-charge (explosive) raiding is configured directly in the
  `HDSN_BreachingCharge` mod's own JSON, not through anything in this repo —
  see the disabled `SparkZBaseBreachCompat.c` and its explanation.
- No claim-radius visualization in the client UI yet (players have to infer
  claim boundaries from where actions succeed/fail).
- No multi-base-per-squad support (`FindActiveBaseForGroup` assumes a single
  active claim).

## Roadmap ideas (things that would meaningfully differentiate this mod)

Not committed yet — discuss/prioritize before starting any of these so two
sessions don't build the same thing twice:

- Tiered claim sizes (pay more upkeep for a larger radius) instead of one
  fixed `DefaultClaimRadiusMeters`.
- Contested/siege mechanics: a way for another squad to formally challenge
  an active claim rather than only raiding an abandoned one.
- Base-level shared "vault" currency so upkeep doesn't rely on one player
  holding physical currency items.
- In-world visual claim boundary (marker or effect) instead of the current
  "find out by trying" model.
- Role beyond Leader/Manager/Access — e.g. a build-only role that can place
  BBP parts but not manage storage or pay upkeep.

Update this list as ideas are discussed and move implemented ones to
"Current" with a note on which version shipped them.
