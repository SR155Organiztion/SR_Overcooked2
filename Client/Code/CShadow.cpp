#include "pch.h"
#include "CShadow.h"
#include "CRenderer.h"

CShadow::CShadow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CShadow::CShadow(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CShadow::~CShadow()
{
}

HRESULT CShadow::Ready_GameObject() {
	return S_OK;
}
_int CShadow::Update_GameObject(const _float& fTimeDelta) {
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;

}
void CShadow::LateUpdate_GameObject(const _float& fTimeDelta) {

}
void CShadow::Render_GameObject() {
	m_pVIBuffer->Render_Buffer();
}

CShadow* CShadow::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	CShadow* pShadow = new CShadow(pGraphicDev);

	if (FAILED(pShadow->Ready_GameObject()))
	{
		Safe_Release(pShadow);
		MSG_BOX("pShadow Failed");
		return nullptr;
	}

	return pShadow;
}

void CShadow::Free() {
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
}