/**
* @file		IState.h
* @date		2025-07-01
* @author	권예지
* @brief	재료의 상태를 관리하는 상태 패턴(FSM) 인터페이스 모음
* @details
*			- IState : 재료 상태의 공통 인터페이스 (Enter, Update, Exit 메서드)
*			- IRawState : 재료가 생(날 것) 상태일 때의 동작 정의
*			- IChopState : 재료가 썰리는 중 상태일 때의 동작 정의
*			- ICookState : 재료가 가열 조리 중 상태일 때의 동작 정의
*			- IDoneState : 조리가 완료되어 더 이상 변화가 없는 상태를 정의
* 
*			각 상태는 CIngredient 객체를 받아 해당 상태에 맞는 처리를 담당하며,
*			Enter_State에서 상태 진입 시 초기화,
*			Update_State에서 상태별 지속 동작,
*			Exit_State에서 상태 종료 시 정리 작업을 수행한다.
*/
#pragma once
#include "Engine_Define.h"
#include "CIngredient.h"

class IState
{
public:
	virtual void Enter_State(CIngredient* pIngredient) = 0;
	virtual void Update_State(CIngredient* pIngredient, const _float& fTiemDelta) = 0;
	virtual void Exit_State(CIngredient* pIngredient) = 0;
	virtual ~IState() = default;
};

class IRawState : public IState {
	// IState을(를) 통해 상속됨
	void Enter_State(CIngredient* pIngredient) override;
	void Update_State(CIngredient* pIngredient, const _float& fTiemDelta) override;
	void Exit_State(CIngredient* pIngredient) override;
	virtual ~IRawState() = default;
};

class IChopState : public IState {
	// IState을(를) 통해 상속됨
	void Enter_State(CIngredient* pIngredient) override;
	void Update_State(CIngredient* pIngredient, const _float& fTiemDelta) override;
	void Exit_State(CIngredient* pIngredient) override;
	virtual ~IChopState() = default;
};

class ICookState : public IState {
	// IState을(를) 통해 상속됨
	void Enter_State(CIngredient* pIngredient) override;
	void Update_State(CIngredient* pIngredient, const _float& fTiemDelta) override;
	void Exit_State(CIngredient* pIngredient) override;
	virtual ~ICookState() = default;
};

class IDoneState : public IState {
	// IState을(를) 통해 상속됨
	void Enter_State(CIngredient* pIngredient) override;
	void Update_State(CIngredient* pIngredient, const _float& fTiemDelta) override;
	void Exit_State(CIngredient* pIngredient) override;
	virtual ~IDoneState() = default;
};