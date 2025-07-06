#pragma once
#include "CUi_Gauge.h"
class CUi_Scoer :public CUi_Gauge
{

private:
	LPD3DXFONT m_pFont;
	LPD3DXSPRITE m_pSprite;

public:
	CUi_Scoer();
	CUi_Scoer(LPDIRECT3DDEVICE9 _pGraphicDev);
	CUi_Scoer(const CGameObject& _rhs);
	~CUi_Scoer();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, GAUGE_TYPE _type);
	void Update_GameObject();
	void LateUpdate_GameObject();
	virtual void Render_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	
private:
	void Free();
};

