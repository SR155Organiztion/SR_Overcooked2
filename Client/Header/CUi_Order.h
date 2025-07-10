#pragma once
#include "CUi_Object.h"
#include "CTransform.h"
#include "CRecipeMgr.h"

class CUi_Order :  public CUi
{
public:
	typedef struct OrderData
	{
		Engine::CRecipeMgr::RECIPETYPE m_eType;
		Engine::CRecipeMgr::tagRecipe Recipe;
		int m_iWidth;
		int m_iGap;
		float m_fXScale = 0.25f;
		float m_tYScale = 0.35f;
		D3DXVECTOR3 m_vStartPos; //���� ��ġ
		D3DXVECTOR3 m_vTargetPos; //�̵��� ��ġ
		D3DXVECTOR3 m_vPos; //���� ��ġ
		DWORD m_dwStartTime; ///���� �ð�
		DWORD m_dwLimitTime; ///���� �ð�
		DWORD m_dwTime; //���� �ð�
		DWORD m_dwHideTime; //������� �ð�
		bool m_bVisible; //���̴� ��� 
		bool m_bAnimating; /// �ִϸ��̼� �� ����
		float m_fAnimTime; ///���� �ִϸ��̼� �ð�
		float m_fAnimDuration; /// �ִϸ��̼� �� �ҿ� �ð�(��)
	}ORDER;

private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CSprite* m_pSpriteCom2;
	Engine::CSprite* m_pSpriteCom3;

	ORDER m_tData;
	list<ORDER> m_listData; // ������ ����

	RECT m_SrcRect;
	static bool m_bRemoved;

	int m_iseconds;
	int m_iminute;
	int m_pGauge;

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
	void OrdersAnimation();

	void Make_Order(Engine::CRecipeMgr::tagRecipe _Recipe);
	list<ORDER>* Get_OrderData();
	


private:
	void Free();

};
