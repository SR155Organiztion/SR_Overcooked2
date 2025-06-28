#include "pch.h"
#include "CFireExtinguisher.h"

CFireExtinguisher::CFireExtinguisher(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CFireExtinguisher::CFireExtinguisher(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CFireExtinguisher::~CFireExtinguisher()
{
}

HRESULT CFireExtinguisher::Ready_GameObject()
{
	return S_OK;
}

_int CFireExtinguisher::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CFireExtinguisher::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CFireExtinguisher::Render_GameObject()
{
}

HRESULT CFireExtinguisher::Add_Component()
{
	return S_OK;
}

CFireExtinguisher* CFireExtinguisher::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CFireExtinguisher::Free()
{
}

_bool CFireExtinguisher::Get_CanCarry() const
{
	return _bool();
}