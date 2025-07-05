#include "pch.h"
#include "CFireExtinguisher.h"
#include "CInteractMgr.h"

CFireExtinguisher::CFireExtinguisher(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CFireExtinguisher::CFireExtinguisher(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CFireExtinguisher::~CFireExtinguisher()
{
}

HRESULT CFireExtinguisher::Ready_GameObject()
{
	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	//CInteractMgr::GetInstance()->Add_List(CInteractMgr::TOOL, this);

	return S_OK;
}

_int CFireExtinguisher::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CFireExtinguisher::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CFireExtinguisher::Render_GameObject()
{
}

HRESULT CFireExtinguisher::Add_Component()
{
	return S_OK;
}

CFireExtinguisher* CFireExtinguisher::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	return nullptr;
}

void CFireExtinguisher::Free()
{
	//CInteractMgr::GetInstance()->Remove_List(CInteractMgr::TOOL, this);
}

_bool CFireExtinguisher::Get_CanCarry() const
{
	return false;
}