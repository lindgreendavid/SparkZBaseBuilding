# Contributing (for two people / two AI sessions working at once)

This mod is being built by two people, each likely working through their own
AI assistant session, sometimes at the same time. Enfusion Script files don't
diff or merge cleanly (no semantic merge tooling), so the rules below matter
more here than in a typical software project — treat file ownership as the
real safety net, git as the backstop.

## Git workflow

**Hard rule, no exceptions: `git fetch` and check for upstream changes
before you (or your AI session) read code to plan work, before every
commit, and before every push.** The whole point of two people/AI sessions
working on this at once is that the other person's changes can land at any
moment — starting work, committing, or pushing against a stale view of
`main` is exactly how the "both edited the same file" and "my change
silently undoes theirs" problems happen. Concretely, before any of those
three actions:

```
git fetch origin
git log HEAD..origin/main --oneline   # anything listed here, you haven't seen yet
```

If that shows commits you haven't reviewed, **read them** (`git show` /
`git diff`) before proceeding — especially anything touching
`docs/`, `CHANGELOG.md`, or a file adjacent to what you're about to change.
Merge/rebase them in before you commit or push your own work on top.

- `main` is the integration branch. Nobody commits straight to `main`.
- Each person/session works on their own branch:
  `git checkout -b <yourname>/<short-feature-name>`.
- Push your branch and open a PR into `main` when a change is ready to
  integrate — don't sit on a branch for days accumulating unrelated changes.
- Pull and merge/rebase `main` into your branch **before** starting new work
  each session, not after — you want to know about the other person's
  changes before you start editing, not after you've already diverged.
- Commit messages: one line, present tense, states *what* changed; if the
  *why* isn't obvious from the diff, add it as a short paragraph below (see
  existing commits in `SparkZBase`'s parent modpack repo for the style used
  throughout this project).

## File/module ownership (avoid simultaneous edits)

Before starting a session, claim a lane out loud (Discord/voice) so you're
not both mid-edit on the same file:

| Lane | Files |
|---|---|
| Server logic / permissions | `SparkZBase/scripts/4_World/SparkZBase/Server/`, `SparkZBase/scripts/4_World/SparkZBase/Data/` |
| BBP / CodeLock / Storage hooks | `SparkZBase/scripts/4_World/SparkZBase/BBP/`, `.../CodeLock/`, `.../Storage/` |
| UI / client | `SparkZBase/scripts/5_Mission/SparkZBase/UI/`, `SparkZBase/gui/` |
| Networking / RPC payloads | `SparkZBase/scripts/3_Game/SparkZBase/Network/`, `SparkZBase/scripts/3_Game/SparkZBase/Data/` |
| Config / manifest | `SparkZBase/config.cpp`, `SparkZBase/mod.cpp`, `docs/`, `CHANGELOG.md` |
| Squad/permission system | `SparkZGroup/` — see "Cross-repo sync" below, this is not a normal editing lane |
| Shared RPC/infra | `SparkZCore/` — same as above |

If a change genuinely spans lanes (e.g. a new permission needs a new RPC
field), whoever starts it says so up front and the other person avoids that
file until it's merged.

## Cross-repo sync — `SparkZGroup/` and `SparkZCore/`

These two folders are a **snapshot**, not this repo's own project. Their
real source of truth is the main SparkZ modpack, where they're also used by
mods outside this project (SparkZKOTH, SparkZAirdrop, etc.). That means two
different rules depending on which direction a change goes:

**If SparkZGroup or SparkZCore change in the main modpack** (a permission
flag added, a bug fixed, a new RPC type, anything): that change **must** be
pulled into this repo too, or this repo's `SparkZBase` code will silently
drift against a squad/RPC system it no longer matches. To do this:

1. Run `tools/sync-dependencies.ps1 -ModpackRoot <path to the main modpack>`.
2. Review `git diff` for `SparkZGroup/` and `SparkZCore/` — confirm you
   understand what changed before committing it blind.
3. Add a dated entry to `CHANGELOG.md` under a "Dependency sync" heading
   describing what changed in the dependency and why it matters to
   base-building code (e.g. "added `SQUAD_PERMISSION_BASE_BUILD` flag —
   `SparkZBaseManagerServer.CanBuildAtPosition` should migrate to it").
4. If the change affects a class or method `SparkZBase` actually calls,
   update the "what SparkZBase actually uses from them" section in
   `docs/ARCHITECTURE.md` to match.
5. Commit the sync and the doc updates together, then push.

**If base-building work needs a change *inside* `SparkZGroup/` or
`SparkZCore/`** (e.g. a new permission flag base-building needs that doesn't
exist yet): make it here, but flag it loudly (commit message + chat message
to the other person) — it needs to be **manually ported back** to the main
modpack's copy of that mod, since there's no automatic link in that
direction. Don't let the two copies diverge silently in either direction.

## Versioning

Bump `SparkZBaseConstants.MOD_VERSION` on every change that ships to the
test server, using a short descriptive suffix
(e.g. `"0.3.3-STORAGE-DISMANTLE-FIX"`). This is how you confirm which build
is actually running — grep the built PBO for the version string after
building, don't assume the build picked up your latest edit.

## Testing before you say something is done

Run through `docs/CODING_STANDARDS.md`'s testing checklist. In short: full
delete-then-replace of the mod folder on server *and* client, confirm the
server shows joinable in the Launcher (not just that it boots), then
exercise the actual permission level the change targets in-game.

## When you hit the "server is greyed out" class of bug

Don't re-read the diff first. See Pitfall #5 in `docs/CODING_STANDARDS.md`:
bisect the mod list first, get a fresh RPT from the actual failing boot,
and only then look at what changed in that RPT's timeframe.

## Documentation is part of the change, not an afterthought

If you add a new permission flag, a new hook file, a new persisted field, or
find a new pitfall the hard way: update `docs/ARCHITECTURE.md`,
`docs/CODING_STANDARDS.md`, or `CHANGELOG.md` in the *same* PR. The next
session (human or AI) starts cold and only has these files plus the code —
undocumented tribal knowledge is knowledge that gets re-broken.
