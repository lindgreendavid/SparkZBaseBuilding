import bpy
import math
import os
import random
from mathutils import Vector


ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
OUT_DIR = os.path.join(ROOT, "Source", "Models", "WoodDoorWall")
TEXTURE_DIR = os.path.join(OUT_DIR, "textures")
PREVIEW_DIR = os.path.join(OUT_DIR, "previews")
os.makedirs(TEXTURE_DIR, exist_ok=True)
os.makedirs(PREVIEW_DIR, exist_ok=True)

WALL_WIDTH = 4.0
WALL_HEIGHT = 4.25
WALL_DEPTH = 0.24
DOOR_OPENING_WIDTH = 1.20
DOOR_OPENING_HEIGHT = 2.45
DOOR_LEAF_WIDTH = 1.12
DOOR_LEAF_HEIGHT = 2.36
DOOR_LEAF_DEPTH = 0.105
PLANK_WIDTH = 0.235
PLANK_GAP = 0.012


def clean_scene():
    bpy.ops.object.select_all(action="SELECT")
    bpy.ops.object.delete(use_global=False)
    for datablocks in (bpy.data.meshes, bpy.data.curves, bpy.data.materials, bpy.data.cameras, bpy.data.lights):
        pass


def make_collection(name, parent=None):
    collection = bpy.data.collections.new(name)
    (parent or bpy.context.scene.collection).children.link(collection)
    return collection


def move_to_collection(obj, collection):
    for existing in list(obj.users_collection):
        existing.objects.unlink(obj)
    collection.objects.link(obj)


def create_wood_texture():
    import numpy as np

    size = 1024
    rng = np.random.default_rng(74123)
    y, x = np.mgrid[0:size, 0:size]
    grain = (
        0.48 * np.sin(x * 0.055 + np.sin(y * 0.014) * 3.2)
        + 0.25 * np.sin(x * 0.121 + y * 0.008)
        + 0.12 * np.sin(x * 0.31)
    )
    grain += rng.normal(0.0, 0.12, (size, size))
    grain = (grain - grain.min()) / (grain.max() - grain.min())
    knots = np.zeros((size, size), dtype=np.float32)
    for cx, cy, sx, sy in ((260, 300, 115, 48), (750, 690, 90, 38), (520, 860, 60, 26)):
        radius = ((x - cx) / sx) ** 2 + ((y - cy) / sy) ** 2
        knots += np.exp(-radius * 2.5) * (0.5 + 0.5 * np.sin(radius * 18.0))
    tone = np.clip(0.45 + grain * 0.36 - knots * 0.22, 0.0, 1.0)
    r = np.clip(tone * 0.78 + 0.14, 0.0, 1.0)
    g = np.clip(tone * 0.52 + 0.09, 0.0, 1.0)
    b = np.clip(tone * 0.29 + 0.055, 0.0, 1.0)
    rgba = np.dstack((r, g, b, np.ones_like(r))).astype(np.float32)
    image = bpy.data.images.new("SPKZ_CleanWood_D", width=size, height=size, alpha=False, float_buffer=False)
    image.pixels.foreach_set(rgba.ravel())
    image.filepath_raw = os.path.join(TEXTURE_DIR, "spkz_clean_wood_d.png")
    image.file_format = "PNG"
    image.save()
    return image


def make_material(name, image, tint=(1.0, 1.0, 1.0, 1.0), roughness=0.62):
    mat = bpy.data.materials.new(name)
    mat.use_nodes = True
    nodes = mat.node_tree.nodes
    links = mat.node_tree.links
    for node in list(nodes):
        nodes.remove(node)
    output = nodes.new("ShaderNodeOutputMaterial")
    shader = nodes.new("ShaderNodeBsdfPrincipled")
    tex = nodes.new("ShaderNodeTexImage")
    tex.image = image
    tex.interpolation = "Linear"
    shader.inputs["Base Color"].default_value = tint
    shader.inputs["Roughness"].default_value = roughness
    links.new(tex.outputs["Color"], shader.inputs["Base Color"])
    links.new(shader.outputs["BSDF"], output.inputs["Surface"])
    return mat


def make_metal_material():
    mat = bpy.data.materials.new("SPKZ_BlackenedSteel")
    mat.use_nodes = True
    shader = mat.node_tree.nodes.get("Principled BSDF")
    shader.inputs["Base Color"].default_value = (0.025, 0.03, 0.032, 1.0)
    shader.inputs["Metallic"].default_value = 0.78
    shader.inputs["Roughness"].default_value = 0.28
    return mat


def uv_box(obj, scale=1.0, rotation=0.0):
    bpy.context.view_layer.objects.active = obj
    obj.select_set(True)
    bpy.ops.object.mode_set(mode="EDIT")
    bpy.ops.mesh.select_all(action="SELECT")
    bpy.ops.uv.cube_project(cube_size=scale, correct_aspect=True)
    bpy.ops.object.mode_set(mode="OBJECT")
    if rotation:
        uv = obj.data.uv_layers.active
        c = math.cos(rotation)
        s = math.sin(rotation)
        for loop in uv.data:
            u = loop.uv.x - 0.5
            v = loop.uv.y - 0.5
            loop.uv.x = u * c - v * s + 0.5
            loop.uv.y = u * s + v * c + 0.5
    obj.select_set(False)


def box(name, location, dimensions, collection, material=None, bevel=0.0, uv_scale=1.0, parent=None):
    bpy.ops.mesh.primitive_cube_add(location=location)
    obj = bpy.context.object
    obj.name = name
    obj.dimensions = dimensions
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
    move_to_collection(obj, collection)
    if material:
        obj.data.materials.append(material)
    uv_box(obj, uv_scale)
    if bevel > 0:
        mod = obj.modifiers.new("Clean_Edges", "BEVEL")
        mod.width = bevel
        mod.segments = 2
        mod.limit_method = "ANGLE"
    if parent:
        obj.parent = parent
    return obj


def add_vertical_planks(prefix, x_min, x_max, z_min, z_max, y, collection, materials, parent=None):
    usable = x_max - x_min
    count = max(1, int((usable + PLANK_GAP) / (PLANK_WIDTH + PLANK_GAP)))
    actual_width = (usable - PLANK_GAP * (count - 1)) / count
    for i in range(count):
        x = x_min + actual_width * 0.5 + i * (actual_width + PLANK_GAP)
        mat = materials[i % len(materials)]
        plank = box(
            f"{prefix}_{i + 1:02d}",
            (x, y, (z_min + z_max) * 0.5),
            (actual_width, 0.065, z_max - z_min),
            collection,
            mat,
            bevel=0.008,
            uv_scale=0.85,
            parent=parent,
        )
        plank["dayz_selection"] = "door" if parent else "wall"


def add_memory_point(name, location, collection, parent=None):
    obj = bpy.data.objects.new(name, None)
    obj.empty_display_type = "PLAIN_AXES"
    obj.empty_display_size = 0.09
    obj.location = location
    obj["dayz_memory_point"] = True
    collection.objects.link(obj)
    if parent:
        obj.parent = parent
    return obj


def build_model():
    clean_scene()
    scene = bpy.context.scene
    scene.unit_settings.system = "METRIC"
    scene.unit_settings.scale_length = 1.0
    scene.render.engine = "BLENDER_EEVEE"
    scene.render.resolution_x = 1400
    scene.render.resolution_y = 1050
    scene.render.resolution_percentage = 100
    scene.render.image_settings.file_format = "PNG"
    scene.world.color = (0.025, 0.03, 0.035)

    master = make_collection("SPKZ_WoodDoorWall")
    visual = make_collection("1.000 - Resolution", master)
    geometry = make_collection("Geometry", master)
    fire_geometry = make_collection("Fire Geometry", master)
    view_geometry = make_collection("View Geometry", master)
    memory = make_collection("Memory", master)
    preview = make_collection("PREVIEW_ONLY", master)

    image = create_wood_texture()
    woods = [
        make_material("SPKZ_Wood_Light", image, (1.05, 0.98, 0.88, 1.0)),
        make_material("SPKZ_Wood_Mid", image, (0.91, 0.83, 0.71, 1.0)),
        make_material("SPKZ_Wood_Warm", image, (1.04, 0.84, 0.62, 1.0)),
        make_material("SPKZ_Wood_Dark", image, (0.72, 0.63, 0.52, 1.0)),
    ]
    metal = make_metal_material()

    outer_post = 0.18
    frame_depth = 0.22
    opening_left = -DOOR_OPENING_WIDTH * 0.5
    opening_right = DOOR_OPENING_WIDTH * 0.5

    add_vertical_planks("WallPlank_Left", -WALL_WIDTH / 2 + outer_post, opening_left - 0.10, 0.16, WALL_HEIGHT - 0.16, 0.0, visual, woods)
    add_vertical_planks("WallPlank_Right", opening_right + 0.10, WALL_WIDTH / 2 - outer_post, 0.16, WALL_HEIGHT - 0.16, 0.0, visual, woods[1:] + woods[:1])
    add_vertical_planks("WallPlank_Header", opening_left - 0.10, opening_right + 0.10, DOOR_OPENING_HEIGHT + 0.14, WALL_HEIGHT - 0.16, 0.0, visual, woods[2:] + woods[:2])

    for name, loc, dims in (
        ("Frame_LeftOuter", (-WALL_WIDTH / 2 + outer_post / 2, -0.015, WALL_HEIGHT / 2), (outer_post, frame_depth, WALL_HEIGHT)),
        ("Frame_RightOuter", (WALL_WIDTH / 2 - outer_post / 2, -0.015, WALL_HEIGHT / 2), (outer_post, frame_depth, WALL_HEIGHT)),
        ("Frame_Bottom", (0, -0.015, 0.09), (WALL_WIDTH - 2 * outer_post, frame_depth, 0.18)),
        ("Frame_Top", (0, -0.015, WALL_HEIGHT - 0.09), (WALL_WIDTH - 2 * outer_post, frame_depth, 0.18)),
        ("DoorJamb_Left", (opening_left - 0.075, -0.055, DOOR_OPENING_HEIGHT / 2), (0.15, 0.17, DOOR_OPENING_HEIGHT)),
        ("DoorJamb_Right", (opening_right + 0.075, -0.055, DOOR_OPENING_HEIGHT / 2), (0.15, 0.17, DOOR_OPENING_HEIGHT)),
        ("DoorLintel", (0, -0.055, DOOR_OPENING_HEIGHT + 0.075), (DOOR_OPENING_WIDTH + 0.30, 0.17, 0.15)),
    ):
        member = box(name, loc, dims, visual, woods[1], bevel=0.012, uv_scale=0.9)
        member["dayz_selection"] = "wall"

    hinge_x = opening_left + 0.03
    hinge_y = -WALL_DEPTH * 0.5 - DOOR_LEAF_DEPTH * 0.5 - 0.012
    door_root = bpy.data.objects.new("door_hinge", None)
    door_root.empty_display_type = "ARROWS"
    door_root.empty_display_size = 0.22
    door_root.location = (hinge_x, hinge_y, 0.055)
    door_root["dayz_selection"] = "door"
    visual.objects.link(door_root)

    local_x_min = 0.0
    local_x_max = DOOR_LEAF_WIDTH
    add_vertical_planks("DoorPlank", local_x_min, local_x_max, 0.06, DOOR_LEAF_HEIGHT, 0.0, visual, woods[3:] + woods[:3], parent=door_root)
    for i, z in enumerate((0.18, DOOR_LEAF_HEIGHT - 0.18), 1):
        rail = box(f"DoorRail_{i}", (DOOR_LEAF_WIDTH / 2, -0.056, z), (DOOR_LEAF_WIDTH, 0.075, 0.14), visual, woods[1], bevel=0.01, uv_scale=0.8, parent=door_root)
        rail["dayz_selection"] = "door"
    brace = box("DoorBrace", (DOOR_LEAF_WIDTH / 2, -0.062, DOOR_LEAF_HEIGHT / 2), (0.14, 0.075, 1.52), visual, woods[1], bevel=0.008, uv_scale=0.7, parent=door_root)
    brace.rotation_euler.y = math.radians(-25.0)
    brace["dayz_selection"] = "door"

    handle = box("DoorHandle", (DOOR_LEAF_WIDTH - 0.13, -0.115, 1.10), (0.055, 0.12, 0.055), visual, metal, bevel=0.012, uv_scale=1.0, parent=door_root)
    handle["dayz_selection"] = "door"
    for i, z in enumerate((0.42, DOOR_LEAF_HEIGHT - 0.42), 1):
        hinge = box(f"DoorHinge_{i}", (0.055, -0.108, z), (0.12, 0.045, 0.18), visual, metal, bevel=0.008, uv_scale=1.0, parent=door_root)
        hinge["dayz_selection"] = "door"

    door_root.rotation_mode = "XYZ"
    door_root.rotation_euler.z = 0.0
    door_root.keyframe_insert(data_path="rotation_euler", frame=1, index=2)
    door_root.rotation_euler.z = math.radians(-105.0)
    door_root.keyframe_insert(data_path="rotation_euler", frame=30, index=2)
    if door_root.animation_data and door_root.animation_data.action:
        door_root.animation_data.action.name = "Door_Open_0_to_1"
    scene.frame_start = 1
    scene.frame_end = 30
    scene.frame_set(1)

    # Simple authored proxies for future P3D LOD conversion. The door proxy is
    # parented to the same hinge so projectile and physical obstruction can move.
    box("Geo_Wall_Left", ((-WALL_WIDTH / 2 + opening_left) / 2, 0, WALL_HEIGHT / 2), (WALL_WIDTH / 2 + opening_left, WALL_DEPTH, WALL_HEIGHT), geometry, None)
    box("Geo_Wall_Right", ((opening_right + WALL_WIDTH / 2) / 2, 0, WALL_HEIGHT / 2), (WALL_WIDTH / 2 - opening_right, WALL_DEPTH, WALL_HEIGHT), geometry, None)
    box("Geo_Wall_Header", (0, 0, (DOOR_OPENING_HEIGHT + WALL_HEIGHT) / 2), (DOOR_OPENING_WIDTH, WALL_DEPTH, WALL_HEIGHT - DOOR_OPENING_HEIGHT), geometry, None)
    geo_door = box("Geo_Door", (DOOR_LEAF_WIDTH / 2, 0, DOOR_LEAF_HEIGHT / 2), (DOOR_LEAF_WIDTH, DOOR_LEAF_DEPTH, DOOR_LEAF_HEIGHT), geometry, None, parent=door_root)
    geo_door["dayz_selection"] = "door"
    for src in ("Geo_Wall_Left", "Geo_Wall_Right", "Geo_Wall_Header"):
        original = geometry.objects.get(src)
        for target, prefix in ((fire_geometry, "Fire"), (view_geometry, "View")):
            copy_obj = original.copy()
            copy_obj.data = original.data.copy()
            copy_obj.name = src.replace("Geo", prefix)
            target.objects.link(copy_obj)
    for target, prefix in ((fire_geometry, "Fire"), (view_geometry, "View")):
        copy_obj = geo_door.copy()
        copy_obj.data = geo_door.data.copy()
        copy_obj.name = f"{prefix}_Door"
        target.objects.link(copy_obj)
        copy_obj.parent = door_root
        copy_obj["dayz_selection"] = "door"

    for lod_collection in (geometry, fire_geometry, view_geometry):
        for proxy_obj in lod_collection.objects:
            proxy_obj.hide_render = True
            proxy_obj.hide_set(True)

    add_memory_point("axis_door_start", (hinge_x, hinge_y, 0.10), memory)
    add_memory_point("axis_door_end", (hinge_x, hinge_y, 2.35), memory)
    add_memory_point("snap_left", (-WALL_WIDTH / 2, 0, 0), memory)
    add_memory_point("snap_right", (WALL_WIDTH / 2, 0, 0), memory)
    add_memory_point("snap_floor_bottom", (0, 0, 0), memory)
    add_memory_point("snap_floor_top", (0, 0, WALL_HEIGHT), memory)

    # Preview ground, excluded from the asset collections used for export.
    ground_mat = bpy.data.materials.new("PreviewGround")
    ground_mat.diffuse_color = (0.09, 0.105, 0.11, 1.0)
    box("Preview_Ground", (0, 0, -0.055), (7.5, 5.5, 0.10), preview, ground_mat, bevel=0.02)

    camera_data = bpy.data.cameras.new("PreviewCamera")
    camera = bpy.data.objects.new("PreviewCamera", camera_data)
    preview.objects.link(camera)
    camera.location = (5.8, -7.2, 4.7)
    target = Vector((0, 0, 2.0))
    camera.rotation_euler = (target - camera.location).to_track_quat("-Z", "Y").to_euler()
    camera_data.lens = 52
    scene.camera = camera

    key_data = bpy.data.lights.new("Key", "AREA")
    key_data.energy = 1350
    key_data.shape = "RECTANGLE"
    key_data.size = 5.0
    key = bpy.data.objects.new("Key", key_data)
    key.location = (-3.5, -4.2, 6.4)
    preview.objects.link(key)
    key.rotation_euler = ((Vector((0, 0, 2.0)) - key.location).to_track_quat("-Z", "Y").to_euler())

    fill_data = bpy.data.lights.new("Fill", "AREA")
    fill_data.energy = 720
    fill_data.size = 4.0
    fill = bpy.data.objects.new("Fill", fill_data)
    fill.location = (4.5, -1.8, 3.5)
    preview.objects.link(fill)
    fill.rotation_euler = ((Vector((0, 0, 2.0)) - fill.location).to_track_quat("-Z", "Y").to_euler())

    scene["SPKZ_model_status"] = "prototype_dimensions_pending_in_game_validation"
    scene["SPKZ_wall_width_m"] = WALL_WIDTH
    scene["SPKZ_wall_height_m"] = WALL_HEIGHT
    scene["SPKZ_door_opening_width_m"] = DOOR_OPENING_WIDTH
    scene["SPKZ_door_opening_height_m"] = DOOR_OPENING_HEIGHT
    scene["SPKZ_animation_selection"] = "door"
    scene["SPKZ_animation_axis"] = "axis_door_start axis_door_end"

    blend_path = os.path.join(OUT_DIR, "SPKZ_WoodDoorWall.blend")
    bpy.ops.wm.save_as_mainfile(filepath=blend_path)

    scene.render.filepath = os.path.join(PREVIEW_DIR, "SPKZ_WoodDoorWall_closed.png")
    scene.frame_set(1)
    bpy.ops.render.render(write_still=True)
    scene.render.filepath = os.path.join(PREVIEW_DIR, "SPKZ_WoodDoorWall_open.png")
    scene.frame_set(30)
    bpy.ops.render.render(write_still=True)
    scene.frame_set(1)
    image.filepath = "//textures/spkz_clean_wood_d.png"
    bpy.ops.wm.save_as_mainfile(filepath=blend_path)

    print(f"CREATED {blend_path}")


build_model()
