#pragma once
#include "CUi.h"
class CUi_BurntFood :   public CUi
{
private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CVIBuffer* m_pBufferCom;

	list<UIDATA> m_listData;
	UIDATA m_tData;
	float CookLodingYOffset = 1.f;

	int m_iFrameCount;
	int m_iGaugeFrame;
	float fElapsed = 0;
public:
	CUi_BurntFood();
	CUi_BurntFood(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_BurntFood(const CGameObject& rhs);
	~CUi_BurntFood();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	HRESULT Add_Component();

	CGameObject* Make_BurntFood(bool _m_bVisible);
	void UpdatePosition(const _vec3& _vPos);


	void On_Off(bool bProcess)
	{
		m_tData.m_bProcess = bProcess;

	}

	void Free();
};

