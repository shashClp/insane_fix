// AW3D - Anaconda 3D Windows Library
// Texture

#include <windows.h>
#include <mem.h>
#include <log.h>
#include <image.h>
#include <traverse.h>
#include <texture.h>

DWORD TXT_maxblocs;
TTxtBloc *TXT_blocs;
TTxtInBloc *TXT_inblocs;

////////////////////////////////////////////////////////////////////////////

void TXT_ReCalculateSlots (TList &l)
{
TTxtFreeBloc t;
TTxtFreeBloc *t1;
TTxtFreeBloc *t2;
TTraverse ta1;
TTraverse ta2;
DWORD maxa,a;
TListItem *la1=NULL;
TListItem *la2=NULL;
DWORD i,j,w1,h1;

  do {
    maxa=0;
    if (l.count>1)
    {
      LIST_GetTraverse (ta1,l);
      for (i=0; i<l.count; i++)
      {
        t1 = (TTxtFreeBloc *) LIST_Get (ta1);
        w1 = t1->x+t1->w;
        h1 = t1->y+t1->h;
        LIST_GetTraverse (ta2,l);
        for (j=0; j<l.count; j++)
        {
          t2 = (TTxtFreeBloc *) LIST_Get (ta2);
          if (t2!=t1)
          {
            if (((w1 == t2->x) && (t1->y==t2->y) && (t1->h==t2->h)) ||
                ((h1 == t2->y) && (t1->x==t2->x) && (t1->w==t2->w)))
            {
              a = (t1->w*t1->h)+(t2->w*t2->h);
              if (a>maxa)
              {
                maxa = a;
                la1 = (TListItem *) LIST_GetItem (ta1);
                la2 = (TListItem *) LIST_GetItem (ta2);
              }
            }
          }
          LIST_Next (ta2);
        }
        LIST_Next (ta1);
      }
      if (maxa!=0)
      {
        t1 = (TTxtFreeBloc *) LIST_GetFromItem (la1);
        t2 = (TTxtFreeBloc *) LIST_GetFromItem (la2);
        t.base = t1->base;
        t.x = t1->x;
        t.y = t1->y;
        if (t1->y==t2->y)
        {
          t.w = t1->w+t2->w;
          t.h = t1->h;
        } else {
          t.w = t1->w;
          t.h = t1->h+t2->h;
        }
        LIST_Delete (l,la1);
        LIST_Delete (l,la2);
        LIST_Add (l,&t,sizeof(TTxtFreeBloc));
      }
    }
  } while (maxa!=0);
}

void TXT_FillSet (TListItem *s,TTxtBloc *b,DWORD w,DWORD h,DWORD *src)
{
TTxtFreeBloc *t;
DWORD i,j;
DWORD *dst;

  if (s)
  {
    t = (TTxtFreeBloc *) LIST_GetFromItem (s);
    dst = b->bits+t->x+(t->y*256);
    for (j=0; j<h; j++)
      for (i=0; i<w; i++)
        dst[(j*256)+i] = src[(j*w)+i];
  } else {
    dst = b->bits;
    for (j=0; j<h; j++)
      for (i=0; i<w; i++)
        dst[(j*256)+i] = src[(j*w)+i];
  }
}

TListItem *TXT_FindCoincidentSet (DWORD w,DWORD h)
{
TTraverse ta;
TTxtFreeBloc *t;
DWORD i,j;

  for (i=0; i<TXT_maxblocs; i++)
  {
    if (TXT_blocs[i].bits)
    {
      LIST_GetTraverse (ta,TXT_blocs[i].sets);
      for (j=0; j<TXT_blocs[i].sets.count; j++)
      {
        t = (TTxtFreeBloc *) LIST_Get (ta);
        if ((w==t->w) && (h==t->h)) return LIST_GetItem (ta);
        LIST_Next (ta);
      }
    }
  }
  return NULL;
}

TListItem *TXT_FindFirstEmptySet (DWORD w,DWORD h)
{
TTraverse ta;
TTxtFreeBloc *t;
DWORD i,j;

  for (i=0; i<TXT_maxblocs; i++)
  {
    if (TXT_blocs[i].bits)
    {
      LIST_GetTraverse (ta,TXT_blocs[i].sets);
      for (j=0; j<TXT_blocs[i].sets.count; j++)
      {
        t = (TTxtFreeBloc *) LIST_Get (ta);
        if ((w<=t->w) && (h<=t->h)) return LIST_GetItem (ta);
        LIST_Next (ta);
      }
    }
  }
  return NULL;
}

TTxtBloc *TXT_FindFirstEmptyBloc()
{
DWORD i=0;

  while (i<TXT_maxblocs)
  {
    if (TXT_blocs[i].bits==NULL) return TXT_blocs+i;
    i++;
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////

void TXT_Init(DWORD max)
{
DWORD i;

  SEC_START("Initializing Texture Sloter");
  LOG ("Texture slots: %i",max);
  TXT_maxblocs = max;
  TXT_blocs = (TTxtBloc *) GetMem (max*sizeof(TTxtBloc));
  TXT_inblocs = (TTxtInBloc *) GetMem (TXT_MAXTEXTURE*sizeof(TTxtInBloc));
  for (i=0; i<TXT_MAXTEXTURE; i++)
  {
    TXT_inblocs[i].txt = NULL;
    TXT_inblocs[i].cops = 0;
  }
  for (i=0; i<TXT_maxblocs; i++)
  {
    TXT_blocs[i].id = i;
    TXT_blocs[i].bits = NULL;
    LIST_Init (TXT_blocs[i].sets);
  }
  SEC_END();
}

void TXT_Close ()
{
DWORD i;

  SEC_START ("Freeing Texture Sloter");
  for (i=0; i<TXT_maxblocs; i++)
  {
    if (TXT_blocs[i].bits)
    {
      LOG ("Disposing bloc %i",i);
      FreeMem (TXT_blocs[i].bits);
      TXT_blocs[i].bits = NULL;
      if (TXT_blocs[i].sets.count>0)
      {
#ifdef LOG_LOGS
        TTraverse ta;
        TTxtFreeBloc *t;
        LIST_GetTraverse (ta,TXT_blocs[i].sets);
        for (DWORD j=0; j<TXT_blocs[i].sets.count; j++)
        {
          t = (TTxtFreeBloc *) LIST_Get (ta);
          LOG ("Set %i (X: %3i,Y: %3i | W: %3i H: %3i) was empty",j,t->x,t->y,t->w,t->h);
          LIST_Next (ta);
        }
#endif
        LIST_Free (TXT_blocs[i].sets);
        LIST_Init (TXT_blocs[i].sets);
      } else LOG ("FULL!");
    }
  }
  FreeMem (TXT_blocs);
  TXT_blocs = NULL;

  for (i=0; i<TXT_MAXTEXTURE; i++)
  {
    if (TXT_inblocs[i].txt!=NULL)
    {
      FreeMem (TXT_inblocs[i].id);
      TXT_inblocs[i].id = NULL;
      break;
    }
  }
  FreeMem (TXT_inblocs);
  TXT_inblocs = NULL;
  SEC_END();
}

TTexture *TXT_AddTexture (DWORD w,DWORD h,DWORD *src)
{
TListItem *s;
TTxtBloc *b;
TTxtFreeBloc *t;
TTxtFreeBloc sr;
TTxtFreeBloc sb;
TTxtFreeBloc srb;
TTexture *txt;
long x,y;

  s = TXT_FindCoincidentSet (w,h);
  if (s)
  {
    t = (TTxtFreeBloc *) LIST_GetFromItem (s);
    b = t->base;

    x = t->x;
    y = t->y;
    TXT_FillSet (s,b,w,h,src);
    LIST_Delete (b->sets,s);
    if (b->sets.count==0) {
      LIST_Init(b->sets);
    }
  } else {
    s = TXT_FindFirstEmptySet (w,h);
    if (s)
    {
      t = (TTxtFreeBloc *) LIST_GetFromItem (s);
      b = t->base;

      x = t->x;
      y = t->y;
      TXT_FillSet (s,b,w,h,src);

      sr.base = b;
      sr.x = t->x+w;
      sr.y = t->y;
      sr.w = t->w-w;
      sr.h = h;
      if ((sr.w>0) && (sr.h>0))
        LIST_Add (b->sets,&sr,sizeof(TTxtFreeBloc));

      sb.base = b;
      sb.x = t->x;
      sb.y = t->y+h;
      sb.w = w;
      sb.h = t->h-h;
      if ((sb.w>0) && (sb.h>0))
        LIST_Add (b->sets,&sb,sizeof(TTxtFreeBloc));

      srb.base = b;
      srb.x = t->x+w;
      srb.y = t->y+h;
      srb.w = t->w-w;
      srb.h = t->h-h;
      if ((srb.w>0) && (srb.h>0))
        LIST_Add (b->sets,&srb,sizeof(TTxtFreeBloc));

      LIST_Delete (b->sets,s);
      TXT_ReCalculateSlots (b->sets);
    } else {
      b = TXT_FindFirstEmptyBloc();
      if (!b) {
				ERR ("No more texture slots free");
				return NULL;
			}
      b->bits = (DWORD *) GetMem (256*256*4);

      TXT_FillSet (NULL,b,w,h,src);
      sr.base = b;
      sr.x = w;
      sr.y = 0;
      sr.w = 256-w;
      sr.h = h;
      if ((sr.w>0) && (sr.h>0))
        LIST_Add (b->sets,&sr,sizeof(TTxtFreeBloc));
      sb.base = b;
      sb.x = 0;
      sb.y = h;
      sb.w = w;
      sb.h = 256-h;
      if ((sb.w>0) && (sb.h>0))
        LIST_Add (b->sets,&sb,sizeof(TTxtFreeBloc));
      srb.base = b;
      srb.x = w;
      srb.y = h;
      srb.w = 256-w;
      srb.h = 256-h;
      if ((srb.w>0) && (srb.h>0))
        LIST_Add (b->sets,&srb,sizeof(TTxtFreeBloc));

      if (b->sets.count!=0)
      {
        TXT_ReCalculateSlots (b->sets);
        if (b->sets.count==0) {
          LIST_Init(b->sets);
        }
      } else {
        LIST_Init(b->sets);
      }
      x = 0;
      y = 0;
    }
  }

  txt = (TTexture *) GetMem (sizeof(TTexture));
  txt->flags = 0;
  txt->color = 0;
  txt->bits = b->bits+x+y*256;
  txt->w = w;
  txt->h = h;

  return txt;
}

TTexture *TXT_AddTexture (char *filename)
{
DWORD i;
TImage *img;
TTexture *txt;
TTxtInBloc *in=NULL;

  for (i=0; i<TXT_MAXTEXTURE; i++)
  {
    if (TXT_inblocs[i].txt !=NULL)
    {
      if (!stricmp(TXT_inblocs[i].id,filename))
      {
        TXT_inblocs[i].cops++;
        return TXT_inblocs[i].txt;
      }
    }
  }

  img = IMAGE_Load (filename,32);
	if (!img) {
		ERR("Adding a texture: %s",filename);
		return NULL;
	}
  TImage *tmp;
  switch (img->bpp) 
  {
    case 8:
      tmp = (TImage *)GetMem (sizeof(TImage));
      IMAGE_8To32 (tmp,img);
      FreeMem (img);
      img = tmp;
      break;
    case 24:
      tmp = (TImage *)GetMem (sizeof(TImage));
      IMAGE_24To32 (tmp,img);
      FreeMem (img);
      img = tmp;
      break;
	}

  txt = TXT_AddTexture (img->width,img->height,(DWORD *)img->bits);
  FreeMem (img);

  for (i=0; i<TXT_MAXTEXTURE; i++)
  {
    if (TXT_inblocs[i].txt==NULL)
    {
      in = TXT_inblocs+i;
      in->txt = txt;
      in->cops = 1;
      in->id = (char *) GetMem (strlen(filename)+1);
      strcpy (in->id,filename);
      break;
    }
  }
  if (i==TXT_MAXTEXTURE) {
		ERR("No more Texture slots!");
		return NULL;
	}
  return in->txt;
}

void TXT_DelTexture (TTexture *txt)
{
DWORD i;

  if (txt==NULL) {
		ERR ("Deleting a NULL texture");
		return;
	}
  for (i=0; i<TXT_MAXTEXTURE; i++)
  {
    if (TXT_inblocs[i].txt==txt)
    {
      if (TXT_inblocs[i].cops==1)
      {
        TXT_DelTexture (txt->w,txt->h,txt->bits);
        FreeMem (TXT_inblocs[i].id);
        TXT_inblocs[i].id = NULL;
        TXT_inblocs[i].txt = NULL;
        FreeMem (txt);
      } else {
        TXT_inblocs[i].cops--;
      }
      break;
    }
  }
}

void TXT_DelTexture (DWORD w,DWORD h,DWORD *src)
{
DWORD i;
long x,y,t;
TTxtFreeBloc *tt;
TTxtFreeBloc s;

  if (src==NULL) {
		ERR("Deleting a texture with NULL bits pointer");
		return;
	}
  if ((w==0) || (h==0)) {
		ERR("Deleting a texture with w==0 || h==0");
		return;
	}
  if ((w>256) || (h>256)) {
		ERR("Deleting a texture with w>256 || h>256");
		return;
	}

  i = 0;
  while (i<TXT_maxblocs)
  {
    if (TXT_blocs[i].bits)
    {
      t = (((long)src)-((long)TXT_blocs[i].bits))/4;
      if ((t < 256*256) && (t>=0))
      {
        x = t%256;
        y = t/256;
        s.base = TXT_blocs+i;
        s.x = x;
        s.y = y;
        s.w = w;
        s.h = h;

        LIST_Add (TXT_blocs[i].sets,&s,sizeof(TTxtFreeBloc));

        if (TXT_blocs[i].sets.count>1) {
          TXT_ReCalculateSlots (TXT_blocs[i].sets);
        }

        if (TXT_blocs[i].sets.count==1)
        {
          tt = (TTxtFreeBloc *) LIST_GetFromItem (TXT_blocs[i].sets.first);
          if ((tt->w==256) && (tt->h==256))
          {
            FreeMem (TXT_blocs[i].bits);
            TXT_blocs[i].bits = NULL;
            LIST_Free (TXT_blocs[i].sets);
            LIST_Init (TXT_blocs[i].sets);
          }
        }
        return;
      }
    }
    i++;
  }
  if (i==TXT_maxblocs) {
    ERR ("Texture not found in texture blocs");
		return;
	}
}

