import bpy
import json
import os


root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
asset_dir = os.path.join(root, "Source", "Models", "WoodDoorWall")
blend_path = os.path.join(asset_dir, "SPKZ_WoodDoorWall.blend")
bpy.ops.wm.open_mainfile(filepath=blend_path)

scene = bpy.context.scene
required_collections = [
    "1.000 - Resolution",
    "Geometry",
    "Fire Geometry",
    "View Geometry",
    "Memory",
]
collection_counts = {}
for name in required_collections:
    collection = bpy.data.collections.get(name)
    collection_counts[name] = len(collection.objects) if collection else 0

visual_collection = bpy.data.collections.get("1.000 - Resolution")
visual_meshes = [obj for obj in visual_collection.objects if obj.type == "MESH"]
for obj in visual_meshes:
    obj.data.calc_loop_triangles()
missing_uv = [obj.name for obj in visual_meshes if not obj.data.uv_layers]
door_meshes = [obj.name for obj in visual_meshes if obj.get("dayz_selection") == "door"]

door_root = bpy.data.objects.get("door_hinge")
wood_image = bpy.data.images.get("SPKZ_CleanWood_D")
texture_path_normalized = wood_image.filepath.replace("\\", "/") if wood_image else ""
scene.frame_set(1)
closed_angle = round(door_root.rotation_euler.z, 6) if door_root else None
scene.frame_set(30)
open_angle = round(door_root.rotation_euler.z, 6) if door_root else None

required_memory = ["axis_door_start", "axis_door_end", "snap_left", "snap_right", "snap_floor_bottom", "snap_floor_top"]
missing_memory = [name for name in required_memory if bpy.data.objects.get(name) is None]

validation = {
    "asset": "SPKZ_WoodDoorWall",
    "status": "PASS",
    "prototype_dimensions_m": {
        "wall_width": scene.get("SPKZ_wall_width_m"),
        "wall_height": scene.get("SPKZ_wall_height_m"),
        "door_opening_width": scene.get("SPKZ_door_opening_width_m"),
        "door_opening_height": scene.get("SPKZ_door_opening_height_m"),
    },
    "collections": collection_counts,
    "visual_mesh_count": len(visual_meshes),
    "visual_vertices": sum(len(obj.data.vertices) for obj in visual_meshes),
    "visual_triangles": sum(len(obj.data.loop_triangles) for obj in visual_meshes),
    "all_visual_meshes_uv_mapped": len(missing_uv) == 0,
    "texture_uses_relative_path": bool(wood_image and texture_path_normalized.startswith("//textures/")),
    "texture_path": wood_image.filepath if wood_image else None,
    "missing_uv_meshes": missing_uv,
    "door_selection_mesh_count": len(door_meshes),
    "door_animation": {
        "root": "door_hinge",
        "action": door_root.animation_data.action.name if door_root and door_root.animation_data and door_root.animation_data.action else None,
        "closed_frame": 1,
        "closed_angle_radians": closed_angle,
        "open_frame": 30,
        "open_angle_radians": open_angle,
    },
    "missing_memory_points": missing_memory,
    "notes": [
        "This validates the editable Blender source, not an exported DayZ P3D.",
        "P3D named selections, LOD properties, model.cfg parsing, bullet blocking, collision, and in-game animation still require validation after export.",
        "Prototype dimensions require visual and in-game approval before they become the shared module standard.",
    ],
}

if any(collection_counts[name] == 0 for name in required_collections):
    validation["status"] = "FAIL"
if missing_uv or not door_root or missing_memory or closed_angle == open_angle or not wood_image or not texture_path_normalized.startswith("//textures/"):
    validation["status"] = "FAIL"

path = os.path.join(asset_dir, "MODEL_VALIDATION.json")
with open(path, "w", encoding="utf-8") as handle:
    json.dump(validation, handle, indent=2)

print(json.dumps(validation, indent=2))
if validation["status"] != "PASS":
    raise SystemExit(1)
