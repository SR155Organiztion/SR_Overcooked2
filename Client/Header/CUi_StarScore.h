#pragma once
#include "CUi.h"
#include "CUi_Button.h"
#include "CSprite.h"

class CUi_StarScore :public CUi
{
private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CSprite* m_pSpriteCom2;
	int m_iNumber;

public:
	explicit CUi_StarScore(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUi_StarScore(const CGameObject& rhs);
	~CUi_StarScore();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	HRESULT Add_Component();
	void Set_StarScore(int _Number) { m_iNumber = _Number; }
	void Free();
	 
};

