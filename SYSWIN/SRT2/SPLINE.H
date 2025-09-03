////////////////////////////////////////////////////////////////////////////
//
//  SPLINE.H v.0.1. - By Klauz / Anaconda. April 1999.
//
//  Library for spline calculation.
//
//  Nota!!!: No hi ha control d'errors. Buscar VAR_ERR_Buffer per
//           incloure'n
//
////////////////////////////////////////////////////////////////////////////

#ifndef LIBRARY_SPLINE
#define LIBRARY_SPLINE

#include <library\vertex.h>
#include <library\matrix.h>
#include <library\quat.h>

#define CSPLINE_STOP        0x0001
#define CSPLINE_LOOP        0x0002
#define CSPLINE_GLOBAL_LOOP 0x0004

class CKey
{
public:
  TVertex Position;
  TVertex DS, DD;

  CQuat Quaternion, QA;
  CQuat DSQ, DDQ;

  float Value;

  // Splines values.
  float Tens, Cont, Bias, EaseTo, EaseFrom;

  word Frame;
  CKey *Next;

  CKey ( void );
  ~CKey ( void ) { }

  CKey &operator=( CKey const &K );
};

class CSpline
{
public:
  word NumKeys, LastFrame;
  CKey *Keys, *Last;

  CSpline ( void );
  ~CSpline ( void ) { }
  CSpline &operator=( CSpline const &S );

  void Free ( void );

  void AddKey ( CKey key );

  void InitSpline ( void );
  void InitRotationSpline ( void );

  float   GetRealFrameValues ( word Frame, long flags );
  TVertex GetVertexFrameValues ( word Frame, long flags );
  TMatrix GetQuaternionFrameValues ( word Frame, long flags );

private:
  void CompElementDeriv ( float  pp, float   p, float  pn,
                          float &ds, float &dd, float ksm,
                          float ksp, float kdm, float kdp );
  void CompDeriv ( CKey &keyp, CKey &key, CKey &keyn );
  void CompDerivFirst ( CKey &key, CKey &keyn, CKey &keynn );
  void CompDerivLast ( CKey &keypp, CKey &keyp, CKey &key );
  void CompDerivTwo (CKey &key, CKey &keyn);
  void CompDerivAB ( CKey *keyp, CKey *key, CKey *keyn );
  float SplineEase (float t, float a, float b);
};

#endif

////////////////////////////////////////////////////////////////////////////
// Public Key functions ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

CKey::CKey ( void )
{
  Position.x = Position.y = Position.z = 0.0;
  DS.x = DS.y = DS.z = 0.0;
  DD.x = DD.y = DD.z = 0.0;

  Quaternion.SetElements(0, 0, 0, 0);
  QA.SetElements(0, 0, 0, 0);
  DSQ.SetElements(0, 0, 0, 0);
  DDQ.SetElements(0, 0, 0, 0);

  Value = 0;

  Tens = Cont = Bias = EaseTo = EaseFrom = 0;

  Frame = 0;
  Next = NULL;
}

CKey &CKey::operator=( CKey const &K )
{
  Position = K.Position;
  DS = K.DS;
  DD = K.DD;
  Quaternion = K.Quaternion;
  QA = K.QA;
  DSQ = K.DSQ;
  DDQ = K.DDQ;
  Value = K.Value;
  Tens = K.Tens;
  Cont = K.Cont;
  Bias = K.Bias;
  EaseTo = K.EaseTo;
  EaseFrom = K.EaseFrom;
  Frame = K.Frame;
  Next = K.Next;

  return *this;
}

////////////////////////////////////////////////////////////////////////////
// Public Spline functions /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
CSpline::CSpline ( void ) { NumKeys = LastFrame = 0; Keys = Last = NULL; }

CSpline &CSpline::operator=( CSpline const &S )
{
CKey *Aux;

  this->Free();

  Aux = S.Keys;
  while (Aux) { this->AddKey(*Aux); Aux = Aux->Next; }

  return *this;
}

