# Wood Window Kit

Same wall envelope as the existing wall/door modules (nominal 4.5 m wide x 3.2 m tall). Centred 1.4 x 1.4 m opening with a dark timber surround and two outward-opening wooden shutters. Both halves open/close together using one interaction. Original six plank textures, individually mapped. No glass in this wooden-shutter version.

Spawn `SPKZ_WoodWindowKit`. Placed class `SPKZ_WoodWindow`. Hologram `SPKZ_WoodWindow_Hologram`.

Shares existing cardboard kit, snapping, placement controls, ownership, indestructibility and owner-only screwdriver dismantling. Window state uses the existing synchronized/persistent openable state. Moving selections exist in visual, Geometry, View Geometry and Fire Geometry, with two separate hinge axes.

Editable model: `SparkZBaseBuilding/Source/WoodWindow/SPKZ_WoodWindow.blend`. Frame 1 closed, frame 31 open. Game model: `SparkZBaseBuilding/Data/WoodWindow/SPKZ_WoodWindow.p3d`.

Local verification: matching wall bounds, six readable LODs, animated selection membership and axes, exported texture paths, individual UV meshes and outward-opening Blender collision proxies. Open/closed renders inspected. DayZ compilation/runtime have not been tested.

Pack the inner addon folder; install the same PBO on client/server. Test inventory preview, placement/hologram and wall/floor snapping; open/close from either side (including raised/tilted placement); confirm both leaves move for a second player; check collision/bullet blocking closed versus clear aperture open; restart to check state/owner persistence; dismantle your window back into the correct kit. Existing wall/door/floor regression checks are also recommended.
