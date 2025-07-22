#pragma once
#include "CUi.h"
class CUi_StageInfo : public CUi
{
private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pTextureCom2;
	Engine::CVIBuffer* m_pBufferCom;
	list<UIDATA> m_listData;
	int m_iStarNumber;

public:
	CUi_StageInfo();
	CUi_StageInfo(LPDIRECT3DDEVICE9 _pGraphicDev);
	CUi_StageInfo(const CGameObject& _rhs);
	~CUi_StageInfo();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	HRESULT Add_Component();
	CUi_StageInfo* Make_StageInfo(int _Number, int _StarNumber, _vec3 _vPos);

private:
	void Free();
};

