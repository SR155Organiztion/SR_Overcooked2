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
public:
	typedef struct UiData
	{
		Engine::CRecipeMgr::tagRecipe Recipe;
		CRecipeMgr::RECIPETYPE m_eType;

		bool m_bIsShow = false;

		int m_iWidth = 0; /// ���� ����
		int m_iHeight = 0;/// ���� ����
		int m_iGap = 10; //����

		_vec3 m_vScale{ 0.5f, 0.5f, 0.f }; ///���� ũ��
		float m_fXScale = 0.25f;  /// ���� ũ��
		float m_fYScale = 0.35f; /// ���� ũ��
		RECT* SrcRect = nullptr; ///ũ��
		RECT SrcRect2; /// ������ ũ��
		RECT SrcRect3; /// ��ε� box ũ��

		D3DXVECTOR3 m_vPos{ 0,0,0 }; ///���� ��ġ
		D3DXVECTOR3 m_vStartPos{ 0,0,0 }; ///���� ��ġ
		D3DXVECTOR3 m_vTargetPos{ 0,0,0 }; ///�̵��� ��ġ

		DWORD m_dwStartTime = 0.f; ///���� �ð�
		DWORD m_dwLimitTime = 0.f; ///���� �ð�
		DWORD m_dwTime = 0.f; ///���� �ð�
		DWORD m_dwHideTime = 0.f; //������� �ð�

		bool m_bVisible = false; ///���̴� ��� 
		bool m_bAnimating = false; /// �ִϸ��̼� �� ����

		float m_fAnimTime = 0.f; ///���� �ִϸ��̼� �ð�
		float m_fAnimDuration = 0.f; /// �ִϸ��̼� �� �ҿ� �ð�(��)

	}UIDATA, ORDER, ICON, COOK;


protected:
	
	BUTTON_TYPE m_eButtonType = END_BUTTON;
	GAUGE_TYPE m_eGaugeType = END_GAUGE;

	int m_iAlpha[5];  // ���İ� ���� ����
	int m_iNonAlpha;  // ���İ� ���� ����

	int m_iseconds;
	int m_iminute;

	D3DXVECTOR3* m_pCenter; //ȸ�� ��

	UIDATA m_tData;


public:
	CUi();
	CUi(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi(const CGameObject& rhs);
	~CUi();

	virtual void Render_GameObject();
	virtual	_int Update_GameObject(const _float& fTimeDelta);
	virtual	void LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Add_Component();
	UIDATA* Get_UiData() { return &m_tData; }

};

