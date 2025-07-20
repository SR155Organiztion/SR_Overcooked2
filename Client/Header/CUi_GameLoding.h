#pragma once
#include "CUi_Gauge.h"
class CUi_GameLoding :  public CUi_Gauge
{

private:
	CUi_GameLoding(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_GameLoding(const CGameObject& rhs);
	~CUi_GameLoding();

public:
	void Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev, float m_pPercent);
	void Update_GameObject();
	void LateUpdate_GameObject();
	void Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	HRESULT Add_Component();

private:
	void Free();
};

