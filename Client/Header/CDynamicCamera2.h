#pragma once
#include "CCamera.h"

class CDynamicCamera2 : public CCamera
{
private:
	explicit CDynamicCamera2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera2();

public:
	
	enum PERSPECTIVE { FIRST, THIRD };

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

	void	Set_Target(CGameObject* _pGameObject); 
	void	Set_Offset(const _float _fX, const _float _fY, const _float _fZ);
	void	Set_Perspective(PERSPECTIVE _ePerspective);
	void	Set_MoveSpeed(_float);

	void	Focus(CGameObject* _pGameObject, float _fDurationTime, _bool _bMoveCam, _bool _bBackCam);
	void	Focus(_vec3 _vPos, float _fDurationTime, _bool _bMoveCam, _bool _bBackCam);

	void	Cam_Move(const _float _fTimeDelta);
	void	Cam_Back(const _float _fTimeDelta);

	void	End_Focus();

private:
	CGameObject* m_pTarget{};
	CGameObject* m_pFocusTarget{};
	_vec3		 m_vFocusPos{};

	float m_fFocusTimer{};
	float m_fElapsedTime{};
	float m_fFocusDuration{};
	PERSPECTIVE m_ePerspeective;
	
	_vec3 m_vOffset{};
	_bool m_bFocus = false;
	_bool m_bMove = false;
	_bool m_bBack = false;

	_float m_fMoveSpeed;
public:
	static CDynamicCamera2* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	virtual void		Free();
};

