#include "pch.h"
#include "IState.h"

void IRawState::Enter_State(CIngredient* pIngredient)
{
	//pIngredient->Set_State(CIngredient::RAW);
}

void IRawState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
	CIngredient::INGREDIENT_TYPE eType = pIngredient->Get_IngredientType();

	if (CIngredient::SEAWEED == eType)
		pIngredient->ChangeState(new IDoneState());
}

void IRawState::Exit_State(CIngredient* pIngredient)
{
}

void IChopState::Enter_State(CIngredient* pIngredient)
{
	pIngredient->Set_State(CIngredient::CHOPPED);
}

void IChopState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
	CIngredient::INGREDIENT_TYPE eType = pIngredient->Get_IngredientType();

	if (CIngredient::TOMATO == eType || CIngredient::LETTUCE == eType || CIngredient::CUCUMBER == eType || CIngredient::FISH == eType || CIngredient::SHRIMP == eType)
		pIngredient->ChangeState(new IDoneState());
}

void IChopState::Exit_State(CIngredient* pIngredient)
{
}

void ICookState::Enter_State(CIngredient* pIngredient)
{
	pIngredient->Set_State(CIngredient::COOKED);
}

void ICookState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
}

void ICookState::Exit_State(CIngredient* pIngredient)
{
}

void IDoneState::Enter_State(CIngredient* pIngredient)
{
	pIngredient->Set_State(CIngredient::DONE);
}

void IDoneState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
}

void IDoneState::Exit_State(CIngredient* pIngredient)
{
}

void IBurntState::Enter_State(CIngredient* pIngredient)
{
	pIngredient->Set_State(CIngredient::BURNT);
}

void IBurntState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
}

void IBurntState::Exit_State(CIngredient* pIngredient)
{
}