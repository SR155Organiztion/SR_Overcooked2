#pragma once
#include "CUi_Gauge.h"
class CUi_Score :public CUi_Gauge
{

private:
	LPD3DXFONT m_pFont;
	LPD3DXSPRITE m_pSprite;
	int m_iScore;

public:
	CUi_Score();
	CUi_Score(LPDIRECT3DDEVICE9 _pGraphicDev);
	CUi_Score(const CGameObject& _rhs);
	~CUi_Score();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, GAUGE_TYPE _type);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	
private:
	void Free();
};

