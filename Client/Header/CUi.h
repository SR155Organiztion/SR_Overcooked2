#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"
#include <Windows.h>
#include <mmsystem.h> 
#include "CTexture.h"
#include "CSprite.h"
#include "CRcTex.h"
#include "CRenderer.h"
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
	enum Ui_STATE { ACTIVATY, DEAD };

protected:
	Engine::CSprite* m_pSpriteCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

public:
	CUi(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi(const CGameObject& rhs);
	~CUi();

	virtual void Render_GameObject();
	virtual	_int Update_GameObject(const _float& fTimeDelta);
	virtual	void LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Add_Component();
	

};

