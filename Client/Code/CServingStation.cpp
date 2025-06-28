#include "pch.h"
#include "CServingStation.h"

CServingStation::CServingStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CServingStation::CServingStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CServingStation::~CServingStation()
{
}

HRESULT CServingStation::Ready_GameObject()
{
	return S_OK;
}

_int CServingStation::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CServingStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CServingStation::Render_GameObject()
{
}

HRESULT CServingStation::Add_Component()
{
	return S_OK;
}

CServingStation* CServingStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CServingStation::Free()
{
}

_bool CServingStation::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CServingStation::Set_CarryTypes()
{
}