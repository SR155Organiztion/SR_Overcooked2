#include "pch.h"
#include "CRice.h"

CRice::CRice(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CRice::CRice(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CRice::~CRice()
{
}

HRESULT CRice::Ready_GameObject()
{
	return S_OK;
}

_int CRice::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CRice::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CRice::Render_GameObject()
{
}

HRESULT CRice::Add_Component()
{
	return S_OK;
}

CRice* CRice::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CRice::Free()
{
}