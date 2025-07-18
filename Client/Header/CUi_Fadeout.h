#pragma once
#include "CUi.h"
class CUi_Fadeout :public CUi
{
	private:
		Engine::CSprite* m_pSpriteCom;
		Engine::CSprite* m_pSpriteCom2;
		Engine::CSprite* m_pSpriteCom3; 
		LPD3DXSPRITE m_pSprite; 
		
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

	private:
		void Free();

};

