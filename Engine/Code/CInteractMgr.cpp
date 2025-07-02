#include"CInteractMgr.h"
#include"CTransform.h"

IMPLEMENT_SINGLETON(CInteractMgr)

CInteractMgr::CInteractMgr()
{
}

CInteractMgr::~CInteractMgr()
{
	Free();
}

list<CGameObject*>* CInteractMgr::Get_List(INTERACT eType)
{
	if (eType < 0 || eType >= INTEND)
		return nullptr;

	return &m_list[eType];
}

void CInteractMgr::Add_List(INTERACT eType, CGameObject* pInteract)
{
	if (nullptr == pInteract || eType < 0 || eType >= INTEND)
		return;
	
	m_list[eType].push_back(pInteract);
}

void CInteractMgr::Remove_List(INTERACT eType, CGameObject* pInteract)
{
	if (nullptr == pInteract || eType < 0 || eType >= INTEND)
		return;

	m_list[eType].remove(pInteract);
}

void CInteractMgr::Free()
{
	for (int i = 0; i < INTEND; ++i)
		m_list[i].clear();
}