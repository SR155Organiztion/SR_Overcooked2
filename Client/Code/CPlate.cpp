#include "pch.h"
#include "CPlate.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CIngredient.h"
#include "CManagement.h"
#include "CObjectPoolMgr.h"
#include "CUi_Icon.h"

CPlate::CPlate(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
	ZeroMemory(m_szMenu, sizeof(m_szMenu));
}

CPlate::CPlate(const CGameObject& rhs)
	: CInteract(rhs)
{
	ZeroMemory(m_szMenu, sizeof(m_szMenu));
}

CPlate::~CPlate()
{
}

HRESULT CPlate::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	Set_State(CLEAN);

	return S_OK;
}

_int CPlate::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Ready_IconPool();
	Draw_Icon();

	_matrix matWorld;
	m_pTransformCom->Get_World(&matWorld);
	Billboard(matWorld);
	m_pTransformCom->Set_World(&matWorld);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CPlate::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlate::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

_bool CPlate::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	// nullptr 검사
	if (!pItem || !pPlace)
		return false;

	if (!Get_CanPlace(pItem))
		return false;

	if (DIRTY == m_ePlateState)
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

		Add_Icon(pIngredient->Get_IngredientType());

		pIngredient->Reset();
		CObjectPoolMgr::GetInstance()->Return_Object(pItem->Get_BaseId().c_str(), pItem);
		CManagement::GetInstance()->Delete_GameObject(L"GameObject_Layer", pItem->Get_SelfId(), pItem);

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

		Add_Icon(pIngredient->Get_IngredientType());

		pIngredient->Reset();
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

void CPlate::Reset()
{
	for (auto iter = m_mapIcon.begin(); iter != m_mapIcon.end(); )
	{
		if (CUi_Icon* pIcon = dynamic_cast<CUi_Icon*>(iter->second))
		{
			pIcon->On_Off(false);

			CTransform* pTransform = dynamic_cast<CTransform*>(pIcon->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
			pTransform->Set_Pos(-99.f, -99.f, -99.f);
		}

		m_mapIconPool.insert({ iter->first, iter->second });
		iter = m_mapIcon.erase(iter);
	}

	m_setIngredient.clear();
}

void CPlate::Set_State(PLATESTATE ePlateState)
{
	m_ePlateState = ePlateState;

	switch (ePlateState)
	{
	case PLATESTATE::CLEAN:
		swprintf_s(m_szMenu, L"Proto_PlateTexture_Plate");
		break;
	case PLATESTATE::PLATED:
		// 음식 조합 텍스처는 Add_Ingredient()에서 처리
		return;
	case PLATESTATE::DIRTY:
		swprintf_s(m_szMenu, L"Proto_PlateTexture_Plate_dirty");
		break;
	}

	Change_Texture(m_szMenu);
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlateTexture_Plate"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlateTexture_Plate_Alpha"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture_Alpha", pComponent });

	return S_OK;
}

_bool CPlate::Add_Ingredient(const _tchar* pTag)
{
	if (!pTag)
		return false;

    auto it = m_setIngredient.insert(pTag);

	if (!it.second)	//	set은 중복 키 값이 들어갈 수 없음
		return false;

	m_ePlateState = PLATED;

	_tchar szMenu[256];
	swprintf_s(szMenu, L"Proto_PlateTexture_Plate");

	for (const auto& ingredient : m_setIngredient)
	{
		lstrcat(szMenu, L"_");
		lstrcat(szMenu, ingredient.c_str());
	}

	lstrcpy(m_szMenu, szMenu);

	if (!Change_Texture(szMenu))
	{
		//MSG_BOX("잘못된 메뉴 조합이다~");	// 잘못된 조합별로 추후 만들 수도 있을 것 같음
		//m_setIngredient.clear();
		swprintf_s(m_szMenu, L"Proto_PlateTexture_Plate_wrong");
		Change_Texture(m_szMenu);

		return true;
	} 

	return true;
}

void CPlate::Ready_IconPool()
{
	if (!m_bIconReady)
	{
		CGameObject* pObj = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object9");
		if (!pObj)
			return;

		CGameObject* pIcon = nullptr;
		for (int i = 0; i < static_cast<int>(CIngredient::ING_END) + 1; ++i)
		{
			CIngredient::INGREDIENT_TYPE eType = static_cast<CIngredient::INGREDIENT_TYPE>(i);

			pIcon = dynamic_cast<CUi_Icon*>(pObj)->Make_Icon(eType);

			if (pIcon)
			{
				CTransform* pTransform = dynamic_cast<CTransform*>(pIcon->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
				pTransform->Set_Pos(-99.f, -99.f, -99.f);

				dynamic_cast<CUi_Icon*>(pIcon)->On_Off(false);
			} 
			m_mapIconPool.insert({ eType, pIcon });
		}

		m_bIconReady = true;
	}
}

void CPlate::Add_Icon(CIngredient::INGREDIENT_TYPE eType)
{
	if (m_bIconReady)
	{
		auto iter = m_mapIconPool.find(eType);
		if (iter == m_mapIconPool.end())
			return;

		if (CUi_Icon* pIcon = dynamic_cast<CUi_Icon*>(iter->second))
		{
			_vec3 vPos{};
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			CTransform* pTransform = dynamic_cast<CTransform*>(pIcon->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
			pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
			pIcon->On_Off(true);
		}
			 
		m_mapIcon.insert({ iter->first, iter->second });
		m_mapIconPool.erase(iter);
	} 
}

void CPlate::Draw_Icon()
{
	if (!m_mapIcon.empty())
	{
		_vec3 vOriginPos;
		m_pTransformCom->Get_Info(INFO_POS, &vOriginPos);

		_vec2 vOffset[3][3] = {
			{},
			{{-0.4f, 0.f}, {0.4f, 0.f}},
			{{-0.4f, 0.4f}, {0.4f, 0.4f}, {-0.4f, -0.4f}},
		};

		auto iter = m_mapIcon.begin();

		for (int i = 0; i < m_mapIcon.size(); ++i, ++iter)
		{
			_vec3 vPos = vOriginPos;
			vPos.x += vOffset[m_mapIcon.size() - 1][i].x;
			vPos.y += vOffset[m_mapIcon.size() - 1][i].y;

			dynamic_cast<CUi_Icon*>(iter->second)->UpdatePosition(vPos);
		}
	}
}

_bool CPlate::Change_Texture(const _tchar* pComponentTag)
{
	Engine::CTexture* pNewTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(pComponentTag));
	if (nullptr == pNewTextureCom)
		return false;

	if (m_vecTextureCom.empty())
		return false;

	auto iter = find_if(m_mapComponent[ID_DYNAMIC].begin(), m_mapComponent[ID_DYNAMIC].end(), CTag_Finder(L"Com_Texture"));
	if (iter != m_mapComponent[ID_DYNAMIC].end())
	{
		Safe_Release(iter->second);
		m_mapComponent[ID_DYNAMIC].erase(iter);
	}
	
	m_vecTextureCom[0] = pNewTextureCom;
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
	m_mapIconPool.clear();
	m_mapIcon.clear(); 
	CInteract::Free();
}