#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"
#include <Windows.h>
#include <mmsystem.h> 
#include "CTexture.h"
#include "CSprite.h"
#include "CRcTex.h"
#include "CRenderer.h"
#include "CProtoMgr.h"

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
protected:
	BUTTON_TYPE m_eButtonType = END_BUTTON;
	GAUGE_TYPE m_eGaugeType = END_GAUGE;

	D3DXVECTOR3 m_vPos; //위치
	int m_iAlpha[5];  // 알파값 조정 있음
	int m_iNonAlpha;  // 알파값 조정 없음
	float m_tXScale; // 크기
	float m_tYScale; // 크기
	D3DXVECTOR3* m_pCenter; //회전 등


public:
	CUi();
	CUi(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi(const CGameObject& rhs);
	~CUi();

	virtual void Render_GameObject();
	virtual	_int Update_GameObject(const _float& fTimeDelta);
	virtual	void LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Add_Component();
	

};

