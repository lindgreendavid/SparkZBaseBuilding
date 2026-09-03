# Wider wall planks — 2026-09-03

- Plain wall: 30 narrow boards replaced with 18 wider boards.
- Door wall: six boards per side and six above the doorway; the four existing door-leaf boards are unchanged.
- Each replacement board has its own mesh/UV mapping and uses the original textures 1–6, repeated. Original closed bevel profile retained.
- Dimensions, structural frames, door opening, hinge and collision/memory LODs preserved. All non-plank Blender objects verified unchanged; exported nonvisual LODs checked byte-for-byte against backups.
- Ground-following floor placement now starts with its upper surface 22 cm above terrain (20 cm slab plus 2 cm clearance). Snapped heights and free placement are unchanged. Manual height adjustment remains available.
- Original Blender/P3D files retained outside the addon in `work/backups_before_wider_wall_planks` at the workspace root.
- Updated both editable Blender files and game P3Ds. Render previews inspected; not yet tested in DayZ. Pack the inner addon folder and install the same PBO on client and server.
