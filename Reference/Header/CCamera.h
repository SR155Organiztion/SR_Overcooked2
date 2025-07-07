#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CCamera :  public CGameObject
{
protected:
	explicit	CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

	const _matrix* Get_View() {
		//D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
		return &m_matView;
	}

	const _matrix* Get_Projection() {
		return &m_matProj;
	}

	const _matrix* Get_World() {
		D3DXMatrixInverse(&m_matWorld, nullptr, &m_matView);

		return &m_matWorld;
	}

protected:
	_matrix		m_matView, m_matProj, m_matWorld;
	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFov, m_fAspect, m_fNear, m_fFar;

protected:
	virtual		void		Free();

};

END
