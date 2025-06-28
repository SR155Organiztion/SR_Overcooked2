#include "pch.h"
#include "CTomato.h"

CTomato::CTomato(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CTomato::CTomato(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CTomato::~CTomato()
{
}

HRESULT CTomato::Ready_GameObject()
{
	return S_OK;
}

_int CTomato::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CTomato::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CTomato::Render_GameObject()
{
}

HRESULT CTomato::Add_Component()
{
	return S_OK;
}

CTomato* CTomato::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CTomato::Free()
{
}