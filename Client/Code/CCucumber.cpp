#include "pch.h"
#include "CCucumber.h"

CCucumber::CCucumber(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CCucumber::CCucumber(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CCucumber::~CCucumber()
{
}

HRESULT CCucumber::Ready_GameObject()
{
	return S_OK;
}

_int CCucumber::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CCucumber::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CCucumber::Render_GameObject()
{
}

HRESULT CCucumber::Add_Component()
{
	return S_OK;
}

CCucumber* CCucumber::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CCucumber::Free()
{
}