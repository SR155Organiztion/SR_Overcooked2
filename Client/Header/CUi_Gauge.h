#pragma once
#include "CUi.h"
class CUi_Gauge :  public CUi
{
protected:
    float m_percent;
    bool m_bActivate; //환경 OBJ와 상호작용을 위한 변수
	//(ResourceMgr*)m_pResourceSize = 100; 전체 리소스의 size를 담는다.
	//(ResourceMgr*)m_pResource = 0; 지금까지 읽은 파일 수를 담는다.
	LPDIRECT3DTEXTURE9 m_pTexBar; //게이지 전체 길이만큼의 바
	LPDIRECT3DTEXTURE9 m_pTexGuage; //게이지를 표시

	int m_pGauge; //게이지 표시

protected:
	CUi_Gauge(LPDIRECT3DDEVICE9 pZGraphicDev);
	CUi_Gauge(const CGameObject& rhs);
	~CUi_Gauge();

public:
	void SetPercent(float p) { m_percent = p; }


};

