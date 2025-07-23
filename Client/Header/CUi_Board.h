#pragma once
#include "CUi.h"
class CUi_Board :   public CUi
{
private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CSprite* m_pSpriteCom2;
	Engine::CSprite* m_pSpriteCom3;

	RECT m_SrcRect;
	/*static bool m_bRemoved;*/

	int m_iseconds;
	int m_iminute;
	int m_pGauge;

public:
	CUi_Board();
	CUi_Board(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Board(const CUi_Board& rhs);
	~CUi_Board();

	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	HRESULT Add_Component();
	void Make_Board(bool _bVisible, int _Number);

	void On_Off(bool _bProcess)
	{
		m_tData.m_bProcess = _bProcess;
	}


private:
	void Free();

};