void CSpline::Free ( void )
{
CKey *Aux;

  Aux = Keys;
  while (Aux)
  {
    Keys = Keys->Next;
    Aux->Next = NULL;
    delete Aux;
    Aux = Keys;
  }
  Keys = Last = NULL;
  NumKeys = 0;
}

void CSpline::AddKey ( CKey Key )
{
CKey *New;

  New = new CKey;
  *New = Key;
  if (!Keys) Keys = New;
  else Last->Next = New;
  Last = New;

  NumKeys++;
}

void CSpline::InitSpline ( void )
{
word i = 0, Last3;
CKey *Aux1, *Aux2, *Aux3;

  if (!NumKeys)
  {
    return;
    //sprintf(VAR_ERR_Buffer, "Can not initialize an empty spline.");
    //FUN_ERR_Error(34, VAR_ERR_Buffer);
  }
  if (NumKeys == 1) return;
  if (NumKeys == 2)
  {
    CompDerivTwo(*Keys, *(Keys->Next));
    return;
  }

  Aux1 = Keys; Aux2 = Aux1->Next; Aux3 = Aux2->Next;
  while (Aux3)
  {
    CompDeriv(*Aux1, *Aux2, *Aux3);
    Aux1 = Aux2;
    Aux2 = Aux1->Next;
    if (Aux2->Next) Aux3 = Aux2->Next;
    else Aux3 = NULL;
  }

  CompDerivFirst(*(Keys), *(Keys->Next), *(Keys->Next->Next));

  // To get the last three keys.
  Aux1 = Keys;
  Last3 = NumKeys - 3;
  while (i < Last3) { Aux1 = Aux1->Next; i++; }
  Aux2 = Aux1->Next; Aux3 = Aux2->Next;
  CompDerivLast(*Aux1, *Aux2, *Aux3);
}

void CSpline::InitRotationSpline ( void )
{
word i = 0, Last3;
CKey *Aux1, *Aux2, *Aux3;

  if (!NumKeys)
  {
    //sprintf(VAR_ERR_Buffer, "Can not initialize rotation spline (empty).");
    //FUN_ERR_Error(34, VAR_ERR_Buffer);
  }
  if (NumKeys == 2)
  {
    //sprintf(VAR_ERR_Buffer, "Can not initialize rotation spline (2 keys).");
    //FUN_ERR_Error(34, VAR_ERR_Buffer);
  }
  if (NumKeys == 1) return;

  Aux1 = Keys; Aux2 = Aux1->Next; Aux3 = Aux2->Next;
  while (Aux3)
  {
    CompDerivAB(Aux1, Aux2, Aux3);
    Aux1 = Aux2;
    Aux2 = Aux1->Next;
    if (Aux2->Next) Aux3 = Aux2->Next;
    else Aux3 = NULL;
  }

  CompDerivAB (NULL, Keys, Keys->Next);
  CompDerivAB (Keys, Last, NULL);
}

float CSpline::GetRealFrameValues( word Frame, long flags )
{
float R;
CKey *key, *prevkey;
float t1, t2, t3, h[4];

  if (!NumKeys)
  {
    //sprintf(VAR_ERR_Buffer, "Can not get real frame values (empty spline).");
    //FUN_ERR_Error(34, VAR_ERR_Buffer);
  }
  if (NumKeys == 1) return Keys->Value;

  if ((Frame > Last->Frame) && (flags & CSPLINE_STOP)) return Last->Value;

  if (flags & CSPLINE_LOOP) Frame %= Last->Frame;
  if (flags & CSPLINE_GLOBAL_LOOP)
  {
    Frame %= LastFrame;
    if (Frame > Last->Frame) return Last->Value;
  }

  prevkey = Keys;
  key = prevkey->Next;
  while ((key->Frame < Frame) && (key->Next)) { prevkey = key; key = key->Next; }

  t1 = (float) (Frame - prevkey->Frame) / (float) (key->Frame - prevkey->Frame);
  t1 = SplineEase(t1, prevkey->EaseFrom, key->EaseTo);
  t2 = t1 * t1;
  t3 = t2 * t1;
  h[0] = 2 * t3 - 3 * t2 + 1;
  h[1] = -2 * t3 + 3 * t2;
  h[2] = t3 - 2 * t2 + t1;
  h[3] = t3 - t2;

  R = (h[0]*prevkey->Value) + (h[1]*key->Value) +
      (h[2]*prevkey->DD.x)  + (h[3]*key->DS.x);

  return R;
}

