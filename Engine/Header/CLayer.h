#pragma once
#include "CBase.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CComponent*		Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag);
	CGameObject*	Get_GameObject(const _tchar* _pObjTag);
	const multimap<const _tchar*, CGameObject*>* const Get_GameObjectMap();
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject, LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT			Delete_GameObject(const _tchar* _pObjTag, const CGameObject* _pObj);

public:
	HRESULT			Ready_Layer();
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(const _float& fTimeDelta);

private:
	multimap<const _tchar*, CGameObject*>			m_mapObject;

public:
	static CLayer* Create();

private:
	virtual void	Free();
};

END