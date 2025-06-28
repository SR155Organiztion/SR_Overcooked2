#include "pch.h"
#include "CUiMgr.h"
#include "CUi_Button.h"

CUiMgr::CUiMgr(LPDIRECT3DDEVICE9 pGraphicDev): Engine::CGameObject(pGraphicDev)
{
}

CUiMgr::CUiMgr(const CGameObject& rhs): Engine::CGameObject(rhs)
{
}

CUiMgr::~CUiMgr()
{
}


HRESULT CUiMgr::Ready_GameObject()
{
	return S_OK;
}

void CUiMgr::Update_GameObject()
{
}

void CUiMgr::LateUpdate_GameObject()
{
}

void CUiMgr::Render_GameObject()
{
}


void CUiMgr::Add_GameObject()
{
}

void CUiMgr::Remove_GameObject()
{
}

void CUiMgr::Free()
{
}
