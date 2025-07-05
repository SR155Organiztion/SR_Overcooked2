#include "pch.h"
#include "CPlate.h"
#include "CInteractMgr.h"
#include "CIngredient.h"

CPlate::CPlate(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CPlate::CPlate(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CPlate::~CPlate()
{
}

HRESULT CPlate::Ready_GameObject()
{
	m_stOpt.bApplyGravity = true;
	m_stOpt.bApplyRolling = false;
	m_stOpt.bApplyBouncing = false;
	m_stOpt.bApplyKnockBack = true;

	//CInteractMgr::GetInstance()->Add_List(CInteractMgr::TOOL, this);

	return S_OK;
}

_int CPlate::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CPlate::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CPlate::Render_GameObject()
{
}

HRESULT CPlate::Add_Component()
{
	return S_OK;
}

CPlate* CPlate::Create(LPDIRECT3DDEVICE9 pGraphicDev) 
{
	return nullptr;
}

void CPlate::Free()
{
	//CInteractMgr::GetInstance()->Remove_List(CInteractMgr::TOOL, this);
}

_bool CPlate::Get_CanPlace(CGameObject* pItem)
{
	// 재료 (완성된 상태)
	CInteract* pInteract = dynamic_cast<CInteract*>(pItem);

	if (nullptr == pInteract)
		return false;

	if (CInteract::INGREDIENT == pInteract->Get_InteractType())
	{
		CIngredient* pIngredient = dynamic_cast<CIngredient*>(pInteract);
		if (nullptr == pIngredient)
			return false;

		if (CIngredient::DONE == pIngredient->Get_State())
			return true;
	}

	return false;
}

_bool CPlate::CanPlate(CIngredient* pIngredient) const
{
	return _bool();
}

void CPlate::Plate(CIngredient* pIngredient)
{
}

_bool CPlate::Get_CanCarry() const
{
	return _bool();
}