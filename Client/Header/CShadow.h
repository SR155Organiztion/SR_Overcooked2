#pragma once
#include "CGameObject.h"
#include "CVIBuffer.h"
#include "CTransform.h"

class CShadow :
    public CGameObject
{
protected:
	explicit CShadow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShadow(const CGameObject& rhs);
	virtual ~CShadow();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	virtual			void		Set_VIBuffer(CVIBuffer* _pVIBuffer) {
		m_pVIBuffer = _pVIBuffer;
		m_pVIBuffer->AddRef();
	}
	virtual			void		Set_Transform(CTransform* _pTransform) {
		m_pTransform = _pTransform;
		m_pTransform->AddRef();
	}

private:
	CVIBuffer* m_pVIBuffer;
	CTransform* m_pTransform;

public:
	static CShadow* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};

