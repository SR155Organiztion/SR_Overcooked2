/**
* @file		IState.h
* @date		2025-07-01
* @author	�ǿ���
* @brief	����� FMS �������̽�
* @details	����� ���� ���º��� RawState, ChopState, CookState ���.
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
	// IState��(��) ���� ��ӵ�
	void Enter_State(CIngredient* pIngredient) override;
	void Update_State(CIngredient* pIngredient, const _float& fTiemDelta) override;
	void Exit_State(CIngredient* pIngredient) override;
	virtual ~IRawState() = default;
};

class IChopState : public IState {
	// IState��(��) ���� ��ӵ�
	void Enter_State(CIngredient* pIngredient) override;
	void Update_State(CIngredient* pIngredient, const _float& fTiemDelta) override;
	void Exit_State(CIngredient* pIngredient) override;
	virtual ~IChopState() = default;
};

class ICookState : public IState {
	// IState��(��) ���� ��ӵ�
	void Enter_State(CIngredient* pIngredient) override;
	void Update_State(CIngredient* pIngredient, const _float& fTiemDelta) override;
	void Exit_State(CIngredient* pIngredient) override;
	virtual ~ICookState() = default;
};