//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Scene
#define SystemR_V30_Scene

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

#define NumMaxCreatedObjectes 2000

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\ROBJECTS\ROBJECT.H"

#ifdef PlaneOK
#include "..\ROBJECTS\RPLANE.H"
extern TObject_R *OPlanes[NumMaxCreatedObjectes];
#endif

#ifdef SphereOK
#include "..\ROBJECTS\RSPHERE.H"
extern TObject_R *OSpheres[NumMaxCreatedObjectes];
#endif

#ifdef EllipseOK
#include "..\ROBJECTS\RELLIPSE.H"
extern TObject_R *OEllipses[NumMaxCreatedObjectes];
#endif

#ifdef CylinderOK
#include "..\ROBJECTS\RCYLINDE.H"
extern TObject_R *OCylinders[NumMaxCreatedObjectes];
#endif

#ifdef CubeOK
#include "..\ROBJECTS\RCUBE.H"
extern TObject_R *OCubes[NumMaxCreatedObjectes];
#endif

#ifdef BlobOK
#include "..\ROBJECTS\RBLOB.H"
extern TObject_R *OBlobs[NumMaxCreatedObjectes];
#endif

#ifdef CSGOK
#include "..\ROBJECTS\RCSG.H"
extern TObject_R *OCSG[NumMaxCreatedObjectes];
#endif

#ifdef SorOK
#include "..\ROBJECTS\RSOR.H"
extern TObject_R *OSor[NumMaxCreatedObjectes];
#endif

#ifdef HFieldOK
#include "..\ROBJECTS\RHFIELD.H"
extern TObject_R *OHField[NumMaxCreatedObjectes];
#endif

extern void MakeObject (OBjType Type, long Num);
extern void DestroyObject (OBjType Type, long Num);
#endif
