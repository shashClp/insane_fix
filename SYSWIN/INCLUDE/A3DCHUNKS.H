// A3D - Anaconda 3D File System
// Anaconda 3D File (Chunks)

#ifndef A3D_CHUNKS
#define A3D_CHUNKS

#define CHUNK_MESH					0x1000    // <---
#define CHUNK_VERTEX				0x1001
#define CHUNK_FACES					0x1002
#define CHUNK_PLANES				0x1003
#define CHUNK_NORMALS				0x1004
#define CHUNK_UVCOORDS			0x1005
#define CHUNK_COLOR					0x1006
#define CHUNK_INITPOS				0x1007
#define CHUNK_INITROT				0x1008
#define CHUNK_INITSCA				0x1009
#define CHUNK_TRMATRIX			0x1010
#define CHUNK_OBJOFFMATRIX	0x1011

#define CHUNK_CAMERA				0x2000    // <---
#define CHUNK_TARGET				0x2001

#define CHUNK_LIGHT					0x3000    // <---

#define CHUNK_MATERIAL			0x4000    // <---

#define CHUNK_SPLINE_POS		0x9001
#define CHUNK_POSITION			0x9002
#define CHUNK_SPLINE_ROT		0x9010
#define CHUNK_SPLINE_SCA		0x9020
#define CHUNK_SPLINE_ROLL		0x9030
#define CHUNK_ROLL					0x9031
#define CHUNK_SAMPLE_ANIM		0x9040

#endif

