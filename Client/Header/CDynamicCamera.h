#pragma once

#include "CCamera.h"

class CDynamicCamera :  public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

public:
	HRESULT	Ready_GameObject(const _vec3* pEye, 
		const _vec3* pAt, 
		const _vec3* pUp, 
		const _float& fFov, 
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {}
	void						On_Focus(const _vec3* _vFocus);
	_bool						Move_To(const _vec3* _vPos);
	_bool						Move_To_And_Focus(const _vec3* _vTarget);
	void						Release_Focus();

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move();
	void		Mouse_Fix();

private:
	_float		m_fSpeed;
	_bool		m_bFix;
	_bool		m_bCheck;
	_bool		m_bLCheck;
	_bool		m_bInGame = true;
	_bool		m_bIsMoving = false;

	_vec3		m_vDefaultEye;
	_vec3		m_vDefaultAt;
	_vec3		m_vDefaultUp;

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _vec3* pEye,
									const _vec3* pAt,
									const _vec3* pUp,
									const _float& fFov = D3DXToRadian(60.f),
									const _float& fAspect = (_float)WINCX / WINCY,
									const _float& fNear = 0.1f,
									const _float& fFar = 1000.f);

	_bool Get_IsMoving() {
		return m_bIsMoving;
	}

private:
	virtual void		Free();
};

