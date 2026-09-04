# Standalone building workflow and handover

## Raiding: charge placement (SPKZ_PlacedCharge)

Two portable, plantable items, both extending the installed game's own
`ExplosivesBase` (the real vanilla base `Plastic_Explosive`/
`ImprovisedExplosive` also use - see `scripts/4_World/Entities/
SPKZ_PlacedCharge.c` for why config and script ancestors have to match here):

- `SPKZ_HomemadeBreachingCharge` - 3:30 fuse. 1 breaches a wood wall/window
  outright; wood-tier pieces currently need only 1 (see
  `SPKZ_RequiredChargeCount()` on `SPKZ_WoodWallDoor` - a future metal-tier
  SPKZ piece overrides this to 2, no other changes needed).
- `SPKZ_FabricatedChargeBlock` (the fictional "C4") - 5:00 fuse. Always
  breaches in a single plant regardless of `SPKZ_RequiredChargeCount()` (per
  direction: it "immediately goes through metal and reinforced things").

Both are placed via a new action, `SPKZ_ActionPlaceCharge` (registered on
`SPKZ_WoodWallDoor`, not on the charge item itself - same pattern as
`SPKZ_ActionDismantleWorkbench`): hold the charge, look at a wall/window
where `SPKZ_CanAcceptCharge()` is true, hold to plant. This is NOT vanilla's
generic ground-deploy ghost (`ActionDeployObject`) - that has no concept of
"must be next to a specific wall" - so neither charge class registers those
actions.

Scope, per direction: **SparkZBaseBuilding wall/window pieces only** for
now, not BaseBuildingPlus or any other mod's structures. Floors/roofs/ramps
(`SPKZ_WoodFloor`) and the garage door (`SPKZ_WoodGarage`) are explicitly
excluded (`SPKZ_CanAcceptCharge()` overridden to false / no action
registered) - only walls, door walls, windows, and glass windows accept a
charge, matching "wooden walls and wooden windows."

