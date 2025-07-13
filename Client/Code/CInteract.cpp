#include "pch.h"
#include "CInteract.h"

CInteract::CInteract(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bGround(false), m_bHighlight(false)
{
	ZeroMemory(m_szTemp, sizeof(m_szTemp));
}

CInteract::CInteract(const CGameObject& rhs)
	: Engine::CGameObject(rhs), m_bGround(false), m_bHighlight(false)
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