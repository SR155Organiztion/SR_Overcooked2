#include "pch.h"
#include "CPlate.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

#include "CInteractMgr.h"
#include "CIngredient.h"
#include "CFontMgr.h"

CPlate::CPlate(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CPlate::CPlate(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CPlate::~CPlate()
{
}

HRESULT CPlate::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(2.f, m_pTransformCom->Get_Scale().y, 8.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::TOOL, this);	// 삭제 예정

	return S_OK;
}

_int CPlate::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	swprintf_s(m_szTemp, L"%s", m_szMenu);

	//if (GetAsyncKeyState('1'))
	//	Add_Ingredient(L"pasta");
	//else if (GetAsyncKeyState('2'))
	//	Add_Ingredient(L"tomatosoup");
	//else if (GetAsyncKeyState('3'))
	//	Add_Ingredient(L"seaweed");

	return iExit;
}

void CPlate::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlate::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	_vec2   vPos{ 100.f, 300.f };
	CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
}

_bool CPlate::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	if (nullptr == pItem || nullptr == pPlace)
		return false;

	if (!Get_CanPlace(pItem))
		return false;

	CIngredient* pIngredient = dynamic_cast<CIngredient*>(pItem);
	if (pIngredient)
	{
		CIngredient::INGREDIENT_TYPE eType = pIngredient->Get_IngredientType();
		switch (eType)
		{
		case CIngredient::SEAWEED:
			Add_Ingredient(L"seaweed");
			break;
		case CIngredient::LETTUCE:
			Add_Ingredient(L"lettuce");
			break;
		case CIngredient::TOMATO:
			Add_Ingredient(L"tomato");
			break;
		case CIngredient::CUCUMBER:
			Add_Ingredient(L"cucumber");
			break;
		case CIngredient::FISH:
			Add_Ingredient(L"fish");
			break;;
		case CIngredient::SHRIMP:
			Add_Ingredient(L"shrimp");
			break;
		}
		
		// 재료 삭제
		return true;
	}

	return false;
}

_bool CPlate::Get_CanPlace(CGameObject* pItem)
{
	CIngredient* pIngredient = dynamic_cast<CIngredient*>(pItem);
	if (pIngredient)
	{
		if (CIngredient::DONE == pIngredient->Get_State())
			return true;
	}

	return false;
}

HRESULT CPlate::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlateTexture_Plate"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CPlate::Add_Ingredient(const _tchar* pTag)
{
	auto iter = find_if(m_vecMenu.begin(), m_vecMenu.end(), [pTag](const wstring& str)
		{ 
			return !lstrcmp(str.c_str(), pTag); 
		});

	if (iter != m_vecMenu.end())
		return;

	m_vecMenu.push_back(pTag);

	sort(m_vecMenu.begin(), m_vecMenu.end());

	_tchar szMenu[128];
	swprintf_s(szMenu, L"Proto_PlateTexture_Plate");

	for (const auto& ingredient : m_vecMenu)
	{
		lstrcat(szMenu, L"_");
		lstrcat(szMenu, ingredient.c_str());
	}

	lstrcpy(m_szMenu, szMenu);

	if (FAILED(Change_Texture(szMenu)))
	{
		Change_Texture(L"Proto_PlateTexture_Plate");
		MSG_BOX("잘못된 메뉴 조합이다~");
		return;
	} 
}

HRESULT CPlate::Change_Texture(const _tchar* pComponentTag)
{
	CComponent* pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(pComponentTag));
	if (nullptr == pComponent)
		return E_FAIL;

	auto iter = find_if(m_mapComponent[ID_DYNAMIC].begin(), m_mapComponent[ID_DYNAMIC].end(), CTag_Finder(L"Com_Texture"));
	if (iter == m_mapComponent[ID_DYNAMIC].end())
		return E_FAIL;
	
	Safe_Release(iter->second);
	m_mapComponent[ID_DYNAMIC].erase(iter);

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CPlate* CPlate::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlate* pPlate = new CPlate(pGraphicDev);

	if (FAILED(pPlate->Ready_GameObject()))
	{
		Safe_Release(pPlate);
		MSG_BOX("Plate Create Failed");
		return nullptr;
	}

	return pPlate;
}

void CPlate::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::TOOL, this);	// 삭제 예정
	CInteract::Free();
}