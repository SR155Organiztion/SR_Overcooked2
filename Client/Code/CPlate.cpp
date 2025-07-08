#include "pch.h"
#include "CPlate.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CIngredient.h"

#include "CInteractMgr.h" 
#include "CFontMgr.h"

CPlate::CPlate(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
	ZeroMemory(m_szName, sizeof(m_szName));
}

CPlate::CPlate(const CGameObject& rhs)
	: CInteract(rhs)
{
	ZeroMemory(m_szName, sizeof(m_szName));
}

CPlate::~CPlate()
{
}

HRESULT CPlate::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Pos(2.f, m_pTransformCom->Get_Scale().y, 6.f);

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

	_vec2   vPos{ 100.f, 400.f };
	CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szName, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
}

_bool CPlate::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	if (!pItem || !pPlace)
		return false;

	if (!Get_CanPlace(pItem))
		return false;

	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (!pInteract)
		return false;

	CInteract::INTERACTTYPE eInteractType = pInteract->Get_InteractType();
	CIngredient* pIngredient = nullptr;

	if (CInteract::INGREDIENT == eInteractType)
	{
		pIngredient = dynamic_cast<CIngredient*>(pInteract);
		if (!pIngredient)
			return false;
	}
	else if (CInteract::FRYINGPAN == eInteractType || CInteract::POT == eInteractType)
	{
		IPlace* pPlace = dynamic_cast<IPlace*>(pInteract);
		if (!pPlace)
			return false;

		pIngredient = dynamic_cast<CIngredient*>(pPlace->Get_Item());
		if (!pIngredient)
			return false;
	}

	if (!pIngredient)
		return false;
	const _tchar* pIngredientTag = IngredientTypeToString(pIngredient->Get_IngredientType());

	Add_Ingredient(pIngredientTag);

	// TODO : 재료 삭제 -> 오브젝트 풀링으로 대기 상태로 전환

	return true;
}

_bool CPlate::Get_CanPlace(CGameObject* pItem)
{
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (!pInteract)
		return false;

	CInteract::INTERACTTYPE eType = pInteract->Get_InteractType();
	if (CInteract::INGREDIENT == eType)
	{
		CIngredient* pIngredient = dynamic_cast<CIngredient*>(pInteract);
		if (pIngredient && CIngredient::DONE == dynamic_cast<CIngredient*>(pInteract)->Get_State())
			return true;
	}
	else if (CInteract::FRYINGPAN == eType || CInteract::POT == eType)
	{
		IPlace* pPlace = dynamic_cast<IPlace*>(pInteract);
		if (!pPlace)
			return false;

		CIngredient* pIngredient = dynamic_cast<CIngredient*>(pPlace->Get_Item());
		if (pIngredient && CIngredient::DONE == pIngredient->Get_State())
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
	if (!pTag)
		return;

	m_setIngredient.insert(pTag);

	_tchar szMenu[256];
	swprintf_s(szMenu, L"Proto_PlateTexture_Plate");

	for (const auto& ingredient : m_setIngredient)
	{
		lstrcat(szMenu, L"_");
		lstrcat(szMenu, ingredient.c_str());
	}

	lstrcpy(m_szName, szMenu);

	if (FAILED(Change_Texture(szMenu)))
	{
		MSG_BOX("잘못된 메뉴 조합이다~");	//	일단 잘못된 조합일 경우 빈 그릇으로

		m_setIngredient.clear();
		swprintf_s(m_szName, L"Proto_PlateTexture_Plate");
		Change_Texture(m_szName);

		return;
	} 
}

HRESULT CPlate::Change_Texture(const _tchar* pComponentTag)
{
	Engine::CTexture* pNewTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(pComponentTag));
	if (nullptr == pNewTextureCom)
		return E_FAIL;

	auto iter = find_if(m_mapComponent[ID_DYNAMIC].begin(), m_mapComponent[ID_DYNAMIC].end(), CTag_Finder(L"Com_Texture"));
	if (iter != m_mapComponent[ID_DYNAMIC].end())
	{
		Safe_Release(iter->second);
		m_mapComponent[ID_DYNAMIC].erase(iter);
	}
	
	m_pTextureCom = pNewTextureCom;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pNewTextureCom });

	return S_OK;
}

const _tchar* CPlate::IngredientTypeToString(CIngredient::INGREDIENT_TYPE eType)
{
	switch (eType)
	{
	case CIngredient::SEAWEED:
		return L"seaweed";
	case CIngredient::LETTUCE:
		return L"lettuce";
	case CIngredient::TOMATO:
		return L"tomato";
	case CIngredient::CUCUMBER:
		return L"cucumber";
	case CIngredient::FISH:
		return L"fish";
	case CIngredient::SHRIMP:
		return L"shrimp";
	case CIngredient::RICE:
		return L"rice";
	case CIngredient::PASTA:
		return L"pasta";
	default:
		return nullptr;
	}
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
	m_setIngredient.clear();

	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::TOOL, this);	// 삭제 예정
	CInteract::Free();
}