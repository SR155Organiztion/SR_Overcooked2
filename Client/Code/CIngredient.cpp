#include "pch.h"
#include "CIngredient.h"
#include "IState.h"

CIngredient::CIngredient(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev), m_eIngredientType(ING_END), m_eCookState(RAW), m_pCurrentState(nullptr), m_fProgress(0.f), m_bLocked(false)
{
}

CIngredient::CIngredient(const CGameObject& rhs)
	: CInteract(rhs), m_eIngredientType(ING_END), m_eCookState(CS_END), m_pCurrentState(nullptr), m_fProgress(0.f), m_bLocked(false)
{
}

CIngredient::~CIngredient()
{
}

void CIngredient::Set_Progress(const _float& fProgress)
{
	if (Check_Progress())
		m_fProgress = fProgress;
}

void CIngredient::Add_Progress(const _float& fTimeDelta, const _float& fAdd)
{
	if (Check_Progress())
		m_fProgress += fAdd * fTimeDelta;
}

void CIngredient::ChangeState(IState* pNextState)
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Exit_State(this);
		delete m_pCurrentState;
	}

	m_pCurrentState = pNextState;
	if (m_pCurrentState)
		m_pCurrentState->Enter_State(this);
}

bool CIngredient::Check_Progress()
{
	if (Get_Type() == SEAWEED)
		return false;
	else if (Get_Type() == LETTUCE || Get_Type() == TOMATO || Get_Type() == CUCUMBER || Get_Type() == FISH || Get_Type() == SHRIMP)
	{
		if (Get_State() != RAW)
			return false;
	}
	else if (Get_Type() == RICE || Get_Type() == PASTA)
	{
		//if (Get_State() == DONE)
		//	return false;
	}
	// else if (토마토 소스) RAW, CHOP에서만 Progress 진행

	return true;
}

void CIngredient::Free()
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Exit_State(this);
		delete m_pCurrentState;
		m_pCurrentState = nullptr;
	}

	CInteract::Free();
}