#include "pch.h"
#include "CIngredient.h"
#include "IState.h"

CIngredient::CIngredient(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev), m_eType(ING_END), m_eCookState(CS_END), m_pCurrentState(nullptr), m_fProgress(0.f)
{
}

CIngredient::CIngredient(const CGameObject& rhs)
	: CInteract(rhs)
{
}

CIngredient::~CIngredient()
{
}

_bool CIngredient::Is_FinalStep() const
{
	return false;
}

void CIngredient::Set_Done()
{
	m_eCookState = DONE;
}

void CIngredient::Set_Progress(const _float& fProgress)
{	
	m_fProgress = fProgress;

	if (1.f <= fProgress)
		m_fProgress = 1.f;
}

void CIngredient::Add_Progress(const _float& fTimeDelta, const _float& fAdd)
{
	m_fProgress += fAdd * fTimeDelta;

	if (1.f <=m_fProgress)
		m_fProgress = 1.f;
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

void CIngredient::Free()
{
	Engine::CGameObject::Free();
}

_bool CIngredient::Get_CanCarry() const
{
	return true;
}
