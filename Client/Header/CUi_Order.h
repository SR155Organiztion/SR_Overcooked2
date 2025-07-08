#pragma once
#include "CUi_Object.h"
#include "CTransform.h"

class CUi_Order :  public CUi
{

private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CSprite* m_pSpriteCom2;
	Engine::CSprite* m_pSpriteCom3;

	/*vector<CSprite*> m_vecSprite;*/
	vector<wstring> m_vecType; //�̸� ����
	vector<CUi_Order*> m_vecOrder; //m_bVisible ���� ����
	vector<float> m_vecTime; //�ð� ����

	OBJECT_TYPE m_eObjectType;

	RECT* m_pSrcRect; //��ġ
	DWORD m_dwStartTime; //���� �ð�
	DWORD m_dwLimitTime; //���� �ð�
	DWORD m_dwTime; //���� �ð�
	DWORD m_dwHideTime; //������� �ð�

	bool m_bAnimating;    // �ִϸ��̼� �� ����
	float m_fAnimTime;     // ���� �ִϸ��̼� �ð�
	float m_fAnimDuration; // �ִϸ��̼� �� �ҿ� �ð�(��)
	D3DXVECTOR3 m_vStartPos;      // ���� ��ġ
	D3DXVECTOR3 m_vTargetPos;     // ���� ��ġ(�����ؾ��� �ڸ�)
	D3DXVECTOR3 m_vPos;           // ���� ��ġ(�������� ���)

	int m_iseconds;
	int m_iminute;
	int m_pGauge;

	bool m_bVisible;

public:
	CUi_Order();
	CUi_Order(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Order(const CUi_Order& rhs);
	~CUi_Order();

	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	HRESULT Add_Component();

	void Get_Order(wstring _Name, float _time);

	DWORD Get_StartTIme() 
	{
		return m_dwStartTime;
	}

	DWORD Get_LimitTIme() 
	{
		return m_dwLimitTime;
	}

	DWORD Get_Time() 
	{
		return m_dwTime;
	}



private:
	void Free();

};

