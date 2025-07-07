#include "pch.h"
#include "CServingStation.h"
#include "CInteractMgr.h"
#include "CPlate.h"

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
	m_pTransformCom->Set_Scale({ 1.f, 0.5f, 1.f });
	//m_pTransformCom->Set_Pos(10.f, m_pTransformCom->Get_Scale().y, 10.f);

	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	//CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);
	return S_OK;
}

_int CServingStation::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CServingStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CServingStation::Render_GameObject()
{
	//if (FAILED(Set_Material()))
	//	return;
}

HRESULT CServingStation::Add_Component()
{
	return S_OK;
}

CServingStation* CServingStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	return nullptr;
}

void CServingStation::Free()
{
	//CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
}

_bool CServingStation::Get_CanPlace(CGameObject* pItem)
{
	//CInteract* pInteract = dynamic_cast<CInteract*>(pItem);
	//
	//if (nullptr == pInteract)
	//	return false;
	//
	//if (CInteract::PLATE == pInteract->Get_InteractType())
	//	return true;

	return false;
}