#include "pch.h"
#include "CEmptyStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CInteractMgr.h"
#include "CIngredient.h"

#include "CFontMgr.h"

CEmptyStation::CEmptyStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CEmptyStation::CEmptyStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CEmptyStation::~CEmptyStation()
{
}

HRESULT CEmptyStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(4.5f, m_pTransformCom->Get_Scale().y * 0.5f, 8.f);
	//m_pTransformCom->Set_Pos(10.f, m_pTransformCom->Get_Scale().y, 10.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);

	return S_OK;
}

_int CEmptyStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	//swprintf_s(m_szProgress, L"%d, %p", m_bFull, m_pPlacedItem);

	return iExit;
}

void CEmptyStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CEmptyStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//_vec2   vPos{ 100.f, 200.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szProgress, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	//
}

_bool CEmptyStation::Get_CanPlace(CGameObject* pItem)
{
	// 모든 재료 / 도구 / 소화기
	return true;
}

HRESULT CEmptyStation::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Empty"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CEmptyStation* CEmptyStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CEmptyStation* pEmptyStation = new CEmptyStation(pGraphicDev);

	if (FAILED(pEmptyStation->Ready_GameObject()))
	{
		Safe_Release(pEmptyStation);
		MSG_BOX("Station_Empty Failed");
		return nullptr;
	}

	return pEmptyStation;
}

void CEmptyStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
	Engine::CGameObject::Free();
}