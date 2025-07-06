#include "pch.h"
#include "CTomato.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "IState.h"
#include "CFontMgr.h"
#include "CInteractMgr.h"

#include "IPlace.h"

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

	m_eIngredientType = TOMATO;
	m_eCookState = RAW;
	m_pCurrentState = new IRawState();
	m_pTransformCom->Set_Pos(6.f, m_pTransformCom->Get_Scale().y, 2.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = true;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::CARRY, this);	// 삭제 예정

	return S_OK;
}

_int CTomato::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_pCurrentState)
		m_pCurrentState->Update_State(this, fTimeDelta);

	swprintf_s(m_szTemp, L"토마토\n%p\n%d", m_pCurrentState, m_eCookState);	// 디버깅

	return iExit;
}

void CTomato::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	////// IPlace 테스트
	//if (GetAsyncKeyState('1'))
	//{
	//	list<CGameObject*>* pListStation = CInteractMgr::GetInstance()->Get_List(CInteractMgr::TOOL);
	//	CGameObject* pStation = nullptr;
	//
	//	if (nullptr == pListStation || 0 >= pListStation->size())
	//		return;
	//	
	//	pStation = pListStation->front();
	//	dynamic_cast<IPlace*>(pStation)->Set_Place(this, pStation);
	//}
	////
	//if (GetAsyncKeyState('2'))
	//{
	//	list<CGameObject*>* pListStation = CInteractMgr::GetInstance()->Get_List(CInteractMgr::STATION);
	//	CGameObject* pStation = nullptr;
	//
	//	if (nullptr == pListStation || 0 >= pListStation->size())
	//		return;
	//	
	//	CGameObject* pObj = nullptr;
	//	pStation = pListStation->front();
	//	pObj = dynamic_cast<IPlace*>(pStation)->Get_PlacedItem();
	//
	//	if (nullptr == pObj)
	//		return;
	//
	//	dynamic_cast<CTransform*>(pObj->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(4.f, m_pTransformCom->Get_Scale().y * 0.5f, 6.f);
	//}
}

void CTomato::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	int iIndex = 0;
	if (RAW != m_eCookState)
		iIndex = 1;
	m_pTextureCom->Set_Texture(iIndex);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//_vec2   vPos{ 100.f, 100.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));	// 디버깅
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
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::CARRY, this);	// 삭제 예정
	CIngredient::Free();
}