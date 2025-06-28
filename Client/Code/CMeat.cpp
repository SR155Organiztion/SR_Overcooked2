#include "pch.h"
#include "CMeat.h"

CMeat::CMeat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CMeat::CMeat(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CMeat::~CMeat()
{
}

HRESULT CMeat::Ready_GameObject()
{
	return S_OK;
}

_int CMeat::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CMeat::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CMeat::Render_GameObject()
{
}

HRESULT CMeat::Add_Component()
{
	return S_OK;
}

CMeat* CMeat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CMeat::Free()
{
}