#pragma once
#include "CGameObject.h"

class CNPC : public Engine::CGameObject
{
protected:
	explicit CNPC(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNPC();
};

