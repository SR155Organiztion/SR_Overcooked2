#pragma once
#include "Engine_Define.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CObjectPoolMgr : public CBase
{
	DECLARE_SINGLETON(CObjectPoolMgr)

private:
	explicit CObjectPoolMgr();
	virtual ~CObjectPoolMgr();

public:
	HRESULT			Register_Object(const _tchar* pObjTag, CGameObject* pGameObject);
	CGameObject*	Get_Object(const _tchar* pObjTag);
	void			Return_Object(const _tchar* pObjTag, CGameObject* pGameObject);

private:
	unordered_map<wstring, vector<CGameObject*>> m_mapObject;

public:
	virtual	void	Free();
};

END