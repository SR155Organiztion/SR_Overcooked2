#include "pch.h"
#include "CPasta.h"

CPasta::CPasta(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CPasta::CPasta(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CPasta::~CPasta()
{
}

HRESULT CPasta::Ready_GameObject()
{
	return S_OK;
}

_int CPasta::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CPasta::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CPasta::Render_GameObject()
{
}

HRESULT CPasta::Add_Component()
{
	return S_OK;
}

CPasta* CPasta::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CPasta::Free()
{
}