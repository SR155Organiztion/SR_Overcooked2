#include "pch.h"
#include "CMushroom.h"

CMushroom::CMushroom(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CMushroom::CMushroom(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CMushroom::~CMushroom()
{
}

HRESULT CMushroom::Ready_GameObject()
{
	return S_OK;
}

_int CMushroom::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CMushroom::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CMushroom::Render_GameObject()
{
}

HRESULT CMushroom::Add_Component()
{
	return S_OK;
}

CMushroom* CMushroom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CMushroom::Free()
{
}