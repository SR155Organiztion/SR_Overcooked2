#pragma once
#include "CUi_Gauge.h"
class CUi_Score :public CUi_Gauge
{

private:

	Engine::CSprite* m_pSpriteCom3; // Score
	Engine::CSprite* m_pSpriteCom4; // Coin

	LPD3DXFONT m_pFont;
	LPD3DXSPRITE m_pSprite;
	int m_iScore; // 갱신되는 점수
	int m_iPrevScore; //이전에 갱신된 점수

	std::vector<LPDIRECT3DTEXTURE9> m_vecCoinTex;
	int m_iCoinFrame = 0;
	float m_fFrameTime = 0.f;
	float m_fFrameDelay = 0.08f; 

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
	HRESULT Add_Component();
	
private:
	void Free();
};

