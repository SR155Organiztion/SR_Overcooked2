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

_int CUiMgr::Update_GameObject(const _float& _fTimeDelta)
{
	/*CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);*/
	return 0;
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
