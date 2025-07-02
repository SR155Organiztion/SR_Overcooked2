#include "pch.h"
#include "CDishStation.h"
#include "CInteractMgr.h"

CDishStation::CDishStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CDishStation::CDishStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CDishStation::~CDishStation()
{
}

HRESULT CDishStation::Ready_GameObject()
{
	//CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);
	return S_OK;
}

_int CDishStation::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CDishStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CDishStation::Render_GameObject()
{
}

HRESULT CDishStation::Add_Component()
{
	return S_OK;
}

CDishStation* CDishStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CDishStation::Free()
{
	//CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
}

_bool CDishStation::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CDishStation::Set_CarryTypes()
{
}