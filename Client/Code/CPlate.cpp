#include "pch.h"
#include "CPlate.h"

CPlate::CPlate(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CPlate::CPlate(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CPlate::~CPlate()
{
}

HRESULT CPlate::Ready_GameObject()
{
	return S_OK;
}

_int CPlate::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CPlate::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CPlate::Render_GameObject()
{
}

HRESULT CPlate::Add_Component()
{
	return S_OK;
}

CPlate* CPlate::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CPlate::Free()
{
}

_bool CPlate::CanPlate(CIngredient* pIngredient) const
{
	return _bool();
}

void CPlate::Plate(CIngredient* pIngredient)
{
}

_bool CPlate::Get_CanCarry() const
{
	return _bool();
}

_bool CPlate::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CPlate::Set_CarryTypes()
{
}