While armed, a charge beeps every 4 seconds for the whole fuse duration
(`SPKZ_PlayBeep`, a repeating `CallLater`) via the real
`RemoteDetonator_Trigger_SoundSet` vanilla sound, played through
`ExplosivesBase`'s own `GetArmSoundset()`/`StartItemSoundServer(
SoundConstants.ITEM_EXPLOSIVE_ARM)` mechanism - the same real,
MP-replicated, server-callable sound slot vanilla's own `ActionArmExplosive`
uses for its one-shot arm click, just fired repeatedly here instead.

When a charge's fuse ends, it calls the real vanilla `ExplosivesBase`
explosion pipeline (`Plastic_Explosive_Ammo`, real particle/light/**explosion**
sound via that ammo's own `soundSetExplosion[]` - no custom explosion system
invented) and separately calls
`SPKZ_ApplyChargeHit()` on its target wall, which tracks a persisted hit
counter (`m_SPKZChargeHits`) and deletes the wall outright (no kit returned -
unlike a screwdriver dismantle, this is meant to be a one-way loss) once the
required count is reached. The native area-of-effect explosion damage does
NOT touch the wall directly - walls call `SetAllowDamage(false)` by design
(only our own scripted dismantle/breach paths can remove one), so the hit-
counter approach is deliberate, not a workaround.

**Placeholder models**: both items currently use the SparkZKit cardboard box
model - the friend's `dayz_custom_items_v4.zip` package (delivered
separately) covers the real textures/geometry for both ("Improvised
Breaching Charge" and "Fabricated Charge Block" in that package's naming),
pending their Blender pass and a P3D export. Swapping `model=` in
`explosives_vehicles.hpp` is the only change needed once that's ready.

## Current source of truth

`SparkZBaseBuilding/` is the current standalone addon to pack. Its `Source/`
folder contains the latest editable Blender files, including the owner's
garage edits. `Data/` contains exported editable MLOD P3Ds, model.cfg files,
wood textures and glass materials. The top-level `Source/Models/` and
`SparkZBase/` are historical prototypes; do not use them to rebuild current
pieces. SparkZCore/Group snapshots are not dependencies of this iteration.

## Parts

| Spawn kit | Model directory | Export argument |
|---|---|---|
| SPKZ_WoodWallDoorKit | WoodDoorWall | no argument |
| SPKZ_WoodWallKit | WoodWall | --plain |
| SPKZ_WoodFloorKit | WoodFloor | --floor |
| SPKZ_WoodWindowKit | WoodWindow | --window |
| SPKZ_WoodGlassWindowKit | WoodGlassWindow | --glass |
| SPKZ_WoodGarageKit | WoodGarage | --garage |

Walls use a nominal 4.5 m width / 3.2 m height. Floors are 4.5 m square and
0.20 m thick, with their origin at the walking surface. Wall widths/heights,
origins and snap anchors are coupled: coordinate changes must be reviewed
against `scripts/4_World/Hologram/SPKZ_BuildingHologram.c`.

## Editing an existing Blender model

1. Read the latest local files first. Coordinate with the other contributor.
   Fetch/review upstream when repository sync is requested; do not silently
   replace another person's saved model or routinely push every edit.
2. Back up the affected blend/P3D outside the pack folder. Edit the current
   `SparkZBaseBuilding/Source/<directory>/SPKZ_*.blend`, then save it.
3. Preserve `dayz_selection` custom properties, collection membership and
   hinge/pivot parenting. The exporter selects those properties, not merely
   visible objects. Export uses frame 1 as the closed/rest pose.
4. Every plank has its own UV map and a single wood material. Existing wall
   variants cycle 1–6; floor variants cycle 1–4. Grain follows the length of
   the individual board. Keep touching/overlapping closed cores beneath the
   shallow bevels so seams do not become peepholes. Texture material naming
   is mapped explicitly in the exporter: update that mapping for new assets.
5. Update Geometry, View Geometry and Fire Geometry when the visible shape
   changes. These are separate simplified collision meshes; editing a visual
   board does NOT automatically update them. Keep convex closed components.
6. Moving panels must have the same bone selection in all collision LODs.
   Memory axes must match the Blender pivot and the model.cfg animation.
   Floors also need Roadway. Glass has physical/fire collision, but no pane
   in View Geometry; its transparent visual surface is separate.

## Export to Object Builder / DayZ

Using Blender 5.1 from a PowerShell terminal at repository root:

```powershell
& 'C:/Program Files/Blender Foundation/Blender 5.1/blender.exe' --background --python tools/export_building_mlod.py -- --garage
```

Replace `--garage` using the table. For the door wall omit the trailing
`-- --garage`. The exporter is repository-relative and carries its MLOD
writer with it; it does not need storage-mod files or this workstation's
workspace. It reads the saved blend without saving over it and writes the
matching `Data/<directory>/SPKZ_*.p3d`.

The exporter maps Blender (X,Y,Z) to DayZ (X,Z,Y), preserving UVs and named
selections, and writes visual, Geometry, Memory, LandContact, View Geometry
and Fire Geometry. Floor export adds Roadway. Open the resulting P3D in DayZ
Object Builder to inspect LODs, selections, textures and animation axes.
Blender previews do not validate DayZ shader sorting or animation direction.

Garage caution: the initial native rotation sign disagreed with the Blender
preview. Current model.cfg reverses GarageTilt so the front should face up
when parked. Keep that native correction when re-exporting. Lift stages and
the parent carriage remain unchanged. Retest the full motion and roof fit.

## Adding a part

- Add its config classes, CfgPatches unit entries, model and model.cfg.
- Derive kits from the existing kit hierarchy; implement placed/projection
  types and add an in-hands profile. Return the correct kit on dismantling.
- Add animated interaction classes only where needed, register them and
  include their selections in hologram material refresh. Closed wooden/glass
  wall variants have no door interactions.
- Reuse owner serialization and server-side checks. Never manually delete a
  kit inside OnPlacementComplete: the vanilla deployment action does that.
- Add the new placed class and kit to ServerFiles/SparkZBaseBuilding/types.xml.
- Verify native method signatures in the installed DayZ scripts before adding
  overrides. Previous errors included invalid Update/Open overrides and a
  missing bool return from OnStoreLoad.

## Placement and interaction

X cycles spin, forward/back, up/down, sideways, tilt and roll. Wheel is coarse,
Page Up/Down fine adjustment; F freezes both position and rotation; Home
toggles ground/free-look; End toggles snapping. The legend reports transform
values continuously. Snapped rotation compensates for the attachment anchor.
Manual translation/rotation remains available for ramps and non-ground builds.
Ground-following floor preview starts 22 cm up; wall-bottom snaps intentionally
remain at the wall datum, with manual lift available.

Only the placer can dismantle using a screwdriver. Ordinary open/close actions
are not owner-restricted - the workbench's access action follows that same
rule (anyone can open the build menu; only the placer can dismantle it, and
only once it has no materials left inside). Plot poles, squad permissions,
payment enforcement and C4 integration are NOT implemented in this addon yet.

## Workbench crafting

`SPKZ_WorkbenchKit` deploys `SPKZ_Workbench`: a 20x25 (500-cell) cargo
container with its own real model (`Data/Workbench/SPKZ_Workbench.p3d`) and
eight real named attachment slots for tools (see below) - it extends
`SPKZ_WoodWallDoor`/`SPKZ_WoodWallDoorKit` purely to reuse their owner
tracking, lifetime refresh, save/load and dismantle-to-kit machinery; it is
not a wall (`SPKZ_HasDoor()` is false). Store raw materials in its cargo, then
press interact
(whatever key you have bound - it uses the vanilla look-and-interact system,
`SPKZ_ActionAccessWorkbench`) to open a build menu: category tabs on the left,
recipes in the middle (red if you can't currently afford them), and a cost
breakdown + BUILD NOW button on the right once a recipe is selected. Building
spawns the same kit item the wall/floor/etc. pieces already use - it does not
place anything directly.

Recipes are server-admin-tunable JSON at
`$profile:SparkZBaseBuilding/WorkbenchRecipes.json` (auto-created with
placeholder costs on first boot - see `SPKZ_WorkbenchRecipeCatalog` in
`scripts/3_Game/Data/SPKZ_WorkbenchRecipe.c`), sent to the client over a
self-contained RPC pair (`scripts/3_Game/RPC/SPKZ_WorkbenchRPC.c`) - this
addon still does not depend on SparkZGroup/SparkZCore, so these RPC ids are
scoped entirely to `SPKZ_Workbench`'s own `OnRPC`, not a shared registry. The
server always re-validates recipe and stock server-side before consuming
anything or spawning a kit, regardless of what the client's menu displays.

### Nails, tools, and the sharpening stone

The real workbench model has 500 cargo cells plus eight designated
single-item attachment slots (`CfgSlots`/`CfgNonAIVehicles` in
`workbench_slots.hpp`): `SPKZ_WB_Hacksaw`, `SPKZ_WB_HandSaw`,
`SPKZ_WB_Hammer`, `SPKZ_WB_Shovel`, `SPKZ_WB_Screwdriver`, `SPKZ_WB_Pliers`,
`SPKZ_WB_SledgeHammer`, `SPKZ_WB_SharpeningStone` - only a tool mounted in its
own slot renders on the model and counts as "present"; ordinary cargo stays
hidden. All eight tools and the sharpening stone are real vanilla items
(`Hacksaw`, `HandSaw`, `Hammer`, `Shovel`, `Screwdriver`, `Pliers`,
`SledgeHammer`, `Whetstone`).

- Every recipe can require one or more tools (`SPKZ_WorkbenchRecipe.Tools`,
  `SPKZ_WorkbenchToolRequirement`). A tool is never consumed - it takes a
  flat health-point loss per successful craft instead
  (`SPKZ_Workbench.SPKZ_DamageToolOfType`), and a ruined tool no longer
  counts as present. "Present" means "mounted in its own named attachment
  slot" (`GetInventory().FindAttachmentByName(slotName)` via
  `SPKZ_Workbench.SPKZ_ToolSlotName`/`SPKZ_FindUsableTool`), not anywhere in
  cargo - `CanReceiveAttachment` validates each slot only accepts its
  matching tool classname.
- Which tool a recipe needs, and how much durability it costs, is a
  placeholder pending real design (per direction: "we gotta find a nice way
  of checking what items use what tools") - the current seed data
  deliberately varies it (Hammer for wood pieces, Pliers for the glass
  frame, Hacksaw for the garage's metal sheets) to prove the system supports
  different tools per recipe; retune freely in the JSON.
- Nail is now a required material on every seeded recipe - per direction,
  it's essential across all wall/door/window/floor/garage construction.
  Materials are counted/consumed from cargo only
  (`CargoBase.GetItemCount()`/`GetItem(int)`) - tools in their attachment
  slots are never scanned as materials.
- The sharpening stone (`SPKZ_WorkbenchRecipeCatalog.SharpeningStoneClassName`
  = `Whetstone`, mounted in `SPKZ_WB_SharpeningStone`) fully offsets a tool's
  durability loss for the craft it's present for. This is a placeholder
  repair amount (full offset = "keeps tools maintained for free while
  mounted"); the exact behaviour is a design decision to revisit later.

The workbench kit places like a plain vanilla deployable (storage crate/
tent-style ghost, no axis-rotate/freeze/snap controls) even though its kit
class extends SPKZ_WoodWallDoorKit for its shared kit logic - the
axis-rotate/freeze/snap wall-joining system in
`scripts/4_World/Hologram/SPKZ_BuildingHologram.c` is a blanket
`modded class Hologram` gated entirely on one helper, `SPKZ_IsWallKit()`,
which now explicitly excludes `SPKZ_WorkbenchKit` so every gated method
falls through to real vanilla Hologram behaviour for it.

Each tool's durability loss per craft is currently: Hammer 5, Pliers 5,
Hacksaw 8 (see `SPKZ_WorkbenchRecipe.SeedDefaults`) - fully offset if a
Whetstone is mounted in its slot for that craft.

All recipe icons currently point at the same placeholder kit icon - there is
no per-part icon art yet (see docs/BRIEF.md's open asset-source question).
The icon widget and per-recipe `IconPath` field are already wired end to end,
so real art drops in later with zero script changes.

Material/tool requirement rows show each item's real, already-translated
name (`EntityAI.GetDisplayName()` on the same local preview instance the row
renders - no manual classname-to-label table to maintain) rather than the
raw classname, plus a quantity or MOUNTED/MISSING line and a colored status
dot. Materials and tools both flow into one scrollable column
(`DetailFlowSpacer`, a single `Columns 1` grid spacer with dynamically
inserted section-header widgets) instead of two separately fixed-position
sections - the old layout overlapped once a recipe had enough rows to
overflow its statically-allocated slice of the panel.

Categories: "Wood" covers all six wall/door/window/floor/garage kits
(including the garage, which uses some metal plate but is still fundamentally
a wood structure - it was miscategorized as "Metal" originally). "Metal" is
shown as an always-present tab even with zero recipes in it yet, a preview of
what's coming. A "Misc" category (vanilla `GardenPlotGreenhouse`) was tried
and removed again per direction - its preview didn't render correctly and it
needs more work before shipping. Sandbags and tank traps were requested too,
but neither exists as a player-craftable vanilla item in the installed game
version - the only "tank trap"-like class found anywhere in
`structures_*`/`gear_*.pbo` is `StaticObj_Misc_Hedgehog_Concrete`/`_Iron`,
non-spawnable map decoration, not a deployable kit. Adding either for real,
or a genuine "Metal" category (chain-link, corrugated sheet, etc.), means
building custom SparkZ models for them - the same kind of work as the
workbench itself.

`SPKZ_Workbench` extends `SPKZ_WorkbenchStorageBase` (a shared base for
built pieces needing real Inventory/Cargo persistence while staying
non-pickable - `Container_Base` config ancestor, `IsTakeable`/
`CanPutIntoHands`/`CanPutInCargo` all false), adopted from the
collaborator's own independently-converged fix for the exact same
config/script mismatch bug this addon hit. The stock snapshot sent to the
client now includes tool presence (1/0 from the named attachment slot, not
a cargo scan) alongside material counts - it previously only collected
material classnames, so every tool always showed MISSING in the UI
regardless of whether it was actually mounted.

Not yet done: squad Base Access permission gating on the access/build actions
(this addon has no plot-pole/squad-permission system to check against yet -
see docs/BRIEF.md), a real workbench model, and in-game verification of the
whole crafting flow (menu layout, RPC round-trip, cargo consumption, kit
spawn) - this has only been statically reviewed against real DayZ script
APIs, not yet compiled or tested in-game.

## Packing, server setup and testing

The owner normally packs the inner addon folder manually. No automatic PBO
build is required. Preserve prefix `SparkZBaseBuilding`; use DayZ tools for
texture conversion/binarization. Exclude Source/, Blender backups, PNG source
textures and development files from a release PBO; retain P3D, PAA, RVMAT,
BISURF, config/scripts/layouts/inputs and the prefix. Do not ship private keys.
Use an identical new PBO on client and server, with no stale competing copies.

Persistence also requires the ServerFiles mission integration: see INSTALL.txt.
The supplied economy-core XML is a snapshot of the local Chernarus mission;
on another server MERGE its small `<ce>` include rather than overwriting that
server's existing configuration. Back up the mission and NEVER wipe storage_1
to apply this change. Persistence uses native CE entries and stored owner/open
state. Placed-object cleanup lifetime is renewed, but ObjectDelete/dismantling
still work; there is no custom restore loop that resurrects deleted pieces.

User-confirmed in-game: walls, floors/roof fit, window interaction, improved
glass rendering, placement controls, snapping and dismantling. The latest
garage-facing correction and native restart-persistence changes are prepared
but still need validation with the mission files installed. Previously lost
objects are not restored by this patch.

Before marking a build verified: test all kit previews; placement/snap edges;
opening from both sides and network sync; vehicle clearance; bullets through
open apertures and blocked by closed panels/glass; owner/non-owner dismantle;
clean server restart retaining position/orientation/owner/open state; deletion
remaining deleted after restart. Keep static/model checks distinct from actual
DayZ compilation and runtime checks. See per-feature notes for the test history.

## Collaboration / repository delivery

When asked to push, fetch and review remote changes, reconcile without replacing
other work, document the changed features and unverified items, then commit to a
feature branch and push without force. Main is an integration branch. Keep old
prototype/version rules scoped to their own code, not this standalone addon.
Do not remove required third-party credits. Binary blend files need one editor
at a time; they cannot be meaningfully line-merged.
