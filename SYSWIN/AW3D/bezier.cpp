// AW3D - Anaconda 3D Library
// Bezier

#include <windows.h>
#include <a3d.h>
#include <bezier.h>
#include <log.h>
#include <mem.h>

TVertex *BEZIER_paux=NULL;
TBezierCurve BEZIER_caux;

////////////////////////////////////////////////////////////////
// Global functions
////////////////////////////////////////////////////////////////

void BEZIER_Init()
{
	BEZIER_paux = (TVertex *) GetMem(sizeof(TVertex)*BEZIER_MAX_CP);
	BEZIER_CurveInit(BEZIER_caux,BEZIER_MAX_CP,0);
}

void BEZIER_Close()
{
	FreeMem(BEZIER_paux);
	BEZIER_CurveClose(BEZIER_caux);
}


////////////////////////////////////////////////////////////////
// Curve functions
////////////////////////////////////////////////////////////////

void BEZIER_CurveInit(TBezierCurve &c, int n, DWORD flags)
{
	c.cp = (TVertex *) GetMem(sizeof(TVertex)*n);
	c.act = 0;
	c.flags = flags;
	if (flags & BEZIER_LOOP) c.n = n + 1;
	else c.n = n;
}

void BEZIER_CurveClose(TBezierCurve &c)
{
	FreeMem(c.cp);
	c.n = c.act = 0;
	c.flags = 0;
}

void BEZIER_CurveAddPoint(TBezierCurve &c, float x, float y, float z)
{
	if (c.act>=c.n) {
		LOG("BEZIER_CurveAddPoint: Can not add another curve point");
		return;
	}
	VTX_Set(c.cp[c.act],x,y,z);
	c.act++;
	if (((c.act+1)==c.n) && (c.flags & BEZIER_LOOP)) {
		c.cp[c.act] = c.cp[0];
		c.act++;
	}
}

void BEZIER_CurveAddPoint(TBezierCurve &c, TVertex &p)
{
	if (c.act>=c.n) {
		LOG("BEZIER_CurveAddPoint: Can not add another curve point");
		return;
	}
	c.cp[c.act] = p;
	c.act++;
	if (((c.act+1)==c.n) && (c.flags & BEZIER_LOOP)) {
		c.cp[c.act] = c.cp[0];
		c.act++;
	}
}

void BEZIER_CurvePoint(TVertex &out, TBezierCurve &c, float u)
{
	int n;
	float t = 1.0f-u;
	memcpy(BEZIER_paux,c.cp,sizeof(TVertex)*c.n);
	for(int k=1; k<c.n; k++) {
		n=c.n-k;
		for(int i=0; i<n; i++) {
			BEZIER_paux[i].x = t*BEZIER_paux[i].x+u*BEZIER_paux[i+1].x;
			BEZIER_paux[i].y = t*BEZIER_paux[i].y+u*BEZIER_paux[i+1].y;
			BEZIER_paux[i].z = t*BEZIER_paux[i].z+u*BEZIER_paux[i+1].z;
		}
	}
	out=BEZIER_paux[0];
}

void BEZIER_Curve(TVertex *out, TBezierCurve &c, int detail)
{
	if (out==NULL) {
		ERR("BEZIER_Curve: You should get some memory first.");
		return;
	}
	float t=1.0f/((float) (detail-1));
	for (int i=0; i<detail; i++) {
		BEZIER_CurvePoint(out[i],c,(float)i*t);
	}
}


////////////////////////////////////////////////////////////////
// Surface functions
////////////////////////////////////////////////////////////////

void BEZIER_SurfaceInit(TBezierSurface &s, WORD m)
{
	s.obj = NULL;
	s.cu = NULL;
	s.cv = (TBezierCurve *) GetMem(sizeof(TBezierCurve)*m);
	s.m = m;
	s.act = 0;
	s.n = 0;
}

void BEZIER_SurfaceClose(TBezierSurface &s)
{
	Free3DObject(s.obj);
	FreeMem(s.obj);
	FreeMem(s.cu);
	FreeMem(s.cv);
	s.obj = NULL;
	s.cu = s.cv = NULL;
	s.n = s.m = s.act = 0;
}

void BEZIER_SurfaceAddCurve(TBezierSurface &s, TBezierCurve &c)
{
	if (s.act>s.m) {
		LOG("BEZIER_SurfaceAddCurve: Can not add another curve");
		return;
	}
	if ((s.act>0) && (c.n>s.n)) {
		ERR("BEZIER_SurfaceAddCurve: Number of curve points do not match with previous curves");
		return;
	}
	s.cv[s.act] = c;
	s.cv[s.act].flags = c.flags;
	if (s.act == 0) s.n = s.cv[0].n;
	s.act++;
	if (s.act==s.m) {
		for (int i=0; i<s.n; i++) {
			s.cu = (TBezierCurve *) realloc(s.cu,sizeof(TBezierCurve)*(i+1));
			BEZIER_CurveInit(s.cu[i],s.m,0);
			for (int j=0; j<s.m; j++) {
				BEZIER_CurveAddPoint(s.cu[i],s.cv[j].cp[i]);
			}
		}
	}
}

void BEZIER_SurfaceGetPoint(TVertex &out, TBezierSurface &s, WORD m, WORD n)
{	out = s.cv[m].cp[n]; }

void BEZIER_SurfaceSetPoint(TBezierSurface &s, WORD m, WORD n, TVertex &in)
{
	s.cv[m].cp[n] = in;
	s.cu[n].cp[m] = in;
}

void BEZIER_SurfacePoint(TVertex &out, TBezierSurface &s, float u, float v)
{
	TVertex q;
	if (s.n<=s.m) {
		BEZIER_caux.n = s.m;
		BEZIER_caux.act = 0;
		for(int i=0; i<s.m; i++) {
			BEZIER_CurvePoint(q,s.cv[i],u);
			BEZIER_CurveAddPoint(BEZIER_caux,q);
		}
		BEZIER_CurvePoint(out,BEZIER_caux,v);
	} else {
		BEZIER_caux.n = s.n;
		BEZIER_caux.act = 0;
		for(int i=0; i<s.n; i++) {
			BEZIER_CurvePoint(q,s.cu[i],v);
			BEZIER_CurveAddPoint(BEZIER_caux,q);
		}
		BEZIER_CurvePoint(out,BEZIER_caux,u);
	}
}

void BEZIER_Surface(TVertex *out, TBezierSurface &s, int s_detail, int c_detail)
{
	if (out==NULL) {
		ERR("BEZIER_Surface: You should get some memory first");
		return;
	}
	int k=0;
	float u,v;
	float t1=1.0f/((float) (c_detail-1));
	float t2=1.0f/((float) (s_detail-1));
	for (int i=0; i<c_detail; i++) {
		u = (float)i*t1;
		for (int j=0; j<s_detail; j++) {
			v = (float)j*t2;
			BEZIER_SurfacePoint(out[k++],s,u,v);
		}
	}
}

////////////////////////////////////////////////////////////////
// Surface to 3D Object functions
////////////////////////////////////////////////////////////////

bool BEZIER_SurfaceInit3DObject(TBezierSurface &s, int s_detail, int c_detail, char *txtname)
{
	s.obj = (TObject *) GetMem(sizeof(TObject));
	memset(s.obj,0,sizeof(TObject));

	s.obj->numface = (s_detail-1)*(c_detail-1)*2;
	s.obj->face = (TFace *) GetMem(sizeof(TFace)*s.obj->numface);
	s.obj->numvtx = s_detail*c_detail;
	s.obj->vtx = (TVertex *) GetMem(sizeof(TVertex)*s.obj->numvtx);
	s.obj->plane = (TPlane *) GetMem(sizeof(TPlane)*s.obj->numface);
	s.obj->nrm = (TVertex *) GetMem(sizeof(TVertex)*s.obj->numvtx);
	s.obj->inf = (TInfo *) GetMem(sizeof(TInfo)*s.obj->numvtx);
	s.obj->bound.dist = 1000000.0f;
	VTX_Set(s.obj->bound.org,0.0f,0.0f,0.0f);

	s.txt = TXT_AddTexture(txtname);
	if (!s.txt) {
		ERR("Converting a bezier surface to a 3D object");
		return false;
	}

	int idx;
	for(int i=0; i<c_detail; i++) {
		for(int j=0; j<s_detail; j++) {
			idx = i*s_detail+j;
			s.obj->inf[idx].uu = ((float) j/(float) s_detail)*(float) (s.txt->w*2);
			s.obj->inf[idx].vv = ((float) i/(float) c_detail)*(float) (s.txt->h*2);
		}
	}

	idx = 0;
	int val1, val2;
	for(i=0; i<c_detail-1; i++) {
		for (int j=0; j<s_detail-1; j++) {
			val1 = i*s_detail+j;
			val2 = (i+1)*s_detail+j;
			s.obj->face[idx].a = val1;
			s.obj->face[idx].c = val2+1;
			s.obj->face[idx].b = val2;
			s.obj->face[idx].i = s.txt;
			idx++;
			s.obj->face[idx].a = val1;
			s.obj->face[idx].c = val1+1;
			s.obj->face[idx].b = val2+1;
			s.obj->face[idx].i = s.txt;
			idx++;
		}
	}

	return true;
}

TObject* BEZIER_Surface2Object(TBezierSurface &s, int s_detail, int c_detail)
{
	BEZIER_Surface(s.obj->vtx,s,s_detail,c_detail);

	TFace *f;
	for (WORD i=0; i<s.obj->numface; i++) {
		f = &s.obj->face[i];
		PLA_CalcABCD(s.obj->plane[i],s.obj->vtx[f->a],s.obj->vtx[f->b],s.obj->vtx[f->c]);
	}

	TVertex nrm;
	WORD fidx, nidx; // face and normal index

	int cn = c_detail-1;
	int sn = s_detail-1;
	int face4row = sn*2;

	// Normals de vertexs per les quatre cantonades
	fidx = 0;
	nrm.x = s.obj->plane[fidx].A; nrm.x += s.obj->plane[fidx+1].A;
	nrm.y = s.obj->plane[fidx].B; nrm.y += s.obj->plane[fidx+1].B;
	nrm.z = s.obj->plane[fidx].C; nrm.z += s.obj->plane[fidx+1].C;
	nrm.x /= 2.0f; nrm.y /= 2.0f; nrm.z /= 2.0f;
	nidx = 0;
	s.obj->nrm[nidx] = nrm;

	fidx = (sn*2)-1;
	nrm.x = s.obj->plane[fidx].A;
	nrm.y = s.obj->plane[fidx].B;
	nrm.z = s.obj->plane[fidx].C;
	nidx = sn;
	s.obj->nrm[nidx] = nrm;

	fidx = face4row*(c_detail-2);
	nrm.x = s.obj->plane[fidx].A;
	nrm.y = s.obj->plane[fidx].B;
	nrm.z = s.obj->plane[fidx].C;
	nidx = s_detail*c_detail-s_detail;
	s.obj->nrm[nidx] = nrm;

	fidx = (sn*cn)-2;
	nrm.x = s.obj->plane[fidx].A; nrm.x += s.obj->plane[fidx-1].A;
	nrm.y = s.obj->plane[fidx].B; nrm.y += s.obj->plane[fidx-1].B;
	nrm.z = s.obj->plane[fidx].C; nrm.z += s.obj->plane[fidx-1].C;
	nrm.x /= 2.0f; nrm.y /= 2.0f; nrm.z /= 2.0f;
	nidx = s_detail*cn;
	s.obj->nrm[nidx] = nrm;
	
	// Normals de vertexs per la resta de vertexs
	WORD k=0;
	for(i=0; i<c_detail; i++) {
		for (int j=0; j<s_detail; j++) {
			if (j==0 && i==0) k++; else
			if (j==sn && i==0) k++; else
			if (j==0 && i==sn) k++; else
			if (j==sn && i==sn) k++; else
			if (i==0) {
				fidx = j*2-1;
				nrm.x = s.obj->plane[fidx].A; nrm.x += s.obj->plane[fidx+1].A; nrm.x += s.obj->plane[fidx+2].A;
				nrm.y = s.obj->plane[fidx].B; nrm.y += s.obj->plane[fidx+1].B; nrm.y += s.obj->plane[fidx+2].B;
				nrm.z = s.obj->plane[fidx].C; nrm.z += s.obj->plane[fidx+1].C; nrm.z += s.obj->plane[fidx+2].C;
				nrm.x *= 0.13f; nrm.y *= 0.13f; nrm.z *= 0.13f;
				s.obj->nrm[k] = nrm;
				k++;
			} else
			if (i==sn) {
				fidx = k+j;
				nrm.x = s.obj->plane[fidx].A; nrm.x += s.obj->plane[fidx+1].A; nrm.x += s.obj->plane[fidx+2].A;
				nrm.y = s.obj->plane[fidx].B; nrm.y += s.obj->plane[fidx+1].B; nrm.y += s.obj->plane[fidx+2].B;
				nrm.z = s.obj->plane[fidx].C; nrm.z += s.obj->plane[fidx+1].C; nrm.z += s.obj->plane[fidx+2].C;
				nrm.x *= 0.13f; nrm.y *= 0.13f; nrm.z *= 0.13f;
				s.obj->nrm[k] = nrm;
				k++;
			} else
			if (j==0) {
				fidx = i*face4row;
				nrm.x = s.obj->plane[fidx].A; nrm.x += s.obj->plane[fidx+1].A; nrm.x += s.obj->plane[fidx-face4row].A;
				nrm.y = s.obj->plane[fidx].B; nrm.y += s.obj->plane[fidx+1].B; nrm.y += s.obj->plane[fidx-face4row].B;
				nrm.z = s.obj->plane[fidx].C; nrm.z += s.obj->plane[fidx+1].C; nrm.z += s.obj->plane[fidx-face4row].C;
				nrm.x *= 0.13f; nrm.y *= 0.13f; nrm.z *= 0.13f;
				s.obj->nrm[k] = nrm;
				k++;
			} else
			if (j==sn) {
				fidx = (i+1)*face4row-1;
				nrm.x = s.obj->plane[fidx].A; nrm.x += s.obj->plane[fidx-face4row-1].A; nrm.x += s.obj->plane[fidx-face4row].A;
				nrm.y = s.obj->plane[fidx].B; nrm.y += s.obj->plane[fidx-face4row-1].B; nrm.y += s.obj->plane[fidx-face4row].B;
				nrm.z = s.obj->plane[fidx].C; nrm.z += s.obj->plane[fidx-face4row-1].C; nrm.z += s.obj->plane[fidx-face4row].C;
				nrm.x *= 0.13f; nrm.y *= 0.13f; nrm.z *= 0.13f;
				s.obj->nrm[k] = nrm;
				k++;
			} else k++;
		}
	}

	k = s_detail+1;
	int p0idx = 0, p1idx = sn*2+1;
	for(i=1; i<cn; i++) {
		for (int j=1; j<sn; j++) {
			nrm.x = s.obj->plane[p0idx].A; nrm.x += s.obj->plane[p0idx+1].A; nrm.x += s.obj->plane[p0idx+2].A;
			nrm.x += s.obj->plane[p1idx].A;	nrm.x += s.obj->plane[p1idx+1].A; nrm.x += s.obj->plane[p1idx+2].A;

			nrm.y = s.obj->plane[p0idx].B; nrm.y += s.obj->plane[p0idx+1].B; nrm.y += s.obj->plane[p0idx+2].B;
			nrm.y += s.obj->plane[p1idx].B;	nrm.y += s.obj->plane[p1idx+1].B; nrm.y += s.obj->plane[p1idx+2].B;

			nrm.z = s.obj->plane[p0idx].C; nrm.z += s.obj->plane[p0idx+1].C; nrm.z += s.obj->plane[p0idx+2].C;
			nrm.z += s.obj->plane[p1idx].C;	nrm.z += s.obj->plane[p1idx+1].C; nrm.z += s.obj->plane[p1idx+2].C;

			nrm.x *= 0.16f; nrm.y *= 0.16f; nrm.z *= 0.16f;
			s.obj->nrm[k] = nrm;

			p0idx += 2;	p1idx += 2;

			k++;
		}
		k += 2;
		p0idx += 2;	p1idx += 2;
	}

	return s.obj;
}

