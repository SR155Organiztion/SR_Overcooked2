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
* - �Ұ�: UI �θ� Ŭ����
* @date 2025-06-29
* @section CREATINFO �ۼ�����
* - �ۼ���: �輭��
* - �ۼ���: 2025.06.29
* @section MODIFYINFO ���� ����
* - ������/ ������: ��������
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

