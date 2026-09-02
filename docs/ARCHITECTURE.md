# Architecture

## Design principle: hook, don't replace

This mod does **not** reimplement base construction. It hooks into
**BaseBuildingPlus (BBP)**'s existing actions (`ActionBuildPart`,
`ActionDismantleBBP`, `ActionDeployObject`, door/lock actions, ...) via
`modded class` overrides, and adds a layer BBP doesn't have on its own:

- squad-scoped **claims** (a flagpole + radius owned by one SparkZGroup squad)
- **upkeep** billed in an in-game currency, with a grace period and automatic
  abandonment
- **permission-gated** build/dismantle/storage/lock actions, keyed off squad
  roles from `SparkZGroup` (Leader / Base Manager / Base Access), not BBP's
  own single-owner model
- integration hooks for sibling SparkZ mods (`SparkZStorage`, `CodeLock`)

Everywhere a hook exists, the pattern is:

```c
modded class SomeBBPOrVanillaAction
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (/* this action targets something SparkZBase cares about */)
        {
            if (!SparkZBaseManagerServer.Get().CanXxxAtPosition(player, position))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }
}
```

`super.ActionCondition(...)` is called when our own check passes, so BBP's
(or vanilla's) native checks still apply on top. The one deliberate exception
is documented in `SparkZBaseStorageHooks.c`: BBP/SparkZStorage's own native
single-owner dismantle check is too restrictive for a squad model, so that
one hook bypasses `super` entirely instead of layering on top of it. See
`docs/CODING_STANDARDS.md` for when bypassing `super` is the right call vs.
the wrong one.

## Module tiers

Enfusion Script requires a `modded class` to live in the same tier
(`3_Game` / `4_World` / `5_Mission`) as the class it extends. This is why
the same feature is sometimes split across tiers:

| Tier | Contains | Why |
|---|---|---|
| `3_Game` | `SparkZBaseConstants`, `SparkZBaseStatusPayload`, `SparkZBaseNetworkState` | Shared data structures and constants both client and server code need; base classes for these live in `3_Game`. |
| `4_World` | `SparkZBaseManagerServer` (the core), `SparkZBaseConfig`, `SparkZBaseRecord`, action hooks (BBP/CodeLock/Storage), `SparkZBaseTerritoryFlag`, `SparkZBaseTime`, player-base hooks | Server-authoritative logic. `BBP_BASE`, `Fence`, `ActionBuildPart`, etc. are `4_World` classes, so anything hooking them must be too. |
| `5_Mission` | `MissionServer`/`MissionGameplay` bootstrap, `SparkZBaseNetworkClient`, the property menu UI | Mission-scope: server startup wiring and all client-side UI/RPC-receiving code. |

## Core class: `SparkZBaseManagerServer`

Singleton (`SparkZBaseManagerServer.Get()`), server-only, in
`scripts/4_World/SparkZBase/Server/SparkZBaseManagerServer.c`. Owns the
in-memory list of `SparkZBaseRecord` (one per claimed base) and is the single
source of truth every hook file calls into. Key entry points other files use:

- `CanUseBaseAtPosition(player, position, managerRequired = false)` — general
  "is this player allowed to use this claimed base" check (doors, storage
  open/close/dismantle). `managerRequired` narrows it to Base Manager only.
- `CanBuildAtPosition(player, position)` — gates BBP building/dismantling;
  falls back to config's `RequireClaimForBBPBuild` when no claim exists at
  all (build-anywhere vs. claim-required world).
- `CanPlaceRestrictedItemAtPosition(player, position)` — stricter, deny-by-
  default check used for CodeLocks and SparkZStorage placement: no claim
  here means no, unlike the two above which default-allow outside any claim.
- `HasActiveBaseAccessAtPosition` — used by the CodeLock bypass-with-access
  flow.
- `ClaimFlag`, `PayUpkeep`, `GiveUpBase`, `BuildStatusText` — the player-
  facing base-management actions, driven by RPCs from the property menu UI.

Permission checks themselves are **not** implemented here — they delegate to
`SparkZGroupServerGroupManager.Get()` (from the sibling `SparkZGroup` mod)
for `HasBaseManager` / `HasBaseAccess` / `GetGroupIdForMember`. SparkZBase
owns *what* is gated; SparkZGroup owns *who* is allowed.

## Data & persistence

- **Config** (`SparkZBaseConfig`): tunables (daily costs, claim radius, grace
  period, currency class name). Loaded from and re-saved to
  `$profile:SparkZ/Base/BaseConfig.json` on every server boot — editing the
  live JSON directly changes server behavior without a PBO rebuild, but a
  fresh JsonFileLoader default value only takes effect for *new* keys; an
  existing key's value on disk always wins.
- **Base records** (`SparkZBaseRecord`, wrapped in
  `SparkZBasePersistentStore`): one entry per claimed base — id, owning
  squad, flag position, radius, timestamps, abandoned flag. Persisted to
  `$profile:SparkZ/Base/Bases.json`, loaded on `SparkZBaseManagerServer`
  construction, saved after every mutation (claim, pay, abandon, give-up).
- **Billing model**: `BuildQuoteForRecord` computes `DailyCost` by scanning
  every object within the base's radius (`GetObjectsAtPosition`, capped at
  `MaxObjectsToScan`) and summing a per-BBP-part, per-door, and per-storage-
  item daily rate on top of the flat `BaseDailyCost`. `DaysOwed` is elapsed
  time since `LastPaidAt` divided into whole days; abandonment triggers once
  `DaysOwed >= GraceDaysBeforeAbandoned`.

## Client/server split

- Server: everything above, plus `SparkZBasePlayerBase.c` (per-player server
  hooks) and `SparkZBaseTerritoryFlag.c` (the flagpole entity itself).
- Client: `SparkZBaseNetworkClient.c` sends RPCs
  (`SPARKZ_RPC_BASE_STATUS_REQUEST` / `..._PAY_REQUEST` / `..._GIVEUP_REQUEST`)
  and receives `SPARKZ_RPC_BASE_STATUS_RESPONSE`; `SparkZBasePropertyMenu.c` /
  `SparkZBasePropertyMenuManager.c` render the UI that triggers those
  requests and displays `SparkZBaseStatusPayload` data.
- RPC IDs and the request/response wiring live in `SparkZCore` (shared
  across all SparkZ mods) via `SparkZCoreRPCId` / `SparkZCoreRPCGuard`.

## Known-disabled code: `SparkZBaseBreachCompat.c`

Present in the tree but never called (the `MissionServer.c` call site is
commented out with an explanation). It was a guessed integration with the
`HDSN_BreachingCharge` explosive mod that turned out actively harmful — see
the pitfall write-up in `docs/CODING_STANDARDS.md` before touching breach-
charge integration again. Breach-charge target lists are now configured
directly in that mod's own JSON, generated from its author-provided
reference files, not through this compat layer.

## Cross-mod dependency map

```
SparkZCore    -> RPC plumbing, shared constants        (required by everything)
SparkZGroup   -> squad membership + role permissions   (SparkZBase reads from this)
BaseBuildingPlus -> construction itself                (SparkZBase hooks its actions)
CodeLock      -> door/tent locking                     (SparkZBase hooks its actions)
SparkZStorage -> squad storage containers               (hooked from SparkZBase, NOT
                                                          listed in SparkZBase's own
                                                          requiredAddons — see pitfall #2
                                                          in CODING_STANDARDS.md)
```
