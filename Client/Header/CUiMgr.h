#pragma once
#include "CGameObject.h"
#include "CUi.h"

class CUiMgr :public CGameObject
{
private:
	map<_tchar*, CGameObject*> m_mapUi;

private:
	explicit CUiMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUiMgr(const CGameObject& rhs);
	~CUiMgr();

public:
	HRESULT Ready_GameObject();
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();

	void Add_GameObject();
	void Remove_GameObject();

	void Free();
};

