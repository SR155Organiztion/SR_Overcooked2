#pragma once

#include "CBase.h"
#include "CLayer.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent* Get_Component(COMPONENTID eID,
		const _tchar* pLayerTag,
		const _tchar* pObjTag,
		const _tchar* pComponentTag);

	CGameObject* Get_GameObject(
		const _tchar* _pLayerTag
		, const _tchar* _pObjTag);

	HRESULT Delete_GameObject(
		const _tchar* _pLayerTag,
		const _tchar* _pObjTag,
		const CGameObject* _pObj);

	CLayer* Get_Layer(const _tchar* _pLayerTag);

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene()PURE;

protected:
	map<const _tchar*, CLayer*>			m_mapLayer;
	LPDIRECT3DDEVICE9					m_pGraphicDev;

protected:
	virtual void			Free();

};

END