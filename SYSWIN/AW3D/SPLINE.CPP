// AW3D - Anaconda 3D Windows Library
// Spline

#include <windows.h>
#include <math.h>
#include <defines.h>
#include <log.h>
#include <mem.h>
#include <spline.h>

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void SPLINE_CompElementDeriv(float  pp, float   p, float  pn,
                             float *ds, float *dd, float ksm,
                             float ksp, float kdm, float kdp)
{
  float delm = p - pp;
  float delp = pn - p;

  *ds = ksm * delm + ksp * delp;
  *dd = kdm * delm + kdp * delp;
}

void SPLINE_CompDeriv(TKey *keyp, TKey *key, TKey *keyn)
{
  float tm, cm, cp, bm, bp, tmcm, tmcp;
  float ksm, ksp, kdm, kdp;
  float dt, fp, fn, c;

  dt = 0.5f * (keyn->frame - keyp->frame);
  fp = (key->frame - keyp->frame) / dt;
  fn = (keyn->frame - key->frame) / dt;
  c = (float) fabs(key->cont);
  fp = fp + c  - c * fp;
  fn = fn + c  - c * fn;
  cm = 1.0f - key->cont;
  tm = 0.5f * (1.0f - key->tens);
  cp = 2.0f - cm;
  bm = 1.0f - key->bias;
  bp = 2.0f - bm;
  tmcm = tm * cm;
  tmcp = tm * cp;
  ksm = tmcm * bp * fp; ksp = tmcp * bm * fp;
  kdm = tmcp * bp * fn; kdp = tmcm * bm * fn;

  SPLINE_CompElementDeriv(keyp->pos.x, key->pos.x, keyn->pos.x,
                          &key->ds.x, &key->dd.x, ksm, ksp, kdm, kdp );

  SPLINE_CompElementDeriv(keyp->pos.y, key->pos.y, keyn->pos.y,
                          &key->ds.y, &key->dd.y, ksm, ksp, kdm, kdp );

  SPLINE_CompElementDeriv(keyp->pos.z, key->pos.z, keyn->pos.z,
                          &key->ds.z, &key->dd.z, ksm, ksp, kdm, kdp );
}

void SPLINE_CompDerivFirst(TKey *key, TKey *keyn, TKey *keynn)
{
  float f20, f10, v20, v10;

  f10 = (float)keyn->frame - key->frame;
  f20 = (float)keynn->frame - key->frame;

  v10 = keyn->pos.x - key->pos.x;
  v20 = keynn->pos.x - key->pos.x;
  key->dd.x = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = keyn->pos.y - key->pos.y;
  v20 = keynn->pos.y - key->pos.y;
  key->dd.y = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = keyn->pos.z - key->pos.z;
  v20 = keynn->pos.z - key->pos.z;
  key->dd.z = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);
}

void SPLINE_CompDerivLast(TKey *keypp, TKey *keyp, TKey *key)
{
  float f20, f10, v20, v10;

  f10 = (float)key->frame - keyp->frame;
  f20 = (float)key->frame - keypp->frame;

  v10 = key->pos.x - keyp->pos.x;
  v20 = key->pos.x - keypp->pos.x;
  key->ds.x = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = key->pos.y - keyp->pos.y;
  v20 = key->pos.y - keypp->pos.y;
  key->ds.y = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = key->pos.z - keyp->pos.z;
  v20 = key->pos.z - keypp->pos.z;
  key->ds.z = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);
}

void SPLINE_CompDerivTwo(TKey *key, TKey *keyn)
{
  VTX_Set(key->ds, 0, 0 ,0);
  key->dd.x = (keyn->pos.x - key->pos.x) * (1.0f - key->tens);
  key->dd.y = (keyn->pos.y - key->pos.y) * (1.0f - key->tens);
  key->dd.z = (keyn->pos.z - key->pos.z) * (1.0f - key->tens);

  VTX_Set(keyn->dd, 0, 0 ,0);
  keyn->ds.x = (keyn->pos.x - key->pos.x) * (1.0f - keyn->tens);
  keyn->ds.y = (keyn->pos.y - key->pos.y) * (1.0f - keyn->tens);
  keyn->ds.z = (keyn->pos.z - key->pos.z) * (1.0f - keyn->tens);
}

