# Standalone building workflow and handover

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
are not owner-restricted. Plot poles, squad permissions, crafting/workbench,
payment enforcement and C4 integration are NOT implemented in this addon yet.

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
