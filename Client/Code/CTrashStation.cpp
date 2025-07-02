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