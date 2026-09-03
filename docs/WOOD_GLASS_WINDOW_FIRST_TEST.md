# Fixed glass window

Spawn `SPKZ_WoodGlassWindowKit`. Same 4.5 x 3.2 m wall module; centred 2.6 x 1.4 m aperture, compared with the shutter window's 1.4 x 1.4 m aperture. Original six wood textures and wider-plank design. Fixed transparent pane, no open/close actions. Shared cardboard kit, snapping, ownership and screwdriver dismantling.

Glass is rendered separately from its physical behaviour: transparent visual material; fixed solid Geometry and Fire Geometry pane; no pane in View Geometry. A custom zero-penetrability surface is assigned to all fire faces of this new model. The building inherits damage/destruction protection. This is an implementation awaiting shooting tests, not a verified guarantee for all ammunition or third-party mods.

Editable file: `SparkZBaseBuilding/Source/WoodGlassWindow/SPKZ_WoodGlassWindow.blend`. Game file: `SparkZBaseBuilding/Data/WoodGlassWindow/SPKZ_WoodGlassWindow.p3d`.

Model/material checks passed; Blender preview inspected. Test in DayZ: visibility from both sides, glass reflections/alpha, hologram, wall/floor snapping, no window actions, owner-only dismantling and persistence. Shoot the glass from both directions with your server's ammunition and confirm no damage behind it. Separately verify that the existing door and shutter openings allow shots when open and block when closed; those assets were not modified by this addition.

Material references: [Bohemia glass shader](https://community.bistudio.com/wiki/Material_-_Basic_glass_reflectance), [surface properties](https://community.bistudio.com/wiki/RVMAT), [fire-geometry penetration](https://community.bistudio.com/wiki/Bullet_penetrability). The local vanilla headlight glass material confirms the environment texture path `dz/data/data/env_land_co.paa`.

Pack the inner addon folder and use the same PBO on client/server. No PBO packed or repository sync performed.
