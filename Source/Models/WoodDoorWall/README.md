# SPKZ Wood Door Wall — Blender Prototype

This is the first editable model prototype for the standalone
SparkZBaseBuilding construction system. It is not yet an exported DayZ P3D
or an in-game-ready object.

## Prototype dimensions

- Outer wall: 4.00 m wide × 4.25 m tall × 0.24 m deep
- Clear doorway target: 1.20 m wide × 2.45 m tall
- Door leaf: 1.12 m wide × 2.36 m tall × 0.105 m deep
- Door travel: 105 degrees, frames 1–30

The 4.00 m × 4.25 m envelope is provisional. It deliberately makes the wall
taller than a future 4.00 m floor rotated upright. The values require visual,
player-clearance, vehicle-clearance, and in-game validation before becoming
the common module standard.

## Contents

- `SPKZ_WoodDoorWall.blend` — editable Blender 5.1 source
- `textures/spkz_clean_wood_d.png` — generated clean wood diffuse texture
- `previews/` — closed and open renders
- `MODEL_VALIDATION.json` — structural checks performed on the Blender file

The visual wall is assembled from separate, UV-mapped wooden boards with
small consistent gaps, framed by square timber members. The door is also
made from separate boards and includes hinges, a handle, rails, and a brace.

## Blender organization

- `1.000 - Resolution` — visible model pieces
- `Geometry` — authored simplified physical-collision proxies
- `Fire Geometry` — authored bullet-blocking proxies
- `View Geometry` — authored visibility/interaction proxies
- `Memory` — hinge-axis and preliminary snap-point markers
- `PREVIEW_ONLY` — camera, lights, and floor used only for renders

The moving objects use the `door` custom selection marker and are parented
to `door_hinge`. The opening preview is keyed from closed at frame 1 to
105 degrees open at frame 30. Memory markers `axis_door_start` and
`axis_door_end` define the intended vertical DayZ animation axis.

## Rebuild and validation

Run with Blender 5.1:

```powershell
blender --background --python tools/modeling/create_wood_door_wall.py
blender --background --python tools/modeling/validate_wood_door_wall.py
```

The generator recreates the blend file, texture, and previews. Validation
checks collection structure, UV presence, door selection membership, memory
points, dimensions, and distinct closed/open animation states.

## Still required before in-game use

- Export the resolution, Geometry, Fire Geometry, View Geometry, and Memory
  LODs into a DayZ P3D without losing named selections or animation axes.
- Author and parse `model.cfg` for the `door` rotation source.
- Register the object and temporary cardboard kit in the standalone config.
- Implement server-authoritative place/open/close/persistence behaviour.
- Verify that the closed door blocks players, view, and bullets.
- Verify that opening moves every relevant collision and ballistic selection
  clear of the doorway.
- Test real player clearance and revise prototype dimensions if required.

Static Blender validation is not a substitute for P3D structural validation,
DayZ script compilation, or multiplayer/in-game testing.
