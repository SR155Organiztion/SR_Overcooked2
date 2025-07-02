#pragma once
#include "CGameObject.h"

class CState
{
public:
	virtual	void		Enter_State(CGameObject* Owner) = 0;
	virtual	void		Update_State(CGameObject* Owner, const _float& fTimeDelta) = 0;
	virtual	void		TestForExit_State(CGameObject* Owner) = 0;
};


