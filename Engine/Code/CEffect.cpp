#include "CEffect.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CEffect::~CEffect()
{
}

_int CEffect::Update_Effect(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	return S_OK;
}

void CEffect::LateUpdate_Effect(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CEffect::Free()
{
	CGameObject::Free();
}
