#include "pch.h"
#include "CLettuce.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

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
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(2.f, 0.f, 2.f);

	return S_OK;
}

_int CLettuce::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CLettuce::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CLettuce::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CLettuce::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_IngredientTexture_Lettuce"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CLettuce* CLettuce::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLettuce* pLettuce = new CLettuce(pGraphicDev);

	if (FAILED(pLettuce->Ready_GameObject()))
	{
		Safe_Release(pLettuce);
		MSG_BOX("Lettuce Create Failed");
		return nullptr;
	}

	return pLettuce;
}

void CLettuce::Free()
{
	Engine::CGameObject::Free();
}