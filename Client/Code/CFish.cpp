#include "pch.h"
#include "CFish.h"

CFish::CFish(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CFish::CFish(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CFish::~CFish()
{
}

HRESULT CFish::Ready_GameObject()
{
	return S_OK;
}

_int CFish::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CFish::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CFish::Render_GameObject()
{
}

HRESULT CFish::Add_Component()
{
	return S_OK;
}

CFish* CFish::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CFish::Free()
{
}