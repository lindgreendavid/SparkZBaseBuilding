# Wood Garage Door Kit

Spawn `SPKZ_WoodGarageKit`. Placed class `SPKZ_WoodGarage`; hologram `SPKZ_WoodGarage_Hologram`. Nominal wall envelope remains 4.5 x 3.2 m. Frame opening is 3.8 m wide x 2.84 m tall; the parked overhead panel leaves approximately 2.75 m usable height. Vehicle fit must be tested with your actual vehicles.

Twelve horizontal wooden boards, each independently UV mapped using original textures 1–6 twice. Dark frame, rear reinforcement and handle. No bottom threshold across the driveway.

Up-and-over mechanism: rotation about a horizontal central pivot combined with a rising parent carriage. Four staged translations approximate the lift curve. Three-second open/close source periods; reverse interaction reverses the mechanism. All visual/physical/bullet-collision panel sections use the same moving bone. The closed panel swings outward at its bottom while its top moves inward, then parks horizontally with approximately half the panel projecting on each side of the wall. Allow space inside and outside during operation.

Existing kit appearance, snapping, placement controls, ownership, persistence and screwdriver dismantling are reused. Doors/windows/floor/glass assets are unchanged.

Editable source: `SparkZBaseBuilding/Source/WoodGarage/SPKZ_WoodGarage.blend`, frame 1 closed, frame 49 halfway, frame 97 open. Game model: `SparkZBaseBuilding/Data/WoodGarage/SPKZ_WoodGarage.p3d`.

Local checks: six readable LODs, moving panel in visual/Geometry/View/Fire sections, hinge/carriage memory, texture paths and sequence. All 97 Blender animation samples checked for ground clearance and a roof underside at 3.0 m; staged lift arithmetic compared against model configuration. Closed/half/open previews inspected. These are not DayZ compiler or runtime tests.

Pack the inner addon folder and use the same PBO on client/server. Test:

1. Inventory kit and hologram; wall/floor snaps; frozen placement adjustment.
2. Open/close from either side, including when the panel is overhead; watch that lift and rotation follow the preview.
3. Put a floor roof at wall-top and verify no clipping throughout the motion.
4. Drive your Humvee through when fully open; allow clearance before operating the door.
5. Verify a second player sees the same motion, reverse halfway, and restart for saved state/ownership.
6. Check closed bullet blocking versus open passage; screwdriver dismantle returns the garage kit for its owner only.

Animation reference: [Bohemia model configuration](https://community.bistudio.com/wiki/Model_Config). No PBO packed or repository sync performed.
