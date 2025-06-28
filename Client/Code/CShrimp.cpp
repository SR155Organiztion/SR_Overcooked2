#include "pch.h"
#include "CShrimp.h"

CShrimp::CShrimp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CShrimp::CShrimp(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CShrimp::~CShrimp()
{
}

HRESULT CShrimp::Ready_GameObject()
{
	return S_OK;
}

_int CShrimp::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CShrimp::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CShrimp::Render_GameObject()
{
}

HRESULT CShrimp::Add_Component()
{
	return S_OK;
}

CShrimp* CShrimp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CShrimp::Free()
{
}