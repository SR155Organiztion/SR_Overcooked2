#pragma once
#include "CUi_Object.h"
class CUi_TimeLimit : public CUi_Object
{
private:
	float m_fmainTime; //���� �ð�, �ʱⰪ�� �ִ� �ð����� ����
	float m_fMaxTime; //�ִ� �ð�
	_tchar* m_szTime[5]; // ǥ���� �ð� (����)

private:
	CUi_TimeLimit(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_TimeLimit(const CGameObject& rhs);
	~CUi_TimeLimit();

public: 
	void Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Update_GameObject();
	void Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
};

