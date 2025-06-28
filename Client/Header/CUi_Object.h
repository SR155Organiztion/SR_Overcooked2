#pragma once
#include "CUi.h"
class CUi_Object :  public CUi
{
private:
	map<_tchar*, LPDIRECT3DTEXTURE9> m_mapObject;

protected:
	CUi_Object(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Object(const CGameObject& rhs);
	~CUi_Object();

public:
	
};

