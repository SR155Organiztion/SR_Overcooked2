#include "pch.h"
#include "CWashStation.h"
#include "CInteractMgr.h"

CWashStation::CWashStation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CWashStation::CWashStation(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CWashStation::~CWashStation()
{
}

HRESULT CWashStation::Ready_GameObject()
{
	//CInteractMgr::GetInstance()->Add_List(CInteractMgr::STATION, this);
	return S_OK;
}

_int CWashStation::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CWashStation::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CWashStation::Render_GameObject()
{
}

HRESULT CWashStation::Add_Component()
{
	return S_OK;
}

CWashStation* CWashStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CWashStation::Free()
{
	//CInteractMgr::GetInstance()->Remove_List(CInteractMgr::STATION, this);
}

_bool CWashStation::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CWashStation::Set_CarryTypes()
{
}