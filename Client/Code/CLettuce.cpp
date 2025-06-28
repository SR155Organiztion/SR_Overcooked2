#include "pch.h"
#include "CLettuce.h"

CLettuce::CLettuce(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CLettuce::CLettuce(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CLettuce::~CLettuce()
{
}

HRESULT CLettuce::Ready_GameObject()
{
	return S_OK;
}

_int CLettuce::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CLettuce::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CLettuce::Render_GameObject()
{
}

HRESULT CLettuce::Add_Component()
{
	return S_OK;
}

CLettuce* CLettuce::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CLettuce::Free()
{
}