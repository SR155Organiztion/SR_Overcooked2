#include "pch.h"
#include "CChopStation.h"

CChopStation::CChopStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CChopStation::CChopStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CChopStation::~CChopStation()
{
}

HRESULT CChopStation::Ready_GameObject()
{
	return S_OK;
}

_int CChopStation::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CChopStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CChopStation::Render_GameObject()
{
}

HRESULT CChopStation::Add_Component()
{
	return S_OK;
}

CChopStation* CChopStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CChopStation::Free()
{
}

_bool CChopStation::CanChop(CIngredient* pIngredient) const
{
	return _bool();
}

void CChopStation::Chop(CIngredient* pIngredient)
{
}

_bool CChopStation::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CChopStation::Set_CarryTypes()
{
}