#include "pch.h"
#include "CIngredient.h"
#include "IState.h"
#include <IPlace.h>

CIngredient::CIngredient(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteract(pGraphicDev), m_eIngredientType(ING_END), m_eCookState(RAW), m_pCurrentState(nullptr), m_bLocked(false)
{
}

CIngredient::CIngredient(const CGameObject& rhs)
	: CInteract(rhs), m_eIngredientType(ING_END), m_eCookState(CS_END), m_pCurrentState(nullptr), m_bLocked(false)
{
}

CIngredient::~CIngredient()
{
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

void CIngredient::On_Collision(CGameObject* _pGameObject)
{
	if (!_pGameObject)
		return;

	INTERACTTYPE eID = dynamic_cast<CInteract*>(_pGameObject)->Get_InteractType();
	switch (eID) {
	case CHOPSTATION:	///< 도마 스테이션
	case SINKSTATION:	///< 싱크 스테이션 (접시 세척)
	case EMPTYSTATION:
	case STATION:
		dynamic_cast<IPlace*>(_pGameObject)->Set_Place(this, _pGameObject);
		break;
	}
}

void CIngredient::On_Snap(CGameObject* _pGameObject)
{
	if (!_pGameObject)
		return;

	INTERACTTYPE eID = dynamic_cast<CInteract*>(_pGameObject)->Get_InteractType();
	switch (eID) {
	case CHOPSTATION:	///< 도마 스테이션
	case SINKSTATION:	///< 싱크 스테이션 (접시 세척)
	case EMPTYSTATION:
	case STATION:
		dynamic_cast<IPlace*>(_pGameObject)->Set_Place(this, _pGameObject);
		break;
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