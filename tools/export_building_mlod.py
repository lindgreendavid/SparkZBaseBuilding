import bpy
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ADDON = ROOT / "SparkZBaseBuilding"
BLEND = ADDON / "Source" / "WoodDoorWall" / "SPKZ_WoodDoorWall.blend"
OUT = ADDON / "Data" / "WoodDoorWall" / "SPKZ_WoodDoorWall.p3d"
PLAIN = '--plain' in sys.argv
FLOOR = '--floor' in sys.argv
WINDOW = '--window' in sys.argv
GLASS = '--glass' in sys.argv
GARAGE = '--garage' in sys.argv
if GARAGE:
    BLEND=ADDON/'Source/WoodGarage/SPKZ_WoodGarage.blend'
    OUT=ADDON/'Data/WoodGarage/SPKZ_WoodGarage.p3d'
if GLASS:
    BLEND = ADDON / 'Source/WoodGlassWindow/SPKZ_WoodGlassWindow.blend'
    OUT = ADDON / 'Data/WoodGlassWindow/SPKZ_WoodGlassWindow.p3d'
if WINDOW:
    BLEND = ADDON / 'Source' / 'WoodWindow' / 'SPKZ_WoodWindow.blend'
    OUT = ADDON / 'Data' / 'WoodWindow' / 'SPKZ_WoodWindow.p3d'
if PLAIN:
    BLEND = ADDON / 'Source' / 'WoodWall' / 'SPKZ_WoodWall.blend'
    OUT = ADDON / 'Data' / 'WoodWall' / 'SPKZ_WoodWall.p3d'
if FLOOR:
    BLEND = ADDON / 'Source' / 'WoodFloor' / 'SPKZ_WoodFloor.blend'
    OUT = ADDON / 'Data' / 'WoodFloor' / 'SPKZ_WoodFloor.p3d'
sys.path.insert(0, str(Path(__file__).resolve().parent))
from medical_mlod import write_visual

LOD_GEOMETRY = 9999999827968.0
LOD_MEMORY = 999999986991104.0
LOD_LANDCONTACT = 1999999973982208.0
LOD_VIEW = 6000000056164352.0
LOD_FIRE = 6999999976046592.0

bpy.ops.wm.open_mainfile(filepath=str(BLEND))
bpy.context.scene.frame_set(1)
if FLOOR:
    for collection in bpy.data.collections:
        collection.hide_viewport = False

def encode(points, normals, faces, selections, resolution, properties=None, masses=None):
    raw = write_visual(points, normals, faces, selections)
    end = raw.index(b'\x01#EndOfFile#\0')
    tags = bytearray()
    if masses:
        payload = b''.join(struct.pack('<f', value) for value in masses)
        tags.extend(b'\x01#Mass#\0' + struct.pack('<I', len(payload)) + payload)
    for key, value in (properties or {}).items():
        payload = key.encode().ljust(64, b'\0') + value.encode().ljust(64, b'\0')
        tags.extend(b'\x01#Property#\0' + struct.pack('<I', len(payload)) + payload)
    raw = raw[:end] + tags + raw[end:]
    return raw[:-4] + struct.pack('<f', resolution)

def collection_names(obj):
    return {collection.name for collection in obj.users_collection}

def material_paths(obj, mesh, tri):
    mat = mesh.materials[tri.material_index] if tri.material_index < len(mesh.materials) else None
    name = mat.name if mat else ''
    if name=='SPKZ_Bulletproof_Glass':
        return '#(argb,8,8,3)color(0.22,0.29,0.30,0.14,CA)', 'SparkZBaseBuilding\\Data\\WoodGlassWindow\\glass.rvmat'
    if name.startswith('SPKZ_Floor_Plank_'):
        index = name.rsplit('_', 1)[-1]
        return f"SparkZBaseBuilding\\Data\\WoodFloor\\textures\\spkz_floor_plank_{index}_co.paa", ''
    if name.startswith('SPKZ_Plank_Timber_Variant_'):
        index = name.rsplit('_', 1)[-1]
        return f"SparkZBaseBuilding\\Data\\WoodDoorWall\\textures\\spkz_plank_variant_{index}_d.paa", ''
    return "SparkZBaseBuilding\\Data\\WoodDoorWall\\textures\\spkz_dark_structural_timber_d.paa", ''

def mesh_lod(objects, resolution, textured=False, mass_total=0.0):
    points=[]; normals=[]; faces=[]; selection_points={}; selection_faces={}
    component_index=0
    for obj in objects:
        deps=bpy.context.evaluated_depsgraph_get()
        evaluated=obj.evaluated_get(deps)
        mesh=bpy.data.meshes.new_from_object(evaluated, preserve_all_data_layers=True, depsgraph=deps)
        mesh.transform(evaluated.matrix_world)
        mesh.calc_loop_triangles()
        base=len(points); face_base=len(faces)
        points.extend((v.co.x, v.co.z, v.co.y) for v in mesh.vertices)
        uv=mesh.uv_layers.active
        for tri in mesh.loop_triangles:
            corners=[]
            for li in tri.loops:
                loop=mesh.loops[li]; n=mesh.corner_normals[li].vector
                ni=len(normals); normals.append((-n.x,-n.z,-n.y))
                u,v=uv.data[li].uv if uv else (0.0,0.0)
                corners.append((base+loop.vertex_index,ni,float(u),float(1-v)))
            tex,mat=material_paths(obj,mesh,tri) if textured else ('','')
            if GLASS and resolution==LOD_FIRE:
                mat='SparkZBaseBuilding\\Data\\WoodGlassWindow\\bulletproof.rvmat'
            faces.append((corners,0,tex,mat))
        selection = 'door' if obj.get('dayz_selection') == 'door' or obj.name.endswith('_Door') else 'wall'
        if obj.get('dayz_selection') in {'window_left','window_right','glass','garage_panel'}:
            selection=obj['dayz_selection']
        selection_points.setdefault(selection,set()).update(range(base,len(points)))
        selection_faces.setdefault(selection,set()).update(range(face_base,len(faces)))
        if not textured:
            component_index += 1
            component_name=f"Component{component_index:02d}"
            selection_points[component_name]=set(range(base,len(points)))
            selection_faces[component_name]=set(range(face_base,len(faces)))
        bpy.data.meshes.remove(mesh)
    selections={name:(selection_points[name],selection_faces[name]) for name in selection_points}
    masses=[mass_total/len(points)]*len(points) if mass_total and points else None
    properties={'autocenter':'0'} if resolution == LOD_GEOMETRY else None
    return encode(points,normals,faces,selections,resolution,properties,masses)

excluded={'Geometry','Fire Geometry','View Geometry','Roadway','PREVIEW_ONLY'}
visual=[o for o in bpy.data.objects if o.type=='MESH' and o.get('dayz_selection') in {'wall','door','window_left','window_right','glass','garage_panel'} and not collection_names(o).intersection(excluded)]
if GLASS:
    visual.sort(key=lambda o:o.get('dayz_selection')=='glass')
geometry=[o for o in bpy.data.objects if o.type=='MESH' and 'Geometry' in collection_names(o)]
view=[o for o in bpy.data.objects if o.type=='MESH' and 'View Geometry' in collection_names(o)]
fire=[o for o in bpy.data.objects if o.type=='MESH' and 'Fire Geometry' in collection_names(o)]

if GARAGE:
    memory_points=[];memory_selections={}
    for axis in ['garage_axis','garage_lift']:
        indices=set()
        for suffix in ['start','end']:
            p=bpy.data.objects[axis+'_'+suffix].matrix_world.translation
            indices.add(len(memory_points));memory_points.append((p.x,p.z,p.y))
        memory_selections[axis]=(indices,set())
    # The lift parent carries the child's hinge axis, not the fixed lift axis.
    memory_selections['garage_carriage']=({0,1},set())
    memory=encode(memory_points,[],[],memory_selections,LOD_MEMORY)
elif WINDOW:
    memory_points=[]
    memory_selections={}
    for side in ['left','right']:
        indices=set()
        for suffix in ['start','end']:
            p=bpy.data.objects['axis_window_'+side+'_'+suffix].matrix_world.translation
            indices.add(len(memory_points))
            memory_points.append((p.x,p.z,p.y))
        memory_selections['window_'+side+'_axis']=(indices,set())
    memory=encode(memory_points,[],[],memory_selections,LOD_MEMORY)
elif PLAIN or FLOOR or GLASS:
    memory_points=[]
    memory_selections={}
    names = ['snap_left','snap_right','snap_front','snap_back'] if FLOOR else ['snap_left','snap_right','snap_floor_bottom','snap_floor_top']
    for name in names:
        point=bpy.data.objects[name].matrix_world.translation
        memory_selections[name]=({len(memory_points)},set())
        memory_points.append((point.x,point.z,point.y))
    memory=encode(memory_points,[],[],memory_selections,LOD_MEMORY)
else:
    axis_start=bpy.data.objects['axis_door_start'].matrix_world.translation
    axis_end=bpy.data.objects['axis_door_end'].matrix_world.translation
    memory_points=[(axis_start.x,axis_start.z,axis_start.y),(axis_end.x,axis_end.z,axis_end.y)]
    memory=encode(memory_points,[],[],{'door_axis':(set(range(2)),set())},LOD_MEMORY)

collision_points=[obj.matrix_world @ vertex.co for obj in geometry for vertex in obj.data.vertices]
left=min(p.x for p in collision_points); right=max(p.x for p in collision_points)
front=min(p.y for p in collision_points); back=max(p.y for p in collision_points)
land_points=[(left,0,front),(right,0,front),(left,0,back),(right,0,back)]
if FLOOR:
    land_points=[(x,-0.2,z) for x,y,z in land_points]
land=encode(land_points,[],[],{},LOD_LANDCONTACT)

lods=[
    mesh_lod(visual,0.0,True),
    mesh_lod(geometry,LOD_GEOMETRY,False,1800.0),
    memory,
    land,
    mesh_lod(view,LOD_VIEW),
    mesh_lod(fire,LOD_FIRE),
]
if FLOOR:
    roadway=[o for o in bpy.data.objects if o.type=='MESH' and 'Roadway' in collection_names(o)]
    lods.append(mesh_lod(roadway,3e15))
OUT.parent.mkdir(parents=True,exist_ok=True)
OUT.write_bytes(b'MLOD'+struct.pack('<II',257,len(lods))+b''.join(lods))
print(f"Wrote {OUT} with {len(lods)} LODs")
