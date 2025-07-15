#pragma once
#include "CUi_Gauge.h"
#include "CIngredient.h"

class CUi_CookLodingBox : public CUi_Gauge
{
private:

	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CVIBuffer* m_pBufferCom;

	list<UIDATA> m_listData;
	UIDATA m_tData;
	float CookLodingYOffset = 2.f;

	int m_iFrameCount;
	int m_iGaugeFrame;

	_bool	m_bProcess = false;	///< 현재 가공 중인지 여부
	_float	m_fProgress = 0.f;	///< 현재 가공 진행도 (0.0f ~ 1.0f) (가열 조리는 0.0f ~2.0f)

	bool m_bIsShow = false;

public:
	CUi_CookLodingBox(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_CookLodingBox(const CGameObject& rhs);
	~CUi_CookLodingBox();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();

	CGameObject* Make_cookLodingBox(bool _m_bProcess);
	HRESULT Add_Component();
	void UpdatePosition(const _vec3& _vPos);
	void Show() {
		m_bIsShow = TRUE;
	}

	void Hide() {
		m_bIsShow = FALSE;
	}

	void On_Off(bool _false)
	{
		m_tData.m_bEnd = _false;
	}

private:
	void Free();
};

