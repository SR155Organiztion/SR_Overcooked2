#pragma once

#include	"CBase.h"
#include	"CScene.h"
#include	"CGameObject.h"
#include	<stack>
BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit	CManagement();
	virtual		~CManagement();

public:
	CComponent* Get_Component(COMPONENTID eID,
		const _tchar* pLayerTag,
		const _tchar* pObjTag,
		const _tchar* pComponentTag);

	CGameObject* Get_GameObject(
		const _tchar* _pLayerTag,
		const _tchar* _pObjTag);

	HRESULT Delete_GameObject(
		const _tchar* _pLayerTag,
		const _tchar* _pObjTag,
		const CGameObject* pObj);

	CLayer* Get_Layer(const _tchar* _pLayerTag);
	CScene* Get_Scene() { return m_pScene; }

public:
	HRESULT			Set_Scene(CScene* pScene);
	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene(const _float& fTimeDelta);
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

	HRESULT			Go_Stage(CScene* pScene);
	HRESULT			Back_Select();

private:
	CScene* m_pScene;
	stack<CScene*> m_SceneStack;

public:
	virtual void			Free();
};

END