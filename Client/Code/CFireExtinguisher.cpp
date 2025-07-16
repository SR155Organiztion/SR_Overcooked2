#include "pch.h"
#include "CFireExtinguisher.h"

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
	//if (FAILED(Set_Material()))
	//	return;
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
}