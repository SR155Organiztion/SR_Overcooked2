#pragma once
#include "Engine_Define.h"
#include "CBase.h"

class CSelectMapObject
{
public:
	virtual void Set_Enable() { m_bEnable = true; }

protected:
	_bool					m_bEnable = false;
	_bool					m_bRender = false;
	_float					m_bElapsedTime{};
};

