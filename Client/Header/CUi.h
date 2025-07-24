#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"
#include "CTexture.h"
#include "CSprite.h"
#include "CRcTex.h"
#include "CRenderer.h"
#include "CProtoMgr.h"

#include <Windows.h>
#include <mmsystem.h> 

#include "CRecipeMgr.h"
/**
* @mainpage
* - 소개: UI 부모 클래스
* @date 2025-06-29
* @section CREATINFO 작성정보
* - 작성자: 김서영
* - 작성일: 2025.06.29
* @section MODIFYINFO 수정 정보
* - 수정일/ 수정자: 수정내역
*/

class CUi :public CGameObject
{
public:
	typedef struct UiData
	{
		Engine::CRecipeMgr::tagRecipe Recipe;
		CRecipeMgr::RECIPETYPE m_eType;

		int m_iNumber = 0;
		int m_iStarNumber = 0;

		//보이기 및 삭제
		bool m_bFail = false; ///true일때 주문서 삭제, false일때 동작
		bool m_bRemove = false;
		bool m_bVisible = false;
		bool m_bProcess = false;
		bool m_bIconDown = false;
		bool m_bIsMgr = true; //false일때 랜더 하는 변수

		int m_iWidth = 0; /// 가로 길이
		int m_iHeight = 0;/// 세로 길이
		int m_iGap = 10; //간격

		_vec3 m_vScale{ 0.5f, 0.5f, 0.f }; ///벡터 크기
		float m_fXScale = 0.25f;  /// 가로 크기
		float m_fYScale = 0.35f; /// 세로 크기
		RECT* m_pSrcRect = nullptr; ///크기

		D3DXVECTOR3 m_vPos{ 0,0,0 }; ///현재 위치
		D3DXVECTOR3 m_vStartPos{ 0,0,0 }; ///시작 위치
		D3DXVECTOR3 m_vTargetPos{ 0,0,0 }; ///이동할 위치

		DWORD m_fDeltaTime = 0.f; //델타타임을 저장하는 변수
		DWORD m_dwStartTime = 0.f; ///시작 시간
		DWORD m_dwLimitTime = 0.f; ///제한 시간
		_float m_dwTime; ///남은 시간
		DWORD m_dwHideTime = 0.f; //사라지는 시간

		bool m_bAnimating = false; /// 애니메이션 중 여부
		float m_fAnimTime = 0.f; ///현재 애니메이션 시간
		float m_fAnimDuration = 0.f; /// 애니메이션 총 소요 시간(초)

		//페이드아웃
		float m_fScale = 1.0f;
		float m_fShrinkSpeed = 0.5f; //줄어드는 속도

	}UIDATA, ORDER, ICON, COOK;


protected:
	
	BUTTON_TYPE m_eButtonType = END_BUTTON;
	GAUGE_TYPE m_eGaugeType = END_GAUGE;

	int m_iAlpha[5];  // 알파값 조정 있음
	int m_iNonAlpha;  // 알파값 조정 없음

	int m_iseconds;
	int m_iminute;

	D3DXVECTOR3* m_pCenter; //회전 등

	UIDATA m_tData;

	_float m_fX;
	_float m_fY;
	_matrix m_ViewMatrix{};
	_matrix m_ProjMatrix{};
	_matrix	m_SaveWorldMatrix{};
	_matrix m_SaveViewMatrix{};
	_matrix m_SaveProjMatrix{};

public:
	CUi();
	CUi(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi(const CGameObject& rhs);
	~CUi();

	virtual void Render_GameObject();
	virtual	_int Update_GameObject(const _float& fTimeDelta);
	virtual	void LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Add_Component();
	void Begin();
	void End();

	UIDATA* Get_UiData() { return &m_tData; }

};

