#pragma once
#include "CGameObject.h"
class CUi_Factory :public CGameObject
{
public:
	explicit CUi_Factory(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUi_Factory(const CGameObject& rhs);
	~CUi_Factory();

public:
	void CUi_Creat(_tchar* _m_szName);
	void Button_Creat(_tchar* _m_szName);
	void Steage_creat(_tchar* _m_szName);
	void Order_Creat(_tchar* _m_szName);
	void Gauge_Creat(_tchar* _m_szName);
	void GameEnd_Creat(_tchar* _m_szName);

	void Free();
};

