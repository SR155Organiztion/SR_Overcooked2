#pragma once
#include "CUi.h"
class CUi_Gauge :  public CUi
{
private:
    float m_percent;
    bool m_bActivate; //환경 OBJ와 상호작용을 위한 변수
	//(ResourceMgr*)m_pResourceSize = 100; 전체 리소스의 size를 담는다.
	//(ResourceMgr*)m_pResource = 0; 지금까지 읽은 파일 수를 담는다.
	LPDIRECT3DTEXTURE9 m_pTexBar; //게이지 전체 길이만큼의 바
	LPDIRECT3DTEXTURE9 m_pTexGuage; //게이지를 표시

	float barWidth; //전체 바의 폭
	float barHeight; //전체 바의 높이
	D3DXVECTOR3 pos;  //바 위치

protected:
	CUi_Gauge(LPDIRECT3DDEVICE9 pZGraphicDev);
	CUi_Gauge(const CGameObject& rhs);
	~CUi_Gauge();

public:
	void SetPercent(float p) { m_percent = p; }


};

