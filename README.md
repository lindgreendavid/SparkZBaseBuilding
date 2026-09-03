# SparkZBaseBuilding

Standalone DayZ building addon. **Pack the inner `SparkZBaseBuilding/` folder**,
not the legacy `SparkZBase/` prototype. No BBP or storage-mod dependency.

Start with [workflow and collaborator handover](docs/WORKFLOW.md): model editing,
UVs, Blender-to-P3D export, animations/collision, adding kits, packing, server
persistence, controls and testing. See [changelog](CHANGELOG.md) for recent work.

Current pieces: wooden wall, door wall, walkable floor/roof/ramp, split shutter
window, fixed wide glass window and up-and-over garage door. Editable Blender
models are under `SparkZBaseBuilding/Source/`; P3Ds/materials under `Data/`.

`tools/export_building_mlod.py` exports the saved models using Blender; see the
handover for arguments. It does not overwrite your saved blend or pack a PBO.

Server installation also needs [ServerFiles/INSTALL.txt](ServerFiles/INSTALL.txt).
Do not delete `storage_1`. The latest persistence and garage-facing corrections
still need in-game verification; previously lost pieces are not restored.

The current addon uses owner-only screwdriver dismantling and native persistence.
Plot poles, workbench crafting, squad permissions, C4 and payment enforcement are
future work. Legacy `SparkZBase/`, `SparkZGroup/`, `SparkZCore/` and top-level
`Source/Models/` are preserved as historical/reference material, not the current
standalone build. Their older architecture docs describe that earlier system.

For collaboration read [CONTRIBUTING](docs/CONTRIBUTING.md) and
[CODING_STANDARDS](docs/CODING_STANDARDS.md), noting that the current workflow
supersedes automatic sync/version rules for the old prototype. Repository sync
and PBO builds are done when explicitly requested.
