#pragma once
#include "CUi.h"
class CUi_Gauge :  public CUi
{
protected:
    
	DWORD m_dwLimitTime; //���� �ð�
	DWORD m_dwTime; //���� �ð�
	DWORD m_dwStartTime; //���� �ð�
	int m_iseconds;
	int m_iminute;
	RECT* m_pSrcRect; 

	int m_pGauge; //������ ǥ��

protected:
	CUi_Gauge(LPDIRECT3DDEVICE9 pZGraphicDev);
	CUi_Gauge(const CGameObject& rhs);
	~CUi_Gauge();

public: 
	HRESULT Add_Component();


};

