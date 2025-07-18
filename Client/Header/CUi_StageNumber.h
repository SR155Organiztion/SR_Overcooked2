#pragma once
#include "CUi.h"
class CUi_StageNumber : public CUi
{
private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CVIBuffer* m_pBufferCom;

	list<UIDATA> m_listData;
	UIDATA m_tData;

public:
	CUi_StageNumber();
	CUi_StageNumber(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_StageNumber(const CUi_StageNumber& rhs);
	~CUi_StageNumber();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	HRESULT Add_Component();

	CGameObject* Make_StageNumber(bool _m_bVisible, int _Number);
	void UpdatePosition(const _vec3& _vPos);


	void On_Off(bool bProcess)
	{
		m_tData.m_bProcess = bProcess;

	}

	void Free();
};

