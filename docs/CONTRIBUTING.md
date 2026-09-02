# Contributing (for two people / two AI sessions working at once)

This mod is being built by two people, each likely working through their own
AI assistant session, sometimes at the same time. Enfusion Script files don't
diff or merge cleanly (no semantic merge tooling), so the rules below matter
more here than in a typical software project — treat file ownership as the
real safety net, git as the backstop.

## Git workflow

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
| Server logic / permissions | `scripts/4_World/SparkZBase/Server/`, `scripts/4_World/SparkZBase/Data/` |
| BBP / CodeLock / Storage hooks | `scripts/4_World/SparkZBase/BBP/`, `.../CodeLock/`, `.../Storage/` |
| UI / client | `scripts/5_Mission/SparkZBase/UI/`, `gui/` |
| Networking / RPC payloads | `scripts/3_Game/SparkZBase/Network/`, `scripts/3_Game/SparkZBase/Data/` |
| Config / manifest | `config.cpp`, `mod.cpp`, `docs/`, `CHANGELOG.md` |

If a change genuinely spans lanes (e.g. a new permission needs a new RPC
field), whoever starts it says so up front and the other person avoids that
file until it's merged.

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