TVertex CSpline::GetVertexFrameValues( word Frame, long flags )
{
TVertex R;
CKey *key, *prevkey;
float t1, t2, t3, h[4];

  if (!NumKeys)
  {
    //sprintf(VAR_ERR_Buffer, "Can not get vertex frame values (empty spline).");
    //FUN_ERR_Error(34, VAR_ERR_Buffer);
  }
  if (NumKeys == 1) return Keys->Position;

  if ((Frame > Last->Frame) && (flags & CSPLINE_STOP)) return Last->Position;

  if (flags & CSPLINE_LOOP) Frame %= Last->Frame;
  if (flags & CSPLINE_GLOBAL_LOOP)
  {
    Frame %= LastFrame;
    if (Frame > Last->Frame) return Last->Position;
  }

  prevkey = Keys;
  key = prevkey->Next;
  while ((key->Frame < Frame) && (key->Next)) { prevkey = key; key = key->Next; }

  t1  = (float) (Frame - prevkey->Frame) / (float) (key->Frame - prevkey->Frame);
  t1 = SplineEase(t1, prevkey->EaseFrom, key->EaseTo);
  t2 = t1 * t1;
  t3 = t2 * t1;
  h[0] = 2 * t3 - 3 * t2 + 1;
  h[1] = -2 * t3 + 3 * t2;
  h[2] = t3 - 2 * t2 + t1;
  h[3] = t3 - t2;

  R.x = (h[0]*prevkey->Position.x) + (h[1]*key->Position.x) +
        (h[2]*prevkey->DD.x) + (h[3]*key->DS.x);
  R.y = (h[0]*prevkey->Position.y) + (h[1]*key->Position.y) +
        (h[2]*prevkey->DD.y) + (h[3]*key->DS.y);
  R.z = (h[0]*prevkey->Position.z) + (h[1]*key->Position.z) +
        (h[2]*prevkey->DD.z) + (h[3]*key->DS.z);

  return R;
}

TMatrix CSpline::GetQuaternionFrameValues( word Frame, long flags )
{
TMatrix M;
CQuat a, b;
CKey *key, *prevkey;
float t, angle, spin;

  if (!NumKeys)
  {
    //sprintf(VAR_ERR_Buffer, "Can not get quaternion frame values (empty spline).");
    //FUN_ERR_Error(34, VAR_ERR_Buffer);
  }

  if (NumKeys == 1) return Keys->QA.InvRotationMatrix();

  if ((Frame > Last->Frame) && (flags & CSPLINE_STOP)) return Last->QA.InvRotationMatrix();

  if (flags & CSPLINE_LOOP) Frame %= Last->Frame;
  if (flags & CSPLINE_GLOBAL_LOOP)
  {
    Frame %= LastFrame;
    if (Frame > Last->Frame) return Last->QA.InvRotationMatrix();
  }

  prevkey = Keys;
  key = prevkey->Next;
  while ((key->Frame < Frame) && (key->Next)) { prevkey = key; key = key->Next; }

  t = (float) (Frame - prevkey->Frame) / (float) (key->Frame - prevkey->Frame);
  t = SplineEase(t, prevkey->EaseFrom, key->EaseTo);
  a = prevkey->QA;

  angle = key->Quaternion.w - prevkey->Quaternion.w;
  if (angle > 0) spin = floor (angle / M_PIx2);
  else spin = ceil (angle / M_PIx2);

  a.SphereInterpol(key->QA, spin, t);
  b = prevkey->DDQ;
  b.SphereInterpol(key->DSQ, spin, t);

  t = (((1.0-t)*2.0)*t);
  a.SphereInterpol(b, 0, t);

  return (a.InvRotationMatrix());
}

