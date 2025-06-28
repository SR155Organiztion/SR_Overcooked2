#pragma once
#include "Engine_Define.h"

class ICarry
{
public:
	enum CARRYTYPE {
		SEAWEED, LETTUCE, TOMATO, CUCUMBER, FISH, SHRIMP, MEAT, MUSHROOM, RICE, PASTA,
		FRYINGPAN, POT, PLATE,
		FIREEXTINGUISHER, CTEND
	};

public:
	virtual CARRYTYPE Get_CarryType() const { return m_eType; }
	virtual _bool Get_CanCarry() const = 0;

protected:
	virtual void Set_CarryType(CARRYTYPE eType) { m_eType = eType; }

protected:
	CARRYTYPE m_eType;
};