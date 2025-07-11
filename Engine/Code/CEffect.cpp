#include "CEffect.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_bActive(false)
{
}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs), m_bActive(false)
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
