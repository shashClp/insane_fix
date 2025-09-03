// AW3D - Anaconda 3D Windows Library
// Order

#ifndef AW3D_ORDER
#define AW3D_ORDER

#include <defines.h>
#include <log.h>
#include <misc.h>

typedef void (_fastcall *AW3D_BufferZOrder_Callback)(void *src);

typedef struct TOwnTreeZItem {
  TOwnTreeZItem *left;
  TOwnTreeZItem *right;
  TOwnTreeZItem *next;
  TOwnTreeZItem *prior;
  DWORD *node;
  float z;
} TTreeZItem;

typedef struct {
  TTreeZItem *root;
  TTreeZItem *first;
  TTreeZItem *last;

#ifdef _DEBUG
  DWORD count;
#endif

} TTreeZ;

class TAW3D_BufferZOrder
{
private:
  DWORD *buffer_init;
  DWORD *buffer;
  DWORD *actual; 
  TTreeZItem *tree_items;
  TTreeZItem *tree_items_actual;
public:

#ifdef _DEBUG
  DWORD entries;
  DWORD memory;
#endif

  TTreeZ tree;

  TAW3D_BufferZOrder(DWORD entries,DWORD memory);
  ~TAW3D_BufferZOrder();  

  void Prepare();
  void Execute();
  void ExecuteReverse();

// Inline func's

  inline void Begin (void *function)
  {
    actual = buffer;  
    *actual = (DWORD)function;
    actual++;

  #ifdef _DEBUG
    if ((DWORD)(actual-buffer_init)>memory)
      ERR ("Order Buffer exausted in Begin!");
  #endif
  }

  inline void Push (void *src,DWORD size)
  {    
    MISC_MemCpy4 (actual,src,size);
    actual+=size;

  #ifdef _DEBUG
    if ((DWORD)(actual-buffer_init)>memory)
      ERR ("Order Buffer exausted!");
  #endif
  }

  inline void Push (DWORD value)
  {
    *actual = value;
    actual++;

  #ifdef _DEBUG
    if ((DWORD)(actual-buffer_init)>memory)
      ERR ("Order Buffer exausted in Push!");
  #endif
  }

  inline void Push (float value)
  {
    *((float *)actual) = value;
    actual++;

  #ifdef _DEBUG
    if ((DWORD)(actual-buffer_init)>memory)
      ERR ("Order Buffer exausted!");
  #endif
  }

  inline void Push (void *value)  
  {
    *actual = (DWORD) value;
    actual++;

  #ifdef _DEBUG
    if ((DWORD)(actual-buffer_init)>memory)
      ERR ("Order Buffer exausted!");
  #endif
  }

  inline void End (float z1,float z2)
  {
    End ((z1+z2)*0.5f);
  }

  inline void End (float z1,float z2,float z3)
  {
    End ((z1+z2+z3)*M_1Div3);
  }

  inline void End (float z)
  {  
    TTreeZItem *i = tree_items_actual++;
    i->left = NULL;
    i->right = NULL;
    i->node = buffer;
    i->z = z;
    if (tree.root==NULL)
    {
      i->next = NULL;
      i->prior = NULL;
      tree.root = i;
      tree.first = i;    
      tree.last = i;    
    } else {
      TTreeZItem *j = tree.root;
      for(;;)
      {
        if (z > j->z)
        {
          if (!j->right)
          {
            j->right = i;
            i->prior = j;
            i->next = j->next;       
            j->next = i;
            if (i->next!=NULL) i->next->prior = i; else tree.last = i;
            break;
          } else j = j->right;
        } else {
          if (j->left==NULL)
          {
            j->left = i;          
            i->next = j;
            i->prior = j->prior;
            j->prior = i;
            if (i->prior!=NULL) i->prior->next = i; else tree.first = i;
            break;
          } else j = j->left;
        }
      }
    }

    buffer = actual;

  #ifdef _DEBUG
    tree.count++;  
    if (tree.count>=entries)
    {
      ERR ("Order elements >= entries allowed!");
      return;
    }
  #endif
  }
};

#endif