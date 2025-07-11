#include "pch.h"
#include "CInteract.h"
#include <IPlace.h>

CInteract::CInteract(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	ZeroMemory(m_szTemp, sizeof(m_szTemp));
}

CInteract::CInteract(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
{
	ZeroMemory(m_szTemp, sizeof(m_szTemp));
}

CInteract::~CInteract()
{
}

void CInteract::Free()
{
	Engine::CGameObject::Free();
}

void CInteract::On_Snap(CGameObject* _pGameObject)
{
	CInteract* pCollision = dynamic_cast<CInteract*>(_pGameObject);
	if (!pCollision)
		return;

	INTERACTTYPE eID = pCollision->Get_InteractType();

	switch (eID) {
	case INGREDIENT:
		dynamic_cast<IPlace*>(this)->Set_Place(_pGameObject, this);
		m_stOpt.bIsSnap = true;
		break;
	}
}
