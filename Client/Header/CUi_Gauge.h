#pragma once
#include "CUi.h"
class CUi_Gauge :  public CUi
{
protected:
    
	DWORD m_dwLimitTime; //제한 시간
	DWORD m_dwTime; //남은 시간
	DWORD m_dwStartTime; //시작 시간
	int m_iseconds;
	int m_iminute;
	RECT* m_pSrcRect; 

	int m_pGauge; //게이지 표시

protected:
	CUi_Gauge(LPDIRECT3DDEVICE9 pZGraphicDev);
	CUi_Gauge(const CGameObject& rhs);
	~CUi_Gauge();

public: 
	HRESULT Add_Component();


};

