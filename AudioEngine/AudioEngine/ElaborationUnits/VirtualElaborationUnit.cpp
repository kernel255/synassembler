//#include "StdAfx.h"
#include "VirtualElaborationUnit.h"

VirtualElaborationUnit::VirtualElaborationUnit(ModuleServices* pService, int numProperties, const EUKind* kind) : AbstractElaborationUnit(pService, numProperties, kind)
{
}

VirtualElaborationUnit::~VirtualElaborationUnit()
{
}

void VirtualElaborationUnit::allocate(void)
{
}
void VirtualElaborationUnit::deallocate(void)
{
}
void VirtualElaborationUnit::play(void)
{
}
void VirtualElaborationUnit::pause(void)
{
}