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
	// m_pTransformCom->Set_Pos(8.f, m_pTransformCom->Get_Scale().y * 0.5f, -1.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);	// 삭제 예정
	
	m_iLidTexNum = 0;
	m_bTakeOut = true;


	return S_OK;
}

_int CIngredientStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	Ready_Lid();

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

	Render_Lid();
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

	CManagement::GetInstance()->Get_Layer(L"GameObject_Layer")->Add_GameObject(pIngredient->Get_SelfId(), pIngredient);

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

	pComponent = m_pLidBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_LidBuffer", pComponent });

	pComponent = m_pLidTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_LidTransform", pComponent });

	pComponent = m_pLidTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationLidTexture_Ingredient"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_LidTexture", pComponent });

	return S_OK;
}

void CIngredientStation::Ready_Lid()
{
	if (m_bReadyLid)	
		return;

	_vec3 thisPos;  m_pTransformCom->Get_Info(INFO_POS, &thisPos);
	m_pLidTransformCom->Set_Scale({ 0.5f, 0.5f, 0.5f });
	m_pLidTransformCom->Set_Pos(thisPos.x, thisPos.y + 0.26f, thisPos.z);
	m_pLidTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_bReadyLid = true;
}

void CIngredientStation::Render_Lid()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pLidTransformCom->Get_World());

	m_pLidTextureCom->Set_Texture(m_iLidTexNum);

	if (FAILED(Set_Material()))
		return;

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
	m_pLidBufferCom->Render_Buffer();
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

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

void CIngredientStation::Set_TypeIngredientStation(const _tchar* create_name)
{
	if (_tcscmp(create_name, L"Create_Seaweed") == 0) {
		m_eTypeIngredient = CIngredient::SEAWEED;
		m_szIngredientName = L"Seaweed";
		m_iLidTexNum = 5;
	}
	else if (_tcscmp(create_name, L"Create_Lettuce") == 0) {
		m_eTypeIngredient = CIngredient::LETTUCE;
		m_szIngredientName = L"Lettuce";
		m_iLidTexNum = 0;
	}
	else if (_tcscmp(create_name, L"Create_Tomato") == 0) {
		m_eTypeIngredient = CIngredient::TOMATO;
		m_szIngredientName = L"Tomato";
		m_iLidTexNum = 1;
	}
	else if (_tcscmp(create_name, L"Create_Cucumber") == 0) {
		m_eTypeIngredient = CIngredient::CUCUMBER;
		m_szIngredientName = L"Cucumber";
		m_iLidTexNum = 2;
	}
	else if (_tcscmp(create_name, L"Create_Fish") == 0) {
		m_eTypeIngredient = CIngredient::FISH;
		m_szIngredientName = L"Fish";
		m_iLidTexNum = 3;
	}
	else if (_tcscmp(create_name, L"Create_Shrimp") == 0) {
		m_eTypeIngredient = CIngredient::SHRIMP;
		m_szIngredientName = L"Shrimp";
		m_iLidTexNum = 4;
	}
	else if (_tcscmp(create_name, L"Create_Rice") == 0) {
		m_eTypeIngredient = CIngredient::RICE;
		m_szIngredientName = L"Rice";
		m_iLidTexNum = 6;
	}
	else if (_tcscmp(create_name, L"Create_Pasta") == 0) {
		m_eTypeIngredient = CIngredient::PASTA;
		m_szIngredientName = L"Pasta";
		m_iLidTexNum = 7;
	}
	else if (_tcscmp(create_name, L"Create_Tomatosoup") == 0) {
		m_eTypeIngredient = CIngredient::TOMATOSOUP;
		m_szIngredientName = L"Tomatosoup";
		m_iLidTexNum = 1;
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