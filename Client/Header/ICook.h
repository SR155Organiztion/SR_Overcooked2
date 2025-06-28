#pragma once
#include "Engine_Define.h"
class CIngredient;

class ICook
{
public:
	virtual _bool CanCook(CIngredient* pIngredient) const = 0;
	virtual void Cook(CIngredient* pIngredient) = 0;
};