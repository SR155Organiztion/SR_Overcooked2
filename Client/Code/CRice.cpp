#include "pch.h"
#include "CRice.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CFontMgr.h"
#include "CInteractMgr.h"

#include "IPlace.h"

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
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_eIngredientType = RICE;
	m_eCookState = RAW;
	m_pCurrentState = new IRawState();
	m_pTransformCom->Set_Pos(5.f, m_pTransformCom->Get_Scale().y, 2.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = true;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::CARRY, this);

	return S_OK;
}

_int CRice::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_pCurrentState)
		m_pCurrentState->Update_State(this, fTimeDelta);

	if (COOKED == m_eCookState || DONE == m_eCookState)
		Add_Progress(fTimeDelta);

	//// FMS µð¹ö±ë ÀÓ½Ã
	//if (RAW == m_eCookState && GetAsyncKeyState('B'))
	//	Set_Progress(1.f);//` Add_Progress(fTimeDelta, 0.5f);
	//swprintf_s(m_szTemp, L"½Ò : %d, %f", m_eCookState, m_fProgress);
	////

	return iExit;
}

void CRice::LateUpdate_GameObject(const _float& fTimeDelta)
{

	//// IPlace Å×½ºÆ®
	if (GetAsyncKeyState('I'))
	{
		list<CGameObject*>* pListStation = CInteractMgr::GetInstance()->Get_List(CInteractMgr::TOOL);
		CGameObject* pStation = nullptr;

		if (pListStation)
			pStation = pListStation->front();

		if (pStation)
			dynamic_cast<IPlace*>(pStation)->Set_Place(this, pStation);
	}
	//
	if (GetAsyncKeyState('J'))
	{
		list<CGameObject*>* pListStation = CInteractMgr::GetInstance()->Get_List(CInteractMgr::TOOL);
		CGameObject* pStation = nullptr;

		if (pListStation)
			pStation = pListStation->front();

		CGameObject* pObj = nullptr;

		if (pStation)
			pObj = dynamic_cast<IPlace*>(pStation)->Get_PlacedItem();

		if (nullptr == pObj)
			return;

		dynamic_cast<IPlace*>(pStation)->Set_Empty();

		dynamic_cast<CTransform*>(pObj->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(4.f, m_pTransformCom->Get_Scale().y * 0.5f, 6.f);
	}

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRice::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	int iIndex = 0;
	if (COOKED == m_eCookState)
		iIndex = 1;
	else if (DONE == m_eCookState)
		iIndex = 2;
	if (BURNT == m_eCookState)
		iIndex = 3;
	m_pTextureCom->Set_Texture(iIndex);

	m_pBufferCom->Render_Buffer();

	//// FMS µð¹ö±ë ÀÓ½Ã
	//_vec2   vPos{ 100.f, 150.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	////

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CRice::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_IngredientTexture_Rice"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CRice* CRice::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRice* pRice = new CRice(pGraphicDev);

	if (FAILED(pRice->Ready_GameObject()))
	{
		Safe_Release(pRice);
		MSG_BOX("Rice Create Failed");
		return nullptr;
	}

	return pRice;
}

void CRice::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::CARRY, this);
	CIngredient::Free();
}