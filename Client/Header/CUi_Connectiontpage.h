#pragma once
#include "CUi.h"
class CUi_Connectiontpage : public CUi
{
private:
	Engine::CSprite* m_pSpriteCom;
	LPD3DXSPRITE m_pSprite;
	list<UIDATA> m_listData;
	std::vector<LPDIRECT3DTEXTURE9> m_vecFadeoutTex; ///애니메이션

	int m_iFrame = 0;
	float m_fFrameTime = 0.f;
	float m_fFrameDelay = 0.08f;

	bool m_bLoadingComplete;

public:
	CUi_Connectiontpage();
	CUi_Connectiontpage(LPDIRECT3DDEVICE9 _pGraphicDev);
	CUi_Connectiontpage(const CUi_Connectiontpage& _rhs);
	~CUi_Connectiontpage();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	HRESULT Add_Component();
	CUi_Connectiontpage* Make_Connectiontpage();
	bool Get_FadeComplete() { return m_bLoadingComplete; }

private:
	void Free();
};