void SPLINE_CompDerivAB (TKey *keyp, TKey *key, TKey *keyn)
{
	TQuat q, qaux;
	TQuat qp, qm, qa, qb;
	TQuat QA, QB, QC;
	TQuat QAA, QAB, QAC;
	float tm, cm, cp, bm, bp, tmcm, tmcp, ksm, ksp, kdm, kdp;
	float dt, fp, fn, c;

  QAB = key->quat;
  QB = key->qa;
  if (keyp) {
    QAA = keyp->quat;
    QA = keyp->qa;
  }
  if (keyn) {
    QAC = keyn->quat;
    QC = keyn->qa;
  }
  if (keyp) {
    if (fabs (QAB.w - QAA.w) > M_PIx2 - QUAT_EPSILON) {
      qm = QAB;
      qm.w = 0.0;
      QUAT_Log(qm);
    } else {
      q = QA;
      if (QUAT_DotUnitProduct(q,QB) < 0.0) QUAT_Neg(q);
      QUAT_LnDif(q,QB);
      qm = q;
    }
  }
  if (keyn) {
    if (fabs (QAC.w - QAB.w) > M_PIx2 - QUAT_EPSILON) {
      qp = QAC;
      qp.w = 0.0;
      QUAT_Log(qp);
    } else {
      q = QC;
      if (QUAT_DotUnitProduct(q,QB) < 0.0) QUAT_Neg(q);
      qaux = QB;
      QUAT_LnDif(qaux,q);
      qp = qaux;
    }
  }
  if (keyp == NULL) qm = qp;
  if (keyn == NULL) qp = qm;
  fp = fn = 1.0f;
  cm = 1.0f - key->cont;
  if (keyp && keyn) {
    dt = 0.5f * (keyn->frame - keyp->frame);
    fp = (key->frame - keyp->frame) / dt;
    fn = (keyn->frame - key->frame) / dt;
    c = (float)fabs(key->cont);
    fp = fp + c - c * fp;
    fn = fn + c - c * fn;
  }
  tm = 0.5f * (1.0f - key->tens);
  cp = 2.0f - cm;
  bm = 1.0f - key->bias;
  bp = 2.0f - bm;
  tmcm = tm * cm;
  tmcp = tm * cp;
  ksm  = 1.0f - tmcm * bp * fp;
  ksp  = -tmcp * bm * fp;
  kdm  = tmcp * bp * fn;
  kdp  = tmcm * bm * fn - 1.0f;

  qa.x = 0.5f * (kdm * qm.x + kdp * qp.x);
  qa.y = 0.5f * (kdm * qm.y + kdp * qp.y);
  qa.z = 0.5f * (kdm * qm.z + kdp * qp.z);
  qa.w = 0.5f * (kdm * qm.w + kdp * qp.w);

  qb.x = 0.5f * (ksm * qm.x + ksp * qp.x);
  qb.y = 0.5f * (ksm * qm.y + ksp * qp.y);
  qb.z = 0.5f * (ksm * qm.z + ksp * qp.z);
  qb.w = 0.5f * (ksm * qm.w + ksp * qp.w);

  QUAT_Exp(qa);
  QUAT_Exp(qb);
  QUAT_Mul(key->dsq,QB,qa);
  QUAT_Mul(key->ddq,QB,qb);
}

float SPLINE_SplineEase (float t, float a, float b)
{
float k;
float s = a+b;

  if (s == 0.0f) return t;
  if (s > 1.0f) {
    a = a/s;
    b = b/s;
  }
  k = 1.0f/(2.0f-a-b);
  if (t < a) return ((k/a)*t*t);
  else {
    if (t < 1.0f-b) return (k*(2*t-a));
    else {
      t = 1.0f-t;
      return (1.0f-(k/b)*t*t);
    }
  }
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void SPLINE_Init(TSpline *a)
{
  if (a->numKeys <= 0) {
		ERR("Can not initialize an empty or negative spline");
		return;
	}
  if (a->numKeys == 1) {
		ERR("Can not initialize spline with only 1 key");
		return;
	}
  if (a->numKeys == 2) {
    SPLINE_CompDerivTwo(a->keys, a->keys->next);
    return;
  }

  TKey *aux1 = a->keys;
  TKey *aux2 = aux1->next;
  TKey *aux3 = aux2->next;
  while (aux3) {
    SPLINE_CompDeriv(aux1, aux2, aux3);
    aux1 = aux2;
    aux2 = aux1->next;
    aux3 = aux2->next;
  }

  SPLINE_CompDerivFirst(a->keys, a->keys->next, a->keys->next->next);

  // To get the last three keys.
  WORD i = 0, last3;
  aux1 = a->keys;
  last3 = a->numKeys - 3;
  while (i < last3) { aux1 = aux1->next; i++; }
  aux2 = aux1->next; aux3 = aux2->next;
  SPLINE_CompDerivLast(aux1, aux2, aux3);
}

void SPLINE_InitRotation(TSpline *a)
{
  if (a->numKeys <= 0) {
		ERR("Can not initialize an empty or negative rotation spline");
		return;
	}
  if (a->numKeys == 1) {
		return;
	}

	TKey *aux1 = a->keys;
	TKey *aux2 = aux1->next;
	TKey *aux3 = aux2->next;
  while (aux3) {
    SPLINE_CompDerivAB(aux1, aux2, aux3);
    aux1 = aux2;
    aux2 = aux1->next;
    aux3 = aux2->next;
  }

  SPLINE_CompDerivAB (NULL, a->keys, a->keys->next);
  SPLINE_CompDerivAB (a->keys, a->last, NULL);
}

void SPLINE_Free(TSpline *a)
{
  if (a == NULL) return;
  TKey *aux;

  aux = a->keys;
  while (aux) {
    a->keys = a->keys->next;
    aux->next = NULL;
    FreeMem(aux);
    aux = a->keys;
  }
}

void SPLINE_Copy(TSpline *a, TSpline *b)
{
  if (b == NULL) { a = NULL; return; }
  TKey *aux;
  SPLINE_Free(a);
	a->start = b->start;
	a->end = b->end;
	a->frames = b->frames;
	a->sceneframes = b->sceneframes;
  aux = b->keys;
  while (aux) { SPLINE_AddKey(a,aux); aux = aux->next; }
}

void SPLINE_CopyKey(TKey *a, TKey *b)
{
  if (b == NULL) { a = NULL; return; }
  *a = *b;
  a->next = NULL;
}

void SPLINE_AddKey(TSpline *a, TKey *k)
{
  TKey *nou;

  nou = (TKey *) GetMem(sizeof(TKey));
  SPLINE_CopyKey(nou,k);
  if (a->keys == NULL) a->keys = nou;
  else a->last->next = nou;
  a->last = nou;

  a->numKeys++;
}

bool SPLINE_GetRealFrameValues(float &value, TSpline *a, float frame, DWORD flags)
{
	if (a==NULL) return false;

  if (a->numKeys <= 0) {
		ERR("Could not return real frame value (empty spline)");
		return false;
	}
  if (a->numKeys == 1) {
		value = a->keys->pos.x;
		return true;
	}

  WORD iframe = (WORD)frame;
  frame -= (float) iframe;
  if (flags & SPLINE_LOOP) iframe %= a->frames;
  if (flags & SPLINE_GLOBAL_LOOP) iframe %= a->sceneframes;
	if (iframe < a->start)	return false;
  if (iframe >= a->end) {
		value = a->last->pos.x;
		return true;
	}
	frame += (float) iframe;

  TKey *prevkey = a->keys;
  TKey *key = prevkey->next;
  while ((key->frame <= iframe) && (key->next)) { prevkey = key; key = key->next; }

  float tt1,tt2,tt3;
  float h[4];
  tt1 = (float)(frame - prevkey->frame) / (float)(key->frame - prevkey->frame);
  tt1 = SPLINE_SplineEase(tt1, prevkey->easeFrom, key->easeTo);
  tt2 = tt1 * tt1;
  tt3 = tt2 * tt1;
  h[0] = 2 * tt3 - 3 * tt2 + 1;
  h[1] = -2 * tt3 + 3 * tt2;
  h[2] = tt3 - 2 * tt2 + tt1;
  h[3] = tt3 - tt2;

  value = (h[0]*prevkey->pos.x) + (h[1]*key->pos.x) +
          (h[2]*prevkey->dd.x)  + (h[3]*key->ds.x);

  return true;
}

bool SPLINE_GetVertexFrameValues(TVertex &r, TSpline *a, float frame, DWORD flags)
{
	if (a==NULL) return false;

	if (a->numKeys <= 0) {
		ERR("Could not return vertex frame value (empty spline)");
		return false;
	}
  if (a->numKeys == 1) {
		r = a->keys->pos;
		return true;
	}

  WORD iframe = (WORD)frame;
  frame -= (float) iframe;
  if (flags & SPLINE_LOOP) iframe %= a->frames;
  if (flags & SPLINE_GLOBAL_LOOP) iframe %= a->sceneframes;
	if (iframe < a->start) return false;
  if (iframe >= a->end) {
		r = a->last->pos;
		return true;
	}
  frame += (float) iframe;

  TKey *prevkey = a->keys;
  TKey *key = prevkey->next;
  while ((key->frame <= iframe) && (key->next)) { prevkey = key; key = key->next; }

  float tt1,tt2,tt3;
  float h[4];
  tt1 = (float)(frame - prevkey->frame) / (float)(key->frame - prevkey->frame);
  tt1 = SPLINE_SplineEase(tt1,prevkey->easeFrom, key->easeTo);
  tt2 = tt1*tt1;
  tt3 = tt2*tt1;
  h[0] = 2*tt3-3*tt2+1;
  h[1] =-2*tt3+3*tt2;
  h[2] = tt3-2*tt2+tt1;
  h[3] = tt3-tt2;

  r.x = (h[0]*prevkey->pos.x) + (h[1]*key->pos.x) +
        (h[2]*prevkey->dd.x)  + (h[3]*key->ds.x);
  r.y = (h[0]*prevkey->pos.y) + (h[1]*key->pos.y) +
        (h[2]*prevkey->dd.y)  + (h[3]*key->ds.y);
  r.z = (h[0]*prevkey->pos.z) + (h[1]*key->pos.z) +
        (h[2]*prevkey->dd.z)  + (h[3]*key->ds.z);

  return true;
}

bool SPLINE_GetQuaternionFrameValues(TMatrix &m,TSpline *a, float frame, DWORD flags)
{
	if (a==NULL) return false;

	if (a->numKeys <= 0) {
		ERR("Could not return quaternion frame value (empty spline)");
		return false;
	}

  if (a->numKeys == 1) {
		QUAT_RotationMatrix(m,a->keys->qa);
		return true;
	}

  WORD iframe = (WORD)frame;
  frame -= (float) iframe;
  if (flags & SPLINE_LOOP) iframe %= a->frames;
  if (flags & SPLINE_GLOBAL_LOOP) iframe %= a->sceneframes;
	if (iframe < a->start) return false;
  if (iframe >= a->end) {
		QUAT_RotationMatrix(m,a->last->qa);
		return true;
	}
  frame += (float) iframe;

  TKey *prevkey = a->keys;
  TKey *key = prevkey->next;
  while ((key->frame <= iframe) && (key->next)) { prevkey = key; key = key->next; }

	float t;
  t = (float) (frame - prevkey->frame) / (float) (key->frame - prevkey->frame);
  t = SPLINE_SplineEase(t, prevkey->easeFrom, key->easeTo);

	TQuat qa, qb;
  qa = prevkey->qa;
  QUAT_Slerp(qa, key->qa, t);
  qb = prevkey->ddq;
  QUAT_Slerp(qb, key->dsq, t);

  t = (((1.0f-t)*2.0f)*t);
  QUAT_Slerp(qa, qb, t);

  QUAT_RotationMatrix(m,qa);

	return true;
}

