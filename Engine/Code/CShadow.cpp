#include "CShadow.h"
#include "CRenderer.h"
#include "CProtoMgr.h"
#include "CVertexShader.h"

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
	if (FAILED(Add_Component()))
		return E_FAIL;
	return S_OK;
}
_int CShadow::Update_GameObject(const _float& fTimeDelta) {
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;

}
void CShadow::LateUpdate_GameObject(const _float& fTimeDelta) {
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}
void CShadow::Render_GameObject() {
	m_pShader->Render_Shader(m_pGraphicDev, m_pTransform->Get_World());
	m_pVIBuffer->Render_Buffer();
	m_pShader->End_RenderShader(m_pGraphicDev);
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


HRESULT CShadow::Add_Component() {
	CComponent* pComponent = nullptr;

	pComponent = m_pShader = dynamic_cast<Engine::CVertexShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_2DShader"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Shader", pComponent });

	return S_OK;
}

void CShadow::Free() {
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
}