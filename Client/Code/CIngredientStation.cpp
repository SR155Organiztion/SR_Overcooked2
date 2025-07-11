#include "pch.h"
#include "CIngredientStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CInteractMgr.h"
#include "CIngredient.h"
#include "CObjectPoolMgr.h"
#include <algorithm>
#include "CManagement.h"

CIngredientStation::CIngredientStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CIngredientStation::CIngredientStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CIngredientStation::~CIngredientStation()
{
}

HRESULT CIngredientStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });
	m_pTransformCom->Set_Pos(8.f, m_pTransformCom->Get_Scale().y * 0.5f, -1.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);	// 삭제 예정

	//m_szOwnIngredient = L"Lettuce";				//경인테스트
	m_bTakeOut = true;

	return S_OK;
}

_int CIngredientStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CIngredientStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CIngredientStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();
}

_bool CIngredientStation::Get_CanPlace(CGameObject* pItem)
{
	// 모든 재료 / 도구 / 소화기
	return true;
}

CGameObject* CIngredientStation::TakeOut_Ingredient()
{
	if (!m_bTakeOut || CIngredient:: ING_END == m_eTypeIngredient)
		return nullptr;

	CGameObject* pIngredient = CObjectPoolMgr::GetInstance()->Get_Object(m_szIngredientName);
	if (!pIngredient)
		return nullptr;

	CManagement::GetInstance()->Get_Layer(L"GameObject_Layer");
	

	return pIngredient;
}

HRESULT CIngredientStation::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Ingredient"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CIngredientStation* CIngredientStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CIngredientStation* pIngredientStation = new CIngredientStation(pGraphicDev);

	if (FAILED(pIngredientStation->Ready_GameObject()))
	{
		Safe_Release(pIngredientStation);
		MSG_BOX("Station_Ingredient Failed");
		return nullptr;
	}

	return pIngredientStation;
}

void CIngredientStation::Set_TypeIngredientStation(std::wstring create_name)
{
	m_szIngredientName = create_name.substr(7);

	if (m_szIngredientName == L"") {
		MSG_BOX("Ingredient Station Name Set Failed... reason : No Name");
		return;
	}

	if (L"Seaweed" == m_szIngredientName) {
		m_eTypeIngredient = CIngredient::SEAWEED;
	}
	else if (L"Lettuce" == m_szIngredientName) {
		m_eTypeIngredient = CIngredient::LETTUCE;
	}
	else if (L"Tomato" == m_szIngredientName) {
		m_eTypeIngredient = CIngredient::TOMATO;
	}
	else if (L"Cucumber" == m_szIngredientName) {
		m_eTypeIngredient = CIngredient::CUCUMBER;
	}
	else if (L"Fish" == m_szIngredientName) {
		m_eTypeIngredient = CIngredient::FISH;
	}
	else if (L"Shrimp" == m_szIngredientName) {
		m_eTypeIngredient = CIngredient::SHRIMP;
	}
	else if (L"Rice" == m_szIngredientName) {
		m_eTypeIngredient = CIngredient::RICE;
	}
	else if (L"Pasta" == m_szIngredientName) {
		m_eTypeIngredient = CIngredient::PASTA;
	}
	else if (L"Tomatosoup" == m_szIngredientName) {
		m_eTypeIngredient = CIngredient::TOMATOSOUP;
	}
	else {
		MSG_BOX("Ingredient Station Name Set Failed... reason : Cant Find correct Name");
		return;
	}
}

void CIngredientStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);	// 삭제 예정
	Engine::CGameObject::Free();
}