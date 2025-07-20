#pragma once
#include "CUi_Timer.h"

class CUi_TimeOut : public CUi_Timer
{

private:

	Engine::CSprite* m_pSpriteCom; //�̹���

	bool m_bReady = false; // false: �Ⱥ��̱� , true: ���̱�
	bool m_bGo = false; // false: �Ⱥ��̱� , true: ���̱�
	bool m_bTimeOut = false; // false: ���� �� , true: ���� �Ϸ�

public:
	CUi_TimeOut();
	CUi_TimeOut(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_TimeOut(const CUi_TimeOut& rhs);
	~CUi_TimeOut();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	HRESULT Add_Component();
	void Set_Ready(bool _bReady) { m_bReady = _bReady; }
	void Set_Go(bool _bGo) { m_bGo = _bGo; }
	void Set_TimeOut(bool _bTimeOut) { m_bTimeOut = _bTimeOut; }
	bool Get_TimeOut() { return m_bTimeOut; }

private: 
	void Free();
};