////////////////////////////////////////////////////////////////////////////
// Private Spline functions ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void CSpline::CompElementDeriv ( float  pp, float   p, float  pn,
                                 float &ds, float &dd, float ksm,
                                 float ksp, float kdm, float kdp )
{
  float delm = p - pp;
  float delp = pn - p;

  ds = ksm * delm + ksp * delp;
  dd = kdm * delm + kdp * delp;
}

void CSpline::CompDeriv ( CKey &keyp, CKey &key, CKey &keyn )
{
float tm, cm, cp, bm, bp, tmcm, tmcp;
float ksm, ksp, kdm, kdp;
float dt, fp, fn, c;

  dt = 0.5 * (keyn.Frame - keyp.Frame);
  fp = (key.Frame - keyp.Frame) / dt;
  fn = (keyn.Frame - key.Frame) / dt;
  c = fabs(key.Cont);
  fp = fp + c  - c * fp;
  fn = fn + c  - c * fn;
  cm = 1.0 - key.Cont;
  tm = 0.5 * (1.0 - key.Tens);
  cp = 2.0 - cm;
  bm = 1.0 - key.Bias;
  bp = 2.0 - bm;
  tmcm = tm * cm;
  tmcp = tm * cp;
  ksm = tmcm * bp * fp; ksp = tmcp * bm * fp;
  kdm = tmcp * bp * fn; kdp = tmcm * bm * fn;

  CompElementDeriv(keyp.Position.x, key.Position.x, keyn.Position.x,
                   key.DS.x, key.DD.x, ksm, ksp, kdm, kdp );

  CompElementDeriv(keyp.Position.y, key.Position.y, keyn.Position.y,
                   key.DS.y, key.DD.y, ksm, ksp, kdm, kdp );

  CompElementDeriv(keyp.Position.z, key.Position.z, keyn.Position.z,
                   key.DS.z, key.DD.z, ksm, ksp, kdm, kdp );
}

