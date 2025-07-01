#include "pch.h"
#include "CTomato.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CFontMgr.h"

CTomato::CTomato(LPDIRECT3DDEVICE9 pGraphicDev)
	: CIngredient(pGraphicDev)
{
}

CTomato::CTomato(const CGameObject& rhs)
	: CIngredient(rhs)
{
}

CTomato::~CTomato()
{
}

HRESULT CTomato::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eType = TOMATO;
	m_eCookState = RAW;
	m_pCurrentState = new IRawState();
	m_pTransformCom->Set_Pos(2.f, m_pTransformCom->Get_Scale().y, 2.f);

	return S_OK;
}

_int CTomato::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_pCurrentState)
		m_pCurrentState->Update_State(this, fTimeDelta);

	//// FMS 디버깅 임시
	//if (GetAsyncKeyState('N'))
	//	Add_Progress(fTimeDelta, 0.5f);
	//swprintf_s(m_szProgress, L"토마토 : %d, %f", m_eCookState, m_fProgress);
	////

	return iExit;
}

void CTomato::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTomato::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	int iIndex = 0;
	if (DONE == m_eCookState || CHOPPED == m_eCookState)
		iIndex = 1;
	m_pTextureCom->Set_Texture(iIndex);

	m_pBufferCom->Render_Buffer();

	//// FMS 디버깅 임시
	//_vec2   vPos{ 100.f, 100.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szProgress, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	////

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CTomato::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_IngredientTexture_Tomato"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CTomato* CTomato::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTomato* pTomato = new CTomato(pGraphicDev);

	if (FAILED(pTomato->Ready_GameObject()))
	{
		Safe_Release(pTomato);
		MSG_BOX("Tomato Create Failed");
		return nullptr;
	}

	return pTomato;
}

void CTomato::Free()
{
	CIngredient::Free();
}