# Wooden floor — first in-game test

Added 2026-09-03. Standalone SparkZBaseBuilding; no BBP dependency. No PBO packed and no GitHub sync performed.

## Model

- Class to spawn: `SPKZ_WoodFloorKit` (shared cardboard kit).
- Placed class: `SPKZ_WoodFloor`; projection: `SPKZ_WoodFloor_Hologram`.
- Dimensions: 4.5 x 4.5 m; thickness 0.20 m. Origin is the upper walking surface. Thickness extends below it, so a roof snapped to wall-top sits flush at that height.
- Twelve individual wide planks, approximately 35.5 cm wide. Each plank owns a separate mesh/UV layer and one material. Four separate PNG/PAA wood images repeat **1,2,3,4,1,2,3,4,1,2,3,4** across the floor.
- Four-millimetre plank bevels with a closed internal core prevent see-through seams. Dark perimeter framing. Textured underside.
- P3D includes visual, Geometry, Memory, LandContact, Roadway, View Geometry and Fire Geometry LODs. Geometry top is 5 mm below the roadway to avoid coplanar collision surfaces.
- Editable source: `SparkZBaseBuilding/Source/WoodFloor/SPKZ_WoodFloor.blend`; packed image textures included.
- In-game model: `SparkZBaseBuilding/Data/WoodFloor/SPKZ_WoodFloor.p3d`.

## Behaviour

Full placement rotation remains enabled for ramps. Existing axis cycling, freeze, micro-adjustments, legend, optional snapping and ground/free-look remain shared with the walls. Floor kits inherit the working plain-wall projection setup, without a door selection or door actions.

Floor-to-wall snapping offers the wall bottom or top. The player's side of the wall chooses the face onto which the floor extends. Floor-to-floor snapping supports all four perimeter edges. Walls can snap onto all four floor edges; their initial orientation follows the selected edge. Snapped rotation retains the joining anchor.

Owner-only screwdriver dismantling returns a floor kit. Existing indestructibility/ownership is inherited. No C4 implementation or crafting changes in this iteration.

## Verification and remaining game test

Passed local checks: model dimensions, twelve separate UV meshes, 1–4 texture order, packed Blender images, all exported texture paths, seven readable MLOD sections, full closed view/fire collision boxes, roadway height, anchor-compensation arithmetic, native matrix API signatures and script brace balance. Preview inspected. This is not a DayZ compiler or runtime test.

Pack the inner `SparkZBaseBuilding` addon folder as usual and use the same resulting PBO on client and server. Test:

1. Spawn floor kit; verify cardboard in inventory and floor hologram on deployment.
2. Join floors edge-to-edge and into a box of four walls. Test the wall-bottom and wall-top snaps from either face.
3. Walk over the floor and seams, including an elevated floor. Tilt a floor to a reasonable ramp angle and walk up it.
4. Freeze and rotate a snapped floor; verify the attachment point remains still. Confirm wheel and micro-adjustments still function.
5. Dismantle your floor with a screwdriver and confirm the returned kit type. Verify another player cannot dismantle it.
6. Check door opening, plain-wall placement and existing wall-to-wall snaps for regressions. Check persistence after a server restart.
7. Test bullet blocking from above/below and inspect the underside as a ceiling.

Roadway reference: [Bohemia Interactive LOD documentation](https://community.bistudio.com/wiki/LOD#Roadway). Runtime walking and ramp limits still need testing in DayZ.

## Texture assets

Four original wood variants are stored as separate PNG/PAA images, 512 x 2048
pixels each, converted with DayZ ImageToPAA. Each floor plank samples exactly
one variant, not the full source atlas.
