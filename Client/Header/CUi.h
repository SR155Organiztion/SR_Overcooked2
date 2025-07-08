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
protected:
	BUTTON_TYPE m_eButtonType = END_BUTTON;
	GAUGE_TYPE m_eGaugeType = END_GAUGE;

	D3DXVECTOR3 m_vPos; //��ġ
	int m_iAlpha[5];  // ���İ� ���� ����
	int m_iNonAlpha;  // ���İ� ���� ����
	float m_tXScale; // ũ��
	float m_tYScale; // ũ��
	D3DXVECTOR3* m_pCenter; //ȸ�� ��


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

