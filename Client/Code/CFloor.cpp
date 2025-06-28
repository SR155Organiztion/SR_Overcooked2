#include "pch.h"
#include "CFloor.h"

CFloor::CFloor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CFloor::CFloor(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CFloor::~CFloor()
{
}

HRESULT CFloor::Ready_GameObject()
{
    return S_OK;
}

_int CFloor::Update_GameObject(const _float& fTimeDelta)
{
    return 0;
}

void CFloor::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CFloor::Render_GameObject()
{
}

HRESULT CFloor::Add_Component()
{
    CComponent* pComponent = nullptr;
    return S_OK;
}


CFloor* CFloor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    return nullptr;
}

void CFloor::Free()
{
}