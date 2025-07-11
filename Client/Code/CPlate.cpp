#include "pch.h"
#include "CPlate.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CIngredient.h"

#include "CInteractMgr.h" 
#include "CFontMgr.h"
#include "CManagement.h"
#include "CObjectPoolMgr.h"

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
	//m_pTransformCom->Set_Pos((_float)(rand() % 3) + 8, m_pTransformCom->Get_Scale().y, (_float)(rand() % 3) + 6);

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

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	swprintf_s(m_szTemp, L"접시\n%p\n%d", &m_setIngredient, (int)m_setIngredient.size());	// 디버깅

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

	//_vec2   vPos{ 100.f, 400.f };
	//CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_szTemp, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
}

void CPlate::Clear_Plate()
{
	m_setIngredient.clear();
	swprintf_s(m_szName, L"Proto_PlateTexture_Plate");
	Change_Texture(m_szName);
}

_bool CPlate::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	// nullptr 검사
	if (!pItem || !pPlace)
		return false;

	if (!Get_CanPlace(pItem))
		return false;

	// pItem이 재료 또는 도구(냄비 또는 후라이팬) 일 수도 있어서 재료 가져오는 부분
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (!pInteract)
		return false;

	CInteract::INTERACTTYPE eInteractType = pInteract->Get_InteractType();

	if (CInteract::INGREDIENT == eInteractType)
	{
		CIngredient* pIngredient = dynamic_cast<CIngredient*>(pInteract);
		if (!pIngredient)
			return false;

		const _tchar* pIngredientTag = IngredientTypeToString(pIngredient->Get_IngredientType());

		if (!Add_Ingredient(pIngredientTag))
			return false;

		CObjectPoolMgr::GetInstance()->Return_Object(pIngredient->Get_SelfId(), pIngredient);
		CManagement::GetInstance()->Delete_GameObject(L"GameObject_Layer", pIngredient->Get_SelfId(), pItem);

		return true;
	}
	else if (CInteract::FRYINGPAN == eInteractType || CInteract::POT == eInteractType)
	{
		IPlace* pPlace = dynamic_cast<IPlace*>(pInteract);
		if (!pPlace)
			return false;

		CIngredient* pIngredient = dynamic_cast<CIngredient*>(pPlace->Get_Item());

		const _tchar* pIngredientTag = IngredientTypeToString(pIngredient->Get_IngredientType());

		if (!Add_Ingredient(pIngredientTag))
			return false;

		if (IPlace* pPlace = dynamic_cast<IPlace*>(pItem))
			pPlace->Set_Empty();

		return true;
	}

	return false;
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

_bool CPlate::Add_Ingredient(const _tchar* pTag)
{
	if (!pTag)
		return false;

    auto it = m_setIngredient.insert(pTag);

	if (false == it.second)	//	set은 중복 키 값이 들어갈 수 없음
		return false;

	_tchar szMenu[256];
	swprintf_s(szMenu, L"Proto_PlateTexture_Plate");

	for (const auto& ingredient : m_setIngredient)
	{
		lstrcat(szMenu, L"_");
		lstrcat(szMenu, ingredient.c_str());
	}

	lstrcpy(m_szName, szMenu);

	if (false == Change_Texture(szMenu))
	{
		MSG_BOX("잘못된 메뉴 조합이다~");	//	일단 잘못된 조합일 경우 빈 그릇으로

		m_setIngredient.clear();
		swprintf_s(m_szName, L"Proto_PlateTexture_Plate");
		Change_Texture(m_szName);

		return false;
	} 

	return true;
}

_bool CPlate::Change_Texture(const _tchar* pComponentTag)
{
	Engine::CTexture* pNewTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(pComponentTag));
	if (nullptr == pNewTextureCom)
		return false;

	auto iter = find_if(m_mapComponent[ID_DYNAMIC].begin(), m_mapComponent[ID_DYNAMIC].end(), CTag_Finder(L"Com_Texture"));
	if (iter != m_mapComponent[ID_DYNAMIC].end())
	{
		Safe_Release(iter->second);
		m_mapComponent[ID_DYNAMIC].erase(iter);
	}
	
	m_pTextureCom = pNewTextureCom;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pNewTextureCom });

	return true;
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
	case CIngredient::TOMATOSOUP:
		return L"tomatosoup";
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