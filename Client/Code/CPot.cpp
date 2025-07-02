#include "pch.h"
#include "CPot.h"
#include "CInteractMgr.h"

CPot::CPot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CPot::CPot(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CPot::~CPot()
{
}

HRESULT CPot::Ready_GameObject()
{
	//CInteractMgr::GetInstance()->Add_List(CInteractMgr::CARRY, this);

	return S_OK;
}

_int CPot::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CPot::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CPot::Render_GameObject()
{
}

HRESULT CPot::Add_Component()
{
	return S_OK;
}

CPot* CPot::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CPot::Free()
{
	//CInteractMgr::GetInstance()->Remove_List(CInteractMgr::CARRY, this);
}

_bool CPot::CanCook(CIngredient* pIngredient) const
{
	return _bool();
}

void CPot::Cook(CIngredient* pIngredient)
{
}

_bool CPot::Get_CanCarry() const
{
	return _bool();
}

_bool CPot::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CPot::Set_CarryTypes()
{
}
