#pragma once
#include "Engine_Define.h"
#include "ICarry.h"
#include <unordered_set>

class IPlace
{
public:
	virtual _bool Get_CanPlace(ICarry* pCarry) const = 0;
	void Set_Full() {}
	void Set_Empty() {}
	virtual ~IPlace() {}

protected:
	virtual void Set_CarryTypes() = 0;

protected:
	_bool m_bFull = false;
	unordered_set<ICarry::CARRYTYPE> m_setCarryTypes;
};