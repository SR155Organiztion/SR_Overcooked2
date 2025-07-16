#include "pch.h"
#include "CIngredient.h"
#include "IState.h"
#include "CManagement.h"
#include "CUi_Icon.h"

CIngredient::CIngredient(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev), m_eIngredientType(ING_END), m_eCookState(RAW), 
	m_pCurrentState(nullptr), m_bLocked(false), m_pIcon(nullptr), m_bIconVisible(false)
{
}

CIngredient::CIngredient(const CGameObject& rhs)
	: CInteract(rhs), m_eIngredientType(ING_END), m_eCookState(CS_END), 
	m_pCurrentState(nullptr), m_bLocked(false), m_pIcon(nullptr), m_bIconVisible(false)
{
}

CIngredient::~CIngredient()
{
}

void CIngredient::Init()
{
	ChangeState(new IRawState());
}

void CIngredient::Reset()
{
	if (m_pIcon)
	{
		m_bIconVisible = false;
		dynamic_cast<CUi_Icon*>(m_pIcon)->On_Off(m_bIconVisible);
	}
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

void CIngredient::Draw_Icon()
{
	if (!m_pIcon)
	{
		CGameObject* pObj = CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object9");
		if (!pObj)
			return;

		m_pIcon = dynamic_cast<CUi_Icon*>(pObj)->Make_Icon(m_eIngredientType);
	}
	else
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		dynamic_cast<CUi_Icon*>(m_pIcon)->UpdatePosition(vPos);
		dynamic_cast<CUi_Icon*>(m_pIcon)->On_Off(m_bIconVisible);
	}
}

void CIngredient::Free()
{
	if (m_szSelfId) std::free((void*)m_szSelfId); //selfId 만들때 버퍼 할당해서 해제하는 작업

	if (m_pCurrentState)
	{
		m_pCurrentState->Exit_State(this);
		delete m_pCurrentState;
		m_pCurrentState = nullptr;
	}

	CInteract::Free();
}