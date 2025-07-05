#include "pch.h"
#include "CSinkStation.h"
#include "CInteractMgr.h"

CSinkStation::CSinkStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CSinkStation::CSinkStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CSinkStation::~CSinkStation()
{
}

HRESULT CSinkStation::Ready_GameObject()
{
	m_pTransformCom->Set_Pos(10.f, m_pTransformCom->Get_Scale().y, 10.f);
	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.eBoundingType = BOX;
	m_stOpt.stCollisionOpt = AABB;

	//CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);
	return S_OK;
}

_int CSinkStation::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CSinkStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CSinkStation::Render_GameObject()
{
}

HRESULT CSinkStation::Add_Component()
{
	return S_OK;
}

CSinkStation* CSinkStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	//CSinkStation* pSinkStation = new CSinkStation(pGraphicDev);
	//
	//if (FAILED(pSinkStation->Ready_GameObject()))
	//{
	//	Safe_Release(pSinkStation);
	//	MSG_BOX("Station_Sink Failed");
	//	return nullptr;
	//}
	//
	//return pSinkStation;

	return nullptr;
}

void CSinkStation::Free()
{
	//CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
	//Engine::CGameObject::Free();
}

_bool CSinkStation::Get_CanPlace(CGameObject* pItem)
{
	// 오염된 접시만
	return false;
}