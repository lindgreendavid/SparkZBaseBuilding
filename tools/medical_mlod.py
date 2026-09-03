import struct
from pathlib import Path

def read(path):
    data=Path(path).read_bytes(); assert data[:4]==b'MLOD'
    count=struct.unpack_from('<I',data,8)[0]; cursor=12; lods=[]
    def string(pos):
        end=data.index(b'\0',pos); return data[pos:end].decode('ascii'),end+1
    for _ in range(count):
        start=cursor; assert data[cursor:cursor+4]==b'P3DM'
        np,nn,nf=struct.unpack_from('<III',data,cursor+12); cursor+=28
        points=[struct.unpack_from('<fffI',data,cursor+i*16) for i in range(np)]; cursor+=np*16
        normals=[struct.unpack_from('<fff',data,cursor+i*12) for i in range(nn)]; cursor+=nn*12
        faces=[]
        for j in range(nf):
            n=struct.unpack_from('<I',data,cursor)[0]; cursor+=4
            corners=[struct.unpack_from('<IIff',data,cursor+i*16) for i in range(4)]; cursor+=64
            flags=struct.unpack_from('<I',data,cursor)[0]; cursor+=4
            tex,cursor=string(cursor); mat,cursor=string(cursor)
            faces.append((corners[:n],flags,tex,mat))
        assert data[cursor:cursor+4]==b'TAGG';cursor+=4;tags=[]
        while True:
            active=data[cursor];cursor+=1;name,cursor=string(cursor)
            size=struct.unpack_from('<I',data,cursor)[0];cursor+=4
            payload=data[cursor:cursor+size];cursor+=size;tags.append((active,name,payload))
            if name=='#EndOfFile#':break
        res=struct.unpack_from('<f',data,cursor)[0];cursor+=4
        lods.append(dict(start=start,end=cursor,points=points,normals=normals,faces=faces,tags=tags,res=res,raw=data[start:cursor]))
    assert cursor==len(data)
    return data,lods

def write_visual(points,normals,faces,selections):
    out=bytearray(b'P3DM'+struct.pack('<IIIIII',28,256,len(points),len(normals),len(faces),0))
    for p in points:out.extend(struct.pack('<fffI',*p,0))
    for n in normals:out.extend(struct.pack('<fff',*n))
    for corners,flags,tex,mat in faces:
        out.extend(struct.pack('<I',len(corners)))
        for c in corners+[(0,0,0.,0.)]*(4-len(corners)):out.extend(struct.pack('<IIff',*c))
        out.extend(struct.pack('<I',flags)+tex.encode('ascii')+b'\0'+mat.encode('ascii')+b'\0')
    out.extend(b'TAGG')
    for name,(vp,fp) in selections.items():
        payload=bytes(1 if i in vp else 0 for i in range(len(points)))+bytes(1 if i in fp else 0 for i in range(len(faces)))
        out.extend(b'\x01'+name.encode()+b'\0'+struct.pack('<I',len(payload))+payload)
    out.extend(b'\x01#EndOfFile#\0'+struct.pack('<I',0)+struct.pack('<f',1.))
    return out

if __name__=='__main__':
    import os
    _,lods=read(os.environ['INSPECT_P3D'])
    for l in lods:
        print('LOD',l['res'],len(l['points']),len(l['faces']),[(n,len(p)) for _,n,p in l['tags']])
        print('MATERIALS',set((f[2],f[3]) for f in l['faces']))
