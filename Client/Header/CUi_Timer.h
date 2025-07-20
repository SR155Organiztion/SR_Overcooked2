#pragma once
#include "CUi_Gauge.h"
class CUi_Timer : public CUi_Gauge
{
private:

	Engine::CSprite* m_pSpriteCom; //게이지
	Engine::CSprite* m_pSpriteCom2; //이미지

	LPD3DXFONT m_pFont;
	LPD3DXSPRITE m_pSprite;

public:
	CUi_Timer();
	CUi_Timer(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Timer(const CGameObject& rhs);
	~CUi_Timer();

public: 
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, GAUGE_TYPE _type);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	HRESULT Add_Component();
	void Set_Timer(DWORD _dwLimitTime);

protected:
	void Free();
};

