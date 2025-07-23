#pragma once
#include "CUi.h"
class CUi_Fadeout :public CUi
{
	private:
		Engine::CSprite* m_pSpriteCom;
		Engine::CSprite* m_pSpriteCom2;
		Engine::CSprite* m_pSpriteCom3; 
		Engine::CSprite* m_pSpriteCom4;
		LPD3DXSPRITE m_pSprite; 
		list<UIDATA> m_listData;
		std::vector<LPDIRECT3DTEXTURE9> m_vecFadeoutTex; ///애니메이션
		
		int m_iFrame = 0;
		float m_fFrameTime = 0.f;
		float m_fFrameDelay = 0.08f;

		bool m_bFadeComplete;

	public:
		CUi_Fadeout();
		CUi_Fadeout(LPDIRECT3DDEVICE9 _pGraphicDev);
		CUi_Fadeout(const CGameObject& _rhs);
		~CUi_Fadeout();

	public:
		HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
		int Update_GameObject(const _float& _fTimeDelta);
		void LateUpdate_GameObject();
		void Render_GameObject();
		HRESULT Add_Component();
		void Make_Fadeout(int _Number);
		bool Get_FadeComplete() { return m_bFadeComplete; }

	private:
		void Free();

};

