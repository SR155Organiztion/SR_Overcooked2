#pragma once
#include "CUi.h"
class CUi_Object :  public CUi
{
private:
	map<_tchar*, LPDIRECT3DTEXTURE9> m_mapObject;

public:
	CUi_Object(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Object(const CGameObject& rhs);
	~CUi_Object();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	HRESULT Add_Order(LPDIRECT3DDEVICE9 pGraphicDev, OBJECT_TYPE _type);
	void Remove_Order(wstring& key);

private:
	void Free();
	
};

