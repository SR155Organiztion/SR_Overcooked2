#pragma once
#include "CUi.h"

class CUi_Button :   public CUi
{
	enum BUTTON_TYPE 
	{START_BUTTON, CONTINUE_BUTTON, STATE_BUTTON, OK_BUTTON, NO_BUTTON, END_BUTTON};


private: 
	LPDIRECT3DTEXTURE9 m_pTexBtn; //�ϴû� BOX ��ư
	LPDIRECT3DTEXTURE9 m_pTexLock;//��� �� ��ư (���� �� ����)
	LPDIRECT3DTEXTURE9 m_pTexLockImage;//�ڹ��� �׸�
	LPDIRECT3DTEXTURE9 m_pTexMenuBack; // ��ư �ڿ� �Ͼ� �� ��׶���
	LPDIRECT3DTEXTURE9 m_pTexMenuHover; // ��Ȳ�� ��ư
	LPDIRECT3DTEXTURE9 m_pTexScroll; //��ư ������ ������ �� 
	LPDIRECT3DTEXTURE9 m_pTexScrollStart; // ������
	LPDIRECT3DTEXTURE9 m_pTexOkBox; //���� ��ư
	LPDIRECT3DTEXTURE9 m_pTexNoBox;  //���� ��ư
	_tchar* m_szName; // ������ �̸�

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

