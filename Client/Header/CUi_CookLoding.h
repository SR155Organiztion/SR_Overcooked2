#pragma once
#include "CUi_Gauge.h"
#include "CIngredient.h"

class CUi_CookLoding : public CUi_Gauge
{
private:

	Engine::CSprite* m_pSpriteCom; ///게이지
	Engine::CSprite* m_pSpriteCom2; ///BOX 이미지
	Engine::CTransform* m_pTransformCom;
	LPD3DXSPRITE m_pSprite;

	list<UIDATA> m_listData;

	UIDATA m_tData;

	int m_iFrameCount;
	int m_iGaugeFrame;

	_bool	m_bProcess = false;	///< 현재 가공 중인지 여부
	_float	m_fProgress = 0.f;	///< 현재 가공 진행도 (0.0f ~ 1.0f) (가열 조리는 0.0f ~2.0f)

public:
	CUi_CookLoding(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_CookLoding(const CGameObject& rhs);
	~CUi_CookLoding();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	CGameObject* Make_cookLoding(bool _m_bProcess, float _m_fProgress);
	HRESULT Add_Component();
	void UpdatePosition(CGameObject* _pGameObject, const _vec3& _vPos);

private:
	void Free();
};

