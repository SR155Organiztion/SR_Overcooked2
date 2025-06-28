#pragma once
#include "CUi.h"
#include "CUi_Button.h"
class CUi_Windows :public CUi
{
public:
	explicit CUi_Windows(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUi_Windows(const CGameObject& rhs);
	~CUi_Windows();

protected:
	CUi_Button* m_pButton; 
	LPDIRECT3DTEXTURE9 m_pTexBack; // 백그라운드
	LPDIRECT3DTEXTURE9 m_pTexBox; // 알림창

public:
	void Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Update_GameObject();
	void LateUpdate_GameObject();
	void Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Free();
	 
};

