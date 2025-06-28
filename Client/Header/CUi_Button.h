#pragma once
#include "CUi.h"

class CUi_Button :   public CUi
{
	enum BUTTON_TYPE 
	{START_BUTTON, CONTINUE_BUTTON, STATE_BUTTON, OK_BUTTON, NO_BUTTON, END_BUTTON};


private: 
	LPDIRECT3DTEXTURE9 m_pTexBtn; //하늘색 BOX 버튼
	LPDIRECT3DTEXTURE9 m_pTexLock;//잠금 된 버튼 (색이 더 찐함)
	LPDIRECT3DTEXTURE9 m_pTexLockImage;//자물쇠 그림
	LPDIRECT3DTEXTURE9 m_pTexMenuBack; // 버튼 뒤에 하얀 거 백그라운드
	LPDIRECT3DTEXTURE9 m_pTexMenuHover; // 주황색 버튼
	LPDIRECT3DTEXTURE9 m_pTexScroll; //버튼 누르면 나오는 거 
	LPDIRECT3DTEXTURE9 m_pTexScrollStart; // 선택지
	LPDIRECT3DTEXTURE9 m_pTexOkBox; //수락 버튼
	LPDIRECT3DTEXTURE9 m_pTexNoBox;  //거절 버튼
	_tchar* m_szName; // 선택지 이름

	BUTTON_TYPE m_eType = END_BUTTON;

	int m_iKeyPress;
	int m_iMousPicking;

protected:
	CUi_Button(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Button(const CGameObject& rhs);
	~CUi_Button();

public:
	void Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev, BUTTON_TYPE _cValue);
	void Update_GameObject();
	void LateUpdate_GameObject();
	void Render_GameObject();

	void Mouse_Pickig(int _m_iKeyPress);
	void Key_Input(int m_iMousPicking);

private:
	void Free();

};

