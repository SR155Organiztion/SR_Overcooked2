#pragma once
#include "CUi_Object.h"
#include "CTransform.h"

class CUi_Order :  public CUi
{
	enum ORDER_TYPE { SALAD_LETTUCE, SALAD_LETTUCE_TOMATO, SALAD_CUCUMBER_LETTUCE_TOMATO
		, SASHIMI_FISH,SASHIMI_SHRIMP,SUSHI_FISH, SUSHI_CUCUMBER, PASTA_TOMATO, OREDER_END };
	struct OrderData
	{
		ORDER_TYPE eType;
		wstring imagePath;
		int width;
		float time;
		D3DXVECTOR3 startPos;
		D3DXVECTOR3 targetPos;
		D3DXVECTOR3 vPos;
		DWORD dwTime;
		DWORD dwStartTime;
		DWORD dwLimitTime;
		DWORD dwHideTime;
		bool bVisible;
		bool bAnimating;
		float fAnimTime;
		float fAnimDuration;
	};

private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CSprite* m_pSpriteCom2;
	Engine::CSprite* m_pSpriteCom3;

	list<OrderData> m_listDate; // ������ ����

	OrderData m_tData;
	ORDER_TYPE m_eObjectType;
	RECT m_SrcRect;
	int m_iGap;
	OrderData data;
	//DWORD m_dwStartTime; //���� �ð�
	//DWORD m_dwLimitTime; //���� �ð�
	//DWORD m_dwTime; //���� �ð�
	//DWORD m_dwHideTime; //������� �ð�

	//bool m_bAnimating;    // �ִϸ��̼� �� ����
	//float m_fAnimTime;     // ���� �ִϸ��̼� �ð�
	//float m_fAnimDuration; // �ִϸ��̼� �� �ҿ� �ð�(��)
	//D3DXVECTOR3 m_vStartPos;      // ���� ��ġ
	//D3DXVECTOR3 m_vTargetPos;     // ���� ��ġ(�����ؾ��� �ڸ�)
	//D3DXVECTOR3 m_vPos;           // ���� ��ġ(�������� ���)

	int m_iseconds;
	int m_iminute;
	int m_pGauge;

	/*bool m_bVisible;*/

public:
	CUi_Order();
	CUi_Order(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Order(const CUi_Order& rhs);
	~CUi_Order();

	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	HRESULT Add_Component();

	void Get_Order(ORDER_TYPE _Name, float _time);




private:
	void Free();

};

