#include "pch.h"
#include "CTrashStation.h"

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

CTrashStation* CTrashStation::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
	return nullptr;
}

void CTrashStation::Free()
{
}

_bool CTrashStation::Get_CanPlace(ICarry* pCarry) const
{
	return _bool();
}

void CTrashStation::Set_CarryTypes()
{
}