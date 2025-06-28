#include "pch.h"
#include "CSeaweed.h"

CSeaweed::CSeaweed(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CSeaweed::CSeaweed(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CSeaweed::~CSeaweed()
{
}

HRESULT CSeaweed::Ready_GameObject()
{
	return S_OK;
}

_int CSeaweed::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CSeaweed::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CSeaweed::Render_GameObject()
{
}

HRESULT CSeaweed::Add_Component()
{
	return S_OK;
}

CSeaweed* CSeaweed::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CSeaweed::Free()
{
}