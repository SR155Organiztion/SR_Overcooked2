#include "pch.h"
#include "CTrashStation.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CObjectPoolMgr.h"
#include "CPlate.h"
#include "CInteractMgr.h"

CTrashStation::CTrashStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CTrashStation::CTrashStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CTrashStation::~CTrashStation()
{
}

HRESULT CTrashStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });
	m_pTransformCom->Set_Pos(12.5f, m_pTransformCom->Get_Scale().y * 0.5f, 6.5f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bIsStation = true;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);		// 삭제 예정

	return S_OK;
}

_int CTrashStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CTrashStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Engine::CGameObject::Compute_ViewZ(&vPos);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTrashStation::Render_GameObject()
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
}

_bool CTrashStation::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	// nullptr 검사
	if (!pItem || !pPlace)
		return false;

	if (!Get_CanPlace(pItem))
		return false;

	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (!pInteract)
		return false;

	CInteract::INTERACTTYPE eInteractType = pInteract->Get_InteractType();

	if (CInteract::INGREDIENT == eInteractType)
	{	
		// 재료일 경우 ObjectPool에 반환
		CObjectPoolMgr::GetInstance()->Return_Object(pItem->Get_BaseId().c_str(), pItem);
		CManagement::GetInstance()->Delete_GameObject(L"GameObject_Layer", pItem->Get_SelfId(), pItem);
		return true;
	}
	else if (CInteract::PLATE == eInteractType)
	{
		// 접시일 경우 안에 있는 내용물 비우기
		dynamic_cast<CPlate*>(pItem)->Set_Clean();
		return false;
	}
	else if (CInteract::POT == eInteractType || CInteract::FRYINGPAN == eInteractType)
	{
		if (IPlace* pPlace = dynamic_cast<IPlace*>(pInteract))
			pPlace->Set_Empty();
		return false;
	}

	return false;
}

_bool CTrashStation::Get_CanPlace(CGameObject* pItem)
{ 
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (!pInteract)
		return false;

	// 식재료 또는
	if (CInteract::INGREDIENT == pInteract->Get_InteractType() ||
		CInteract::PLATE == pInteract->Get_InteractType() ||
		CInteract::POT == pInteract->Get_InteractType() || 
		CInteract::FRYINGPAN == pInteract->Get_InteractType())
		return true;

	return false;
}

_bool CTrashStation::On_Snap(CGameObject* _pGameObject)
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

HRESULT CTrashStation::Add_Component()
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

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Trash"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	pComponent = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Alpha"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_vecTextureCom.push_back(dynamic_cast<CTexture*>(pComponent));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture_Alpha", pComponent });

	return S_OK;
}

CTrashStation* CTrashStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CTrashStation* pTrashStation = new CTrashStation(pGraphicDev);

	if (FAILED(pTrashStation->Ready_GameObject()))
	{
		Safe_Release(pTrashStation);
		MSG_BOX("Station_Trash Failed");
		return nullptr;
	}

	return pTrashStation;
}

void CTrashStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);		// 삭제 예정
	Engine::CGameObject::Free();
}