void CSpline::CompDerivFirst ( CKey &key, CKey &keyn, CKey &keynn )
{
float f20, f10, v20, v10;

  f10 = keyn.Frame - key.Frame;
  f20 = keynn.Frame - key.Frame;

  v10 = keyn.Position.x - key.Position.x;
  v20 = keynn.Position.x - key.Position.x;
  key.DD.x = (v20 * (0.25 - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = keyn.Position.y - key.Position.y;
  v20 = keynn.Position.y - key.Position.y;
  key.DD.y = (v20 * (0.25 - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = keyn.Position.z - key.Position.z;
  v20 = keynn.Position.z - key.Position.z;
  key.DD.z = (v20 * (0.25 - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);
}

void CSpline::CompDerivLast ( CKey &keypp, CKey &keyp, CKey &key )
{
float f20, f10, v20, v10;

  f10 = key.Frame - keyp.Frame;
  f20 = key.Frame - keypp.Frame;

  v10 = key.Position.x - keyp.Position.x;
  v20 = key.Position.x - keypp.Position.x;
  key.DS.x = (v20 * (0.25 - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = key.Position.y - keyp.Position.y;
  v20 = key.Position.y - keypp.Position.y;
  key.DS.y = (v20 * (0.25 - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = key.Position.z - keyp.Position.z;
  v20 = key.Position.z - keypp.Position.z;
  key.DS.z = (v20 * (0.25 - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);
}

void CSpline::CompDerivTwo (CKey &key, CKey &keyn)
{
  key.DS.x = key.DS.y = key.DS.z = 0.0;
  key.DD.x = (keyn.Position.x - key.Position.x) * (1.0 - key.Tens);
  key.DD.y = (keyn.Position.y - key.Position.y) * (1.0 - key.Tens);
  key.DD.z = (keyn.Position.z - key.Position.z) * (1.0 - key.Tens);

  keyn.DD.x = keyn.DD.y = keyn.DD.z = 0.0;
  keyn.DS.x = (keyn.Position.x - key.Position.x) * (1.0 - keyn.Tens);
  keyn.DS.y = (keyn.Position.y - key.Position.y) * (1.0 - keyn.Tens);
  keyn.DS.z = (keyn.Position.z - key.Position.z) * (1.0 - keyn.Tens);
}

void CSpline::CompDerivAB (CKey *keyp, CKey *key, CKey *keyn)
{
CQuat q, qaux;
CQuat qp, qm, qa, qb;
CQuat QA, QB, QC;
CQuat QAA, QAB, QAC;
float tm, cm, cp, bm, bp, tmcm, tmcp, ksm, ksp, kdm, kdp;
float dt, fp, fn, c;

  QAB = key->Quaternion;
  QB = key->QA;
  if (keyp)
  {
    QAA = keyp->Quaternion;
    QA = keyp->QA;
  }
  if (keyn)
  {
    QAC = keyn->Quaternion;
    QC = keyn->QA;
  }

  if (keyp)
  {
    if (fabs (QAB.w - QAA.w) > M_PIx2 - CQUAT_EPSILON)
    {
      qm = QAB;
      qm.w = 0.0;
      qm.Log();
    }
    else
    {
      q = QA;
      if (q.DotUnitProduct(QB) < 0.0) q.Neg();
      q.LnDif(QB);
      qm = q;
    }
  }
  if (keyn)
  {
    if (fabs (QAC.w - QAB.w) > M_PIx2 - CQUAT_EPSILON)
    {
      qp = QAC;
      qp.w = 0.0;
      qp.Log();
    }
    else
    {
      q = QC;
      if (q.DotUnitProduct(QB) < 0.0) q.Neg();
      qaux = QB;
      qaux.LnDif(q);
      qp = qaux;
    }
  }
  if (keyp == NULL) qm = qp;
  if (keyn == NULL) qp = qm;
  fp = fn = 1.0;
  cm = 1.0 - key->Cont;
  if (keyp && keyn)
  {
    dt = 0.5 * (keyn->Frame - keyp->Frame);
    fp = (key->Frame - keyp->Frame) / dt;
    fn = (keyn->Frame - key->Frame) / dt;
    c = fabs(key->Cont);
    fp = fp + c - c * fp;
    fn = fn + c - c * fn;
  }
  tm = 0.5 * (1.0 - key->Tens);
  cp = 2.0 - cm;
  bm = 1.0 - key->Bias;
  bp = 2.0 - bm;
  tmcm = tm * cm;
  tmcp = tm * cp;
  ksm  = 1.0 - tmcm * bp * fp;
  ksp  = -tmcp * bm * fp;
  kdm  = tmcp * bp * fn;
  kdp  = tmcm * bm * fn - 1.0;
  qa.x = 0.5 * (kdm * qm.x + kdp * qp.x);
  qb.x = 0.5 * (ksm * qm.x + ksp * qp.x);
  qa.y = 0.5 * (kdm * qm.y + kdp * qp.y);
  qb.y = 0.5 * (ksm * qm.y + ksp * qp.y);
  qa.z = 0.5 * (kdm * qm.z + kdp * qp.z);
  qb.z = 0.5 * (ksm * qm.z + ksp * qp.z);
  qa.w = 0.5 * (kdm * qm.w + kdp * qp.w);
  qb.w = 0.5 * (ksm * qm.w + ksp * qp.w);

  qa.Exp();
  qb.Exp();
  key->DSQ = QB * qa;
  key->DDQ = QB * qb;
}

float CSpline::SplineEase (float t, float a, float b)
{
float k;
float s = a+b;

  if (s == 0.0) return t;
  if (s > 1.0)
  {
    a = a/s;
    b = b/s;
  }
  k = 1.0/(2.0-a-b);
  if (t < a) return ((k/a)*t*t);
  else
  {
    if (t < 1.0-b) return (k*(2*t-a));
    else
    {
      t = 1.0-t;
      return (1.0-(k/b)*t*t);
    }
  }
}

////////////////////////////////////////////////////////////////////////////

