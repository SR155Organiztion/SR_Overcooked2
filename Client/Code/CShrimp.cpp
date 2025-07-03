#include "pch.h"
#include "CShrimp.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CFontMgr.h"
#include "CInteractMgr.h"

#include "IPlace.h"

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
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eIngredientType = SHRIMP;
	m_eCookState = RAW;
	m_pCurrentState = new IRawState();
	m_pTransformCom->Set_Pos(6.f, m_pTransformCom->Get_Scale().y, 2.f);

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::CARRY, this);

	return S_OK;
}

_int CShrimp::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_pCurrentState)
		m_pCurrentState->Update_State(this, fTimeDelta);

	//// FMS 디버깅 임시
	//if (GetAsyncKeyState('N'))
	//	Add_Progress(fTimeDelta, 0.5f);
	//swprintf_s(m_szTemp, L"새우 : %d, %f", m_eCookState, m_fProgress);
	////

	return iExit;
}

void CShrimp::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	// IPlace 테스트
	if (GetAsyncKeyState('O'))
	{
		list<CGameObject*>* pListStation = CInteractMgr::GetInstance()->Get_List(CInteractMgr::STATION);
		CGameObject* pStation = nullptr;

		if (pListStation)
		{
			pStation = pListStation->front();
		}

		if (pStation)
			dynamic_cast<IPlace*>(pStation)->Set_Place(this, pStation);
	}
	//
}

void CShrimp::Render_GameObject()
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
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	////

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CShrimp::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_IngredientTexture_Shrimp"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CShrimp* CShrimp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShrimp* pShrimp = new CShrimp(pGraphicDev);

	if (FAILED(pShrimp->Ready_GameObject()))
	{
		Safe_Release(pShrimp);
		MSG_BOX("Shrimp Create Failed");
		return nullptr;
	}

	return pShrimp;
}

void CShrimp::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::CARRY, this);
	CIngredient::Free();
}