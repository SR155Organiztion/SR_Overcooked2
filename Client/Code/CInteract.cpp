#include "pch.h"
#include "CInteract.h"

CInteract::CInteract(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CInteract::CInteract(const CGameObject& rhs)
	: Engine::CGameObject(rhs)
{
}

CInteract::~CInteract()
{
}

void CInteract::Free()
{
}