#include "pch.h"
#include "CEmptyStation.h"

CEmptyStation::CEmptyStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CEmptyStation::CEmptyStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CEmptyStation::~CEmptyStation()
{
}

HRESULT CEmptyStation::Ready_GameObject()
{
	return S_OK;
}

_int CEmptyStation::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CEmptyStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CEmptyStation::Render_GameObject()
{
}

HRESULT CEmptyStation::Add_Component()
{
	return S_OK;
}

CEmptyStation* CEmptyStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CEmptyStation::Free()
{
}

_bool CEmptyStation::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CEmptyStation::Set_CarryTypes()
{
}