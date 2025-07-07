#include "pch.h"
#include "CTrashStation.h"
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

_int CTrashStation::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CTrashStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CTrashStation::Render_GameObject()
{
	//if (FAILED(Set_Material()))
	//	return;
}

HRESULT CTrashStation::Add_Component()
{
	return S_OK;
}

CTrashStation* CTrashStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	return nullptr;
}

void CTrashStation::Free()
{
	//CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
}

_bool CTrashStation::Get_CanPlace(CGameObject* pItem)
{
	// 식재료 폐기
	return false;
}