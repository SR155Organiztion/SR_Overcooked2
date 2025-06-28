#pragma once
#include "CUi_Gauge.h"

class CUi_CookLoding : public CUi_Gauge
{

private:
	CUi_CookLoding(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_CookLoding(const CGameObject& rhs);
	~CUi_CookLoding();

public:
	void Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev, float m_pPercent);
	void Update_GameObject();
	void LateUpdate_GameObject();
	void Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);

private:
	void Free();
};

