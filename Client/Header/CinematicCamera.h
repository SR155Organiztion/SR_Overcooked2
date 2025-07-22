#pragma once
#include "CCamera.h"

class CinematicCamera : public CCamera
{
private:
	explicit CinematicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CinematicCamera();

private:
	CGameObject* m_pTarget;

public:
	HRESULT	Ready_GameObject(
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {}
	virtual			void		Look_AtFront();

	static CinematicCamera* Create(
		LPDIRECT3DDEVICE9 pGraphicDev,
		CGameObject* _pTarget,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	virtual void		Free();
};

