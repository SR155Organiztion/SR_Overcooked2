#include "pch.h"
#include "CIngredientStation.h"

CIngredientStation::CIngredientStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CIngredientStation::CIngredientStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CIngredientStation::~CIngredientStation()
{
}

HRESULT CIngredientStation::Ready_GameObject()
{
	return S_OK;
}

_int CIngredientStation::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CIngredientStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CIngredientStation::Render_GameObject()
{
}

HRESULT CIngredientStation::Add_Component()
{
	return S_OK;
}

CIngredientStation* CIngredientStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CIngredientStation::Free()
{
}

_bool CIngredientStation::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CIngredientStation::Set_CarryTypes()
{
}