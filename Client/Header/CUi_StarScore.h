#pragma once
#include "CUi.h"
#include "CUi_Button.h"
#include "CSprite.h"

class CUi_StarScore :public CUi
{
private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CSprite* m_pSpriteCom2;
	Engine::CSprite* m_pSpriteCom3;
	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT m_pFont;

	int m_iStarScoreNumber;
	int m_iDeliveredScoreNumber;
	int m_iFailedScoreNumber;
	int m_iTotalScoreNumber;

	_bool m_bIsShow = FALSE;


public:
	explicit CUi_StarScore(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUi_StarScore(const CGameObject& rhs);
	~CUi_StarScore();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	HRESULT Add_Component();
	void Set_StarScore(int _Number) { m_iStarScoreNumber = _Number; }/// 획득한 별 개수
	void Set_DeliveredScore(int _Number) { m_iDeliveredScoreNumber = _Number; } /// 배달된 음식 점수
	void Set_FailedScore(int _Number) { m_iFailedScoreNumber = _Number; } /// 실패한 음식 점수
	void Set_TotalScore(int _Number) { m_iTotalScoreNumber = _Number; } /// 총합 점수

	void Show() {
		m_bIsShow = TRUE;
	}
	void Hide() {
		m_bIsShow = FALSE;
	}
	void Free();
	 
};

