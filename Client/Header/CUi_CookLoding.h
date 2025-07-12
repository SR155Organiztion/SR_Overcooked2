#pragma once
#include "CUi_Gauge.h"

class CUi_CookLoding : public CUi_Gauge
{
private:

	Engine::CSprite* m_pSpriteCom; ///������
	Engine::CSprite* m_pSpriteCom2; ///BOX �̹���
	LPD3DXSPRITE m_pSprite;

	list<UIDATA> m_listData;

	UIDATA m_tData;

	RECT SrcRect; //ũ��
	RECT SrcRect2;

	int m_iFrameCount;
	int m_iGaugeFrame;

	_bool	m_bProcess = false;	///< ���� ���� ������ ����
	_float	m_fProgress = 0.f;	///< ���� ���� ���൵ (0.0f ~ 1.0f) (���� ������ 0.0f ~2.0f)

public:
	CUi_CookLoding(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_CookLoding(const CGameObject& rhs);
	~CUi_CookLoding();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	void Make_cookLoding(bool _m_bProcess, float _m_fProgress, _vec3 _m_vPos);
	HRESULT Add_Component();
	HRESULT Delete_Component();

private:
	void Free();
};

