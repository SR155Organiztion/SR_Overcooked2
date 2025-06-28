#pragma once
#include "CUi_Windows.h"

class CUi_MainMenu :  public CUi_Windows
{
private:
	LPDIRECT3DTEXTURE9 m_pTexBack; //��׶���
	LPDIRECT3DTEXTURE9 m_pTexWindows; // ����â

public:
	CUi_MainMenu(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_MainMenu(const CGameObject& rhs);
	~CUi_MainMenu();

public:
	void Ready_Object();
	void Update_Object();
	void LateUpdate_Object();
	void Render_Object();

	void Key_Input();





};

