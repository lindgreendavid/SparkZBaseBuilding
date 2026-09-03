# Glass rendering revision

Replaced the legacy Glass/AlphaShadow two-pass setup with a seven-stage Super material based on the locally available vanilla vehicle-part glass structure. Removed the second-pass reference, reduced reflection strength, and changed the pale diffuse colour to a restrained grey-green alpha tint. No emissive contribution.

Replaced the bevelled transparent cube with a thin pair of opposing, backface-culled faces: one visible face from either side, without stacked transparent front/back/edge surfaces. Geometry and bullet collision are unchanged. Previous Blender/P3D retained in workspace `work/glass_before_render_fix`.

The legacy shader's depth-testing symptoms match [this reported issue](https://feedback.bistudio.com/transactions/detail/PHID-XACT-TASK-th7kpjsgdpryv57/). This is a targeted correction, not a confirmed runtime fix yet.

Repack and test hands/items in front of the pane, oblique views behind timber, visibility through it from both sides, distant appearance in bright weather and bullet blocking. In-game validation is still required; a Blender render cannot validate DayZ transparency sorting.
