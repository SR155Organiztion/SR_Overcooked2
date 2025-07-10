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
		D3DXVECTOR3 m_vStartPos; //시작 위치
		D3DXVECTOR3 m_vTargetPos; //이동할 위치
		D3DXVECTOR3 m_vPos; //현재 위치
		DWORD m_dwStartTime; ///시작 시간
		DWORD m_dwLimitTime; ///제한 시간
		DWORD m_dwTime; //남은 시간
		DWORD m_dwHideTime; //사라지는 시간
		bool m_bVisible; //보이는 기능 
		bool m_bAnimating; /// 애니메이션 중 여부
		float m_fAnimTime; ///현재 애니메이션 시간
		float m_fAnimDuration; /// 애니메이션 총 소요 시간(초)
	}ORDER;

private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CSprite* m_pSpriteCom2;
	Engine::CSprite* m_pSpriteCom3;

	ORDER m_tData;
	list<ORDER> m_listData; // 데이터 저장

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
