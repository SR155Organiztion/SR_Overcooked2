#pragma once
#include "CGameObject.h"

class CInteract : public CGameObject
{
protected:
	explicit CInteract(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteract(const CGameObject& rhs);
	virtual ~CInteract();

protected:
	virtual		void		Free();
};