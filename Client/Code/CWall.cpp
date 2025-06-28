#include "pch.h"
#include "CWall.h"

CWall::CWall(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CWall::CWall(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CWall::~CWall()
{
}

HRESULT CWall::Ready_GameObject()
{
    return S_OK;
}

_int CWall::Update_GameObject(const _float& fTimeDelta)
{
    return 0;
}

void CWall::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CWall::Render_GameObject()
{
}

HRESULT CWall::Add_Component()
{
    CComponent* pComponent = nullptr;
    return S_OK;
}


CWall* CWall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    return nullptr;
}

void CWall::Free()
{
}