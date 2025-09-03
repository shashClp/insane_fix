// AW3D - Anaconda 3D Windows Library
// Order

#include <windows.h>
#include <log.h>
#include <mem.h>
#include <order.h>
#include <defines.h>

TAW3D_BufferZOrder::TAW3D_BufferZOrder(DWORD entries,DWORD memory)
{
  SEC_START ("Initializing AW3D_BufferZOrder");

  LOG ("Memory: %i Entries: %i",memory,entries);

#ifdef _DEBUG
  this->memory = memory;
  this->entries =  entries;
#endif
  
  buffer = (DWORD *) GetMem (memory);
  tree_items = (TTreeZItem *) GetMem (entries*sizeof(TTreeZItem));

  buffer_init = buffer;

  Prepare();

  SEC_END ();
}

TAW3D_BufferZOrder::~TAW3D_BufferZOrder()
{
  LOG ("Freeing AW3D_BufferZOrder");
  if (buffer)
  {
    FreeMem (buffer);
    buffer=NULL;
  }  
  if (tree_items)
  {
    FreeMem (tree_items);
    tree_items=NULL;
  }  
}

void TAW3D_BufferZOrder::Prepare()
{
  actual = buffer = buffer_init;

#ifdef _DEBUG
  tree.count = 0;
#endif

  tree.root = NULL;
  tree.first = NULL;
  tree.last = NULL;
  tree_items_actual = tree_items;
}

void TAW3D_BufferZOrder::Execute()
{
AW3D_BufferZOrder_Callback fun;

  TTreeZItem *i = tree.first;
  while (i!=NULL)
  {
    fun = (AW3D_BufferZOrder_Callback)(*i->node);
    fun (i->node+1);
    i = i->next;
  }
}

void TAW3D_BufferZOrder::ExecuteReverse()
{
AW3D_BufferZOrder_Callback fun;

  TTreeZItem *i = tree.last;
  while (i!=NULL)
  {
    fun = (AW3D_BufferZOrder_Callback)(*i->node);
    fun (i->node+1);
    i = i->prior;
  }
}
