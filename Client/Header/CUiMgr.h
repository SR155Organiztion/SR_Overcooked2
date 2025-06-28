#pragma once
#include "CGameObject.h"
#include "CUi.h"

class CUiMgr :public CGameObject
{
private:
	std::vector<CGameObject*> m_pUiList;

private:
	explicit CUiMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUiMgr(const CGameObject& rhs);
	~CUiMgr();

public:
	HRESULT Ready_GameObject();
	void Update_GameObject();
	void LateUpdate_GameObject();
	void Render_GameObject();

	void Add_GameObject();
	void Remove_GameObject();

	void Free();
};

