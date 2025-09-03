//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

#define NumMaxCreatedObjectes 2000

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\ROBJECTS\ROBJECT.H"

#ifdef PlaneOK
#include "..\ROBJECTS\RPLANE.H"
TObject_R *OPlanes[NumMaxCreatedObjectes];
#endif

#ifdef SphereOK
#include "..\ROBJECTS\RSPHERE.H"
TObject_R *OSpheres[NumMaxCreatedObjectes];
#endif

#ifdef EllipseOK
#include "..\ROBJECTS\RELLIPSE.H"
TObject_R *OEllipses[NumMaxCreatedObjectes];
#endif

#ifdef CylinderOK
#include "..\ROBJECTS\RCYLINDE.H"
TObject_R *OCylinders[NumMaxCreatedObjectes];
#endif

#ifdef CubeOK
#include "..\ROBJECTS\RCUBE.H"
TObject_R *OCubes[NumMaxCreatedObjectes];
#endif

#ifdef BlobOK
#include "..\ROBJECTS\RBLOB.H"
TObject_R *OBlobs[NumMaxCreatedObjectes];
#endif

#ifdef CSGOK
#include "..\ROBJECTS\RCSG.H"
TObject_R *OCSG[NumMaxCreatedObjectes];
#endif

#ifdef SorOK
#include "..\ROBJECTS\RSOR.H"
TObject_R *OSor[NumMaxCreatedObjectes];
#endif

#ifdef HFieldOK
#include "..\ROBJECTS\RHFIELD.H"
TObject_R *OHField[NumMaxCreatedObjectes];
#endif

void MakeObject (OBjType Type, long Num)
{

  long Num2 = Num;
  long Cmt;
  if (Num2 >= NumMaxCreatedObjectes) {Num2 = NumMaxCreatedObjectes-1; }

  switch (Type)
  {
#ifdef PlaneOK
    case R_PLANE:
    for (Cmt = 0; Cmt < Num; Cmt++)  OPlanes[Cmt] = CreatePlaneDefault();
        break;
#endif
#ifdef SphereOK
    case R_SPHERE:
    for (Cmt = 0; Cmt < Num; Cmt++)  OSpheres[Cmt] = CreateSphereDefault();
        break;
#endif
#ifdef CylinderOK
    case R_CYLINDER:
    for (Cmt = 0; Cmt < Num; Cmt++)  OCylinders[Cmt] = CreateCylinderDefault();
        break;
#endif
#ifdef EllipseOK
    case R_ELLIPSE:
    for (Cmt = 0; Cmt < Num; Cmt++)  OEllipses[Cmt] = CreateEllipseDefault();
        break;
#endif
#ifdef CubeOK
    case R_CUBE:
    for (Cmt = 0; Cmt < Num; Cmt++)  OCubes[Cmt] = CreateCubeDefault();
        break;
#endif
#ifdef CSGOK
    case R_CSG:
    for (Cmt = 0; Cmt < Num; Cmt++)  OCSG[Cmt] = CreateMinusDefault();
        break;
#endif

#ifdef BlobOK
    case R_BLOB:
    for (Cmt = 0; Cmt < Num; Cmt++)  OBlobs[Cmt] = CreateBlobDefault();
        break;
#endif
#ifdef SorOK
    case R_SOR:
    for (Cmt = 0; Cmt < Num; Cmt++)  OSor[Cmt] = CreateSorDefault();
        break;
#endif

#ifdef HFieldOK
    case R_HFIELD:
    for (Cmt = 0; Cmt < Num; Cmt++)  OHField[Cmt] = CreateHFieldDefault();
        break;
#endif
  };


}

void DestroyObject (OBjType Type, long Num)
{

  long Num2 = Num;
  long Cmt;
  if (Num2 >= NumMaxCreatedObjectes) {Num2 = NumMaxCreatedObjectes-1; }


  switch (Type)
  {
#ifdef PlaneOK
    case R_PLANE:
    for (Cmt = 0; Cmt < Num; Cmt++)
    {
      DestroyPlaneDefault(OPlanes[Cmt]);
      OPlanes[Cmt] = NULL;
    }
        break;
#endif
#ifdef SphereOK
    case R_SPHERE:
    for (Cmt = 0; Cmt < Num; Cmt++)
    {
      DestroySphereDefault(OSpheres[Cmt]);
      OSpheres[Cmt] = NULL;
    }

        break;
#endif
#ifdef CylinderOK
    case R_CYLINDER:
    for (Cmt = 0; Cmt < Num; Cmt++)
    {
      DestroyCylinderDefault(OCylinders[Cmt]);
      OCylinders[Cmt] = NULL;
    }
        break;
#endif
#ifdef EllipseOK
    case R_ELLIPSE:
    for (Cmt = 0; Cmt < Num; Cmt++)
    {
      DestroyEllipseDefault(OEllipses[Cmt]);
      OEllipses[Cmt] = NULL;
    }
        break;
#endif
#ifdef CubeOK
    case R_CUBE:
    for (Cmt = 0; Cmt < Num; Cmt++)
    {
      DestroyCubeDefault(OCubes[Cmt]);
      OCubes[Cmt] = NULL;
    }
        break;
#endif
#ifdef CSGOK
    case R_CSG:
    for (Cmt = 0; Cmt < Num; Cmt++)
      {
        DestroyMinusDefault(OCSG[Cmt]);
        OCSG[Cmt] = NULL;
      }
        break;
#endif

#ifdef BlobOK
    case R_BLOB:
    for (Cmt = 0; Cmt < Num; Cmt++)
    {
      DestroyBlobDefault(OBlobs[Cmt]);
      OBlobs[Cmt] = NULL;
    }
        break;
#endif
#ifdef SorOK
    case R_SOR:
    for (Cmt = 0; Cmt < Num; Cmt++)
    {
      DestroySorDefault(OSor[Cmt]);
      OSor[Cmt] = NULL;
    }
        break;
#endif

  };


}

