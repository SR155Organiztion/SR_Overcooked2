#include "pch.h"
#include "CUi.h"


CUi::CUi(LPDIRECT3DDEVICE9 pGraphicDev): Engine::CGameObject(pGraphicDev)
{
}

CUi::CUi(const CGameObject& rhs):Engine::CGameObject(rhs)
{
}

CUi::~CUi()
{
}

void CUi::Render_GameObject()
{
 
}

