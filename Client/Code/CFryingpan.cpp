#include "pch.h"
#include "CFryingpan.h"
#include "CInteractMgr.h"

CFryingpan::CFryingpan(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CFryingpan::CFryingpan(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CFryingpan::~CFryingpan()
{
}

HRESULT CFryingpan::Ready_GameObject()
{
	//CInteractMgr::GetInstance()->Add_List(CInteractMgr::CARRY, this);

	return S_OK;
}

_int CFryingpan::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CFryingpan::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CFryingpan::Render_GameObject()
{
}

HRESULT CFryingpan::Add_Component()
{
	return S_OK;
}

CFryingpan* CFryingpan::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CFryingpan::Free()
{
	//CInteractMgr::GetInstance()->Remove_List(CInteractMgr::CARRY, this);
}

_bool CFryingpan::CanCook(CIngredient* pIngredient) const
{
	return _bool();
}

void CFryingpan::Cook(CIngredient* pIngredient)
{
}

_bool CFryingpan::Get_CanCarry() const
{
	return _bool();
}

_bool CFryingpan::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CFryingpan::Set_CarryTypes()
{
}