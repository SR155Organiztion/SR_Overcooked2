#include "pch.h"
#include "IState.h"

void IRawState::Enter_State(CIngredient* pIngredient)
{
	pIngredient->Set_Progress(0.f);
	pIngredient->Set_State(CIngredient::RAW);
}

void IRawState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
	CIngredient::INGREDIENT_TYPE eType = pIngredient->Get_Type();

	if (1.f > pIngredient->Get_Progress())
	{
		if (eType == CIngredient::SEAWEED)
			pIngredient->Set_State(CIngredient::DONE);
	}
	else
	{
		if (eType == CIngredient::LETTUCE || eType == CIngredient::TOMATO || eType == CIngredient::CUCUMBER || eType == CIngredient::FISH || eType == CIngredient::SHRIMP)
			pIngredient->ChangeState(new IChopState());
		else if (eType == CIngredient::RICE || eType == CIngredient::PASTA)
			pIngredient->ChangeState(new ICookState());
	} 
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
	CIngredient::INGREDIENT_TYPE eType = pIngredient->Get_Type();

	if (1.f > pIngredient->Get_Progress())
	{
	if (eType == CIngredient::LETTUCE || eType == CIngredient::TOMATO || eType == CIngredient::CUCUMBER || eType == CIngredient::FISH || eType == CIngredient::SHRIMP)
		pIngredient->Set_State(CIngredient::DONE);
	}
	else
	{
		// 토마토스프는 프로그레스 1 되면 ChangeState(new ICook());
	}
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
	if (1.f <= pIngredient->Get_Progress())
		pIngredient->ChangeState(new IDoneState());
}

void ICookState::Exit_State(CIngredient* pIngredient)
{
}

void IDoneState::Enter_State(CIngredient* pIngredient)
{
	pIngredient->Set_Progress(0.f);
	pIngredient->Set_State(CIngredient::DONE);
}

void IDoneState::Update_State(CIngredient* pIngredient, const _float& fTiemDelta)
{
	if (1.f <= pIngredient->Get_Progress())
		pIngredient->Set_State(CIngredient::BURNT);
}

void IDoneState::Exit_State(CIngredient* pIngredient)
{
}