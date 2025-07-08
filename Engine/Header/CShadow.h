#pragma once
#include "CGameObject.h"
#include "CVIBuffer.h"
#include "CTransform.h"
#include "CShader.h"

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
		m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", m_pVIBuffer });
	}
	virtual			void		Set_Transform(CTransform* _pTransform) {
		m_pTransform = _pTransform;
		m_pTransform->AddRef();
		m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", m_pTransform });
	}

private:
	CVIBuffer* m_pVIBuffer;
	CTransform* m_pTransform;
	CShader* m_pShader;

public:
	static CShadow* Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	HRESULT			Add_Component();

private:
	virtual		void		Free();
};

