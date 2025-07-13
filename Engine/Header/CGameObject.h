#pragma once
#include "CBase.h"
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pComponentTag);
	_float		Get_ViewZ() const { return m_fViewZ; }
	

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() = 0;

public:
	void		Compute_ViewZ(const _vec3* pPos);
	void		Set_SelfId(const _tchar* _szSelfId) {
		m_szSelfId = _szSelfId;
	}

	const _tchar* Get_SelfId() const {
		return m_szSelfId;
	}

protected:
	virtual			HRESULT		Set_Material();

protected:
	map<const _tchar*, CComponent*>			m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	const _tchar*							m_szSelfId;

	_float									m_fViewZ;

private:
	CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

protected:
	virtual		void		Free();
};

END