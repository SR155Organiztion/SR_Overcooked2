#include "pch.h"
#include "CServingStation.h"
#include "CInteractMgr.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CPlate.h" 
#include "CObjectPoolMgr.h"
#include "CManagement.h"

CServingStation::CServingStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CServingStation::CServingStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CServingStation::~CServingStation()
{
}

HRESULT CServingStation::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });
	m_pTransformCom->Set_Pos(8.5f, m_pTransformCom->Get_Scale().y * 0.5f, 6.5f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);		// 삭제 예정

	return S_OK;
}

_int CServingStation::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CServingStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CServingStation::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	int iIndex(0);
	if (IProcess* pProcess = dynamic_cast<IProcess*>(Get_Item()))
		if (pProcess->Get_Process())
			iIndex = 1;
	m_pTextureCom->Set_Texture(iIndex);

	if (FAILED(Set_Material()))
		return;

	m_pBufferCom->Render_Buffer();
}

_bool CServingStation::Set_Place(CGameObject* pItem, CGameObject* pPlace)
{
	// nullptr 검사
	if (!pItem || !pPlace)
		return false;

	if (!Get_CanPlace(pItem))
		return false;

	// TODO : 접시에 담긴 재료 정보로 ~

	// 접시를 오브젝트 풀에 반환
	CObjectPoolMgr::GetInstance()->Return_Object(pItem->Get_SelfId(), pItem);
	CManagement::GetInstance()->Delete_GameObject(L"GameObject_Layer", pItem->Get_SelfId());

	return true;
}

_bool CServingStation::Get_CanPlace(CGameObject* pItem)
{
	// 접시만
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	if (!pInteract)
		return false;

	if (CInteract::PLATE == pInteract->Get_InteractType())
		return true;

	return false;
}

HRESULT CServingStation::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationBoxTexture_Serving"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CServingStation* CServingStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	CServingStation* pServingStation = new CServingStation(pGraphicDev);

	if (FAILED(pServingStation->Ready_GameObject()))
	{
		Safe_Release(pServingStation);
		MSG_BOX("Station_Serving Failed");
		return nullptr;
	}

	return pServingStation;
}

void CServingStation::Free()
{
	CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);		// 삭제 예정
	Engine::CGameObject::Free();
}