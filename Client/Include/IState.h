/**
* @file		IState.h
* @date		2025-07-01
* @author	권예지
* @brief	재료의 FMS 인터페이스
* @details	재료의 조리 상태별로 RawState, ChopState, CookState 사용.
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