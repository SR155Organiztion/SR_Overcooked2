#include"CInteractMgr.h"

IMPLEMENT_SINGLETON(CInteractMgr)

CInteractMgr::CInteractMgr()
{
}

CInteractMgr::~CInteractMgr()
{
	Free();
}

list<CInteract*>* CInteractMgr::Get_List(INTERACT eType) const
{
	return nullptr;
}

void CInteractMgr::Clear_List()
{
}

void CInteractMgr::Add_List(INTERACT eType, CInteract* pInteract)
{
}

void CInteractMgr::Free()
{
}