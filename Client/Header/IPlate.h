#pragma once
#include "Engine_Define.h"
class CIngredient;

class IPlate
{
public:
	virtual _bool CanPlate(CIngredient* pIngredient) const = 0;
	virtual void Plate(CIngredient* pIngredient) = 0;
};