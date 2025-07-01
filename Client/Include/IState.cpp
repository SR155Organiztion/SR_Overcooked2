#include "pch.h"
#include "IState.h"

void IRawState::Enter_State(CIngredient* pIngredient)
{
	pIngredient->Set_Progress(0.f);
	pIngredient->Set_State(CIngredient::RAW);
}

void IRawState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
	if (1.f > pIngredient->Get_Progress())
		return;

	CIngredient::INGREDIENT_TYPE eType = pIngredient->Get_Type();
	if (eType == CIngredient::LETTUCE || eType == CIngredient::TOMATO || eType == CIngredient::CUCUMBER || eType == CIngredient::FISH || eType == CIngredient::SHRIMP)
		pIngredient->ChangeState(new IChopState());
}

void IRawState::Exit_State(CIngredient* pIngredient)
{
}

void IChopState::Enter_State(CIngredient* pIngredient)
{
	pIngredient->Set_Progress(0.f);
	pIngredient->Set_State(CIngredient::CHOPPED);
}

void IChopState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
	if (1.f > pIngredient->Get_Progress())
		return;

	CIngredient::INGREDIENT_TYPE eType = pIngredient->Get_Type();
	if (eType == CIngredient::LETTUCE)
		pIngredient->Set_Done();
}

void IChopState::Exit_State(CIngredient* pIngredient)
{
}

void ICookState::Enter_State(CIngredient* pIngredient)
{
	pIngredient->Set_Progress(0.f);
	pIngredient->Set_State(CIngredient::COOKED);
}

void ICookState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
}

void ICookState::Exit_State(CIngredient* pIngredient)
{
}
