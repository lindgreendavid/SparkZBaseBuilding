# Coding Standards & Known Pitfalls

This file exists so that every contributor picking up this mod
inherits lessons that were expensive to learn the first time. Read the
pitfalls section **before** touching `requiredAddons`, multi-line boolean
expressions, or a `super.ActionCondition()` call.

## Conventions

- **Module tier discipline**: a `modded class X` must live in the same
  script tier (`3_Game`/`4_World`/`5_Mission`) where `X` itself is declared.
  If you're not sure which tier a vanilla/BBP class lives in, check an
  existing hook of the same class elsewhere in this repo before guessing.
- **`super` first, deny second — unless the native check is the problem.**
  Default pattern: run our own permission check, `return false` early if it
  fails, otherwise fall through to `super.ActionCondition(...)`. Only skip
  `super` entirely when the native behavior itself conflicts with the squad
  model (see Pitfall #4).
- **Bypass logic goes in a comment, not just a commit message.** Every hook
  file in this repo that does something non-obvious (skips `super`, gates
  one action but not a sibling action) has a comment explaining *why*,
  directly above the code. Keep that pattern — the reasoning is what future
  sessions actually need, not a restatement of what the code does.
- **Config values are live JSON, not just compile-time defaults.** Adding a
  field to `SparkZBaseConfig` gives it a default for servers with no existing
  file; a server with an existing `BaseConfig.json` keeps whatever value is
  already on disk for keys that already exist. Don't assume changing a
  default in code changes behavior on a live server.
- **Persist timestamps as Unix time via `SparkZBaseTime.Now()`**, not engine
  ticks, so state survives a server restart correctly. If a new mod needs
  the same utility and adding a real dependency on `SparkZBase`/`SparkZCore`
  isn't worth it, a small self-contained copy (see `SparkZAirdropTime` in the
  sibling `SparkZAirdrop` mod) is an acceptable, deliberate duplication —
  better than a fragile cross-mod dependency for one utility function.

## Pitfalls (all previously hit in this codebase or a sibling SparkZ mod)

### 1. Multi-line boolean expressions can produce a silent-ish parser warning that still breaks the server

```c
// BROKEN - do not write conditions like this:
quote.IsLeader = isRecordedLeader
    || hasLeaderRoleFlag;
```

Enfusion Script's parser does not reliably accept a boolean expression split
across lines when the continuation operator (`||`/`&&`) starts the second
line. This produces a compiler **warning**, not a hard error —
`Missing ';' at the end of line` — so the PBO still builds and the server
still boots with a clean-looking RPT. The actual symptom is the DayZ
Launcher showing the server as greyed out/unjoinable despite a healthy boot.
**Always write a full boolean expression on one line, or use intermediate
named booleans, one per line:**

```c
// CORRECT
bool isRecordedLeader = groupPayload && groupPayload.LeaderId == statusPlayerId;
bool hasLeaderRoleFlag = SparkZGroupServerGroupManager.Get().HasMemberRoleFlag(statusPlayerId, SparkZGroupConstants.SQUAD_ROLE_LEADER);
quote.IsLeader = isRecordedLeader || hasLeaderRoleFlag;
```

### 2. Never add a sibling SparkZ mod to `requiredAddons[]`, even a real one

Adding e.g. `"SparkZStorage"` to `SparkZBase`'s own `requiredAddons[]` in
`config.cpp` — even though `SparkZStorage` is a real, correctly-loading
addon this mod genuinely depends on functionally — corrupts the DayZ
Launcher's server-browser query. The server itself boots fine and the RPT
looks completely clean; only the Launcher's greyed-out listing reveals it.
Confirmed twice in this project (once for `SPKZ_Storage`→`SparkZBase`, once
would-be for `HDSN_BreachingCharge`, avoided after the first incident).
**Load-order dependency between SparkZ mods is expressed only through the
server's mod line / load order, never through `requiredAddons[]` cross-
references between our own mods.**

### 3. Always delete, never overwrite, an old mod folder before copying in a new build

A stale leftover file from a previous version (e.g. an old hooks file that
referenced a class removed in a rollback) caused a real client-side compile
crash ("Unknown type ...") even though the new build was otherwise correct.
Overwriting a folder in place can leave orphaned files from a prior version
sitting next to the new ones. Full delete-then-copy, every time, on both
server and any client testing it.

### 4. When the native `super` behavior is the actual bug, bypass it — don't layer a check in front of it

`ActionDismantleSPKZMilitaryCrate`'s native `super.ActionCondition()` (in
the storage mod) enforces a single-owner-only check that has nothing to do
with squads. Adding our own check *in addition to* that `super` call would
still block a Base Manager who didn't personally place the crate. The fix
was to skip `super` entirely for this one action and replace it completely
with the squad-based check (see `SparkZBaseStorageHooks.c`). This is the
exception to the "always call `super`" convention above — reach for it only
when you've confirmed the native check itself, not just a missing check
next to it, is the actual conflict.

### 5. The DayZ Launcher's greyed-out/unjoinable state has multiple unrelated root causes

It has come from (at least): a bogus `requiredAddons` entry that references
a class that doesn't need an external addon at all, a *real* cross-SparkZ-
mod `requiredAddons` entry (Pitfall #2), and a plain script syntax warning
(Pitfall #1) with zero config changes involved. **Static code review alone
has never found one of these.** The only reliable diagnostic is bisection:
strip mods from the server's mod line one at a time (or in halves) until the
Launcher shows the server as joinable again, then narrow down from there.
Always get a fresh RPT from the actual failing boot before guessing — but
note the RPT can look completely clean even when the cause is a compiler
*warning* rather than a hard error, so "the RPT shows nothing" is not proof
the newest code change is innocent.

### 6. Don't guess a third-party mod's internal config schema or class names

An earlier attempt to auto-configure the `HDSN_BreachingCharge` explosive
mod's door/window target list by reading its compiled source and guessing
class name strings produced a config that matched nothing and broke charge
placement entirely (worse than not integrating at all). The only reliable
fix came from the mod author's own distributed example config files, and
from live-verifying real object class names in-game (an admin's
"Delete Object" confirmation dialog shows the exact class name of whatever
object is targeted — use that instead of guessing from decompiled/packed
source).

## Testing checklist before calling a change done

1. Build, then **delete** (not overwrite) the old folder on the test server
   and any test client, then copy in the new build.
2. Boot the server, confirm a clean RPT and steady FPS heartbeat.
3. Confirm the server shows up **green/joinable in the DayZ Launcher**, not
   just that it boots — these are different checks (see Pitfall #5).
4. Exercise the actual feature changed, in-game, with the specific
   permission level it targets (e.g. a non-Manager squad member, a non-squad
   player, an abandoned base) — not just the happy path.
5. If something breaks and the RPT looks clean, treat "clean RPT" as
   inconclusive, not exonerating, and start bisecting mods instead of
   re-reading the diff.
