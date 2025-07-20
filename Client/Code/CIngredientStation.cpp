#include "pch.h"
#include "CIngredientStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
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

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bIsStation = true;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;
	
	m_iLidTexNum = 0;

	return S_OK;
}

_int CIngredientStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	Ready_Lid();

	return iExit;
}

void CIngredientStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CIngredientStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	for (int i = 0; i < (int)m_bHighlight + 1; ++i)
	{
		if (m_vecTextureCom.size() > i && m_vecTextureCom[i])
		{
			m_vecTextureCom[i]->Set_Texture(0);
			if (FAILED(Set_Material()))
				return;
			m_pBufferCom->Render_Buffer();
		}
	}

	Render_Lid();
}

_bool CIngredientStation::Get_CanPlace(CGameObject* pItem)
{
	// 모든 재료 / 도구 / 소화기
	return true;
}

CGameObject* CIngredientStation::TakeOut_Ingredient()
{
	if (m_bFull || CIngredient:: ING_END == m_eTypeIngredient)
		return nullptr;

	_vec3 vPos{};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CGameObject* pIngredient = CObjectPoolMgr::GetInstance()->Get_Object(m_szIngredientName, vPos);
	if (!pIngredient)
		return nullptr;

	dynamic_cast<CIngredient*>(pIngredient)->Init();
	CManagement::GetInstance()->Get_Layer(L"GameObject_Layer")->Add_GameObject(pIngredient->Get_SelfId(), pIngredient);

	return pIngredient;
}

_bool CIngredientStation::On_Snap(CGameObject* _pGameObject)
{
	if (dynamic_cast<CIngredient*>(_pGameObject)) {
		if (m_bFull) {
			IPlace* pTool = dynamic_cast<IPlace*>(m_pPlacedItem);
			if (pTool) {
				if (pTool->Set_Place(_pGameObject, m_pPlacedItem)) {
					dynamic_cast<CIngredient*>(_pGameObject)->Set_Ground(true);
					return true;
				}
			}
			return false;
		}
		else {
			Set_Place(_pGameObject, this);
			dynamic_cast<CIngredient*>(_pGameObject)->Set_Ground(true);
			return true;
		}
	}
	return false;
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Ingredient"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Alpha"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture_Alpha", pComponent });

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
		
	m_pLidBufferCom->Render_Buffer();
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
	CInteract::Free();
}