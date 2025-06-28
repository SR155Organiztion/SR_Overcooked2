#pragma once
#include "Engine_Define.h"
class CIngredient;

class IChop 
{
public:
	virtual _bool CanChop(CIngredient* pIngredient) const = 0;
	virtual void Chop(CIngredient* pIngredient) = 0;
};