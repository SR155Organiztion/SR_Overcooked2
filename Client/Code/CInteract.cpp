#include "pch.h"
#include "CInteract.h"

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