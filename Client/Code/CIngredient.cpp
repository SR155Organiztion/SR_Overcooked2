#include "pch.h"
#include "CIngredient.h"

CIngredient::CIngredient(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev)
{
}

CIngredient::CIngredient(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CIngredient::~CIngredient()
{
}

CIngredient::COOKSTATE CIngredient::Get_State() const
{
	return COOKSTATE();
}

void CIngredient::Set_State(COOKSTATE eState)
{
}

_bool CIngredient::Is_FinalStep() const
{
	return false;
}

void CIngredient::Set_Done()
{
}

_float CIngredient::Get_Progress() const
{
	return _float();
}

void CIngredient::Add_Progress(const _float& fAdd)
{
}

void CIngredient::NextState()
{
}

void CIngredient::Free()
{
}

_bool CIngredient::Get_CanCarry() const
{
	return _bool();
}
