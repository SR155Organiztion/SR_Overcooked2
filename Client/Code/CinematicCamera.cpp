#include "pch.h"
#include "CDInputMgr.h"
#include "CinematicCamera.h"
#include "CTransform.h"

CinematicCamera::CinematicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{
}

CinematicCamera::~CinematicCamera()
{
}


HRESULT CinematicCamera::Ready_GameObject(const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if (FAILED(Engine::CCamera::Ready_GameObject()))
		return E_FAIL;


	return S_OK;
}

_int CinematicCamera::Update_GameObject(const _float& fTimeDelta)
{
	Look_AtFront();

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	return 0;
}

void CinematicCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CCamera::LateUpdate_GameObject(fTimeDelta);
}

void CinematicCamera::Look_AtFront()
{
	CTransform* pTransform = dynamic_cast<CTransform*>(
		m_pTarget->Get_Component(ID_DYNAMIC, L"Com_Transform")
		);

	_vec3 vPlayerPos;
	pTransform->Get_Info(INFO_POS, &vPlayerPos);
	
	_vec3 vPlayerLook;
	pTransform->Get_Info(INFO_LOOK, &vPlayerLook);

	m_vEye = vPlayerPos + (vPlayerLook * 5.f);
	m_vAt = vPlayerPos;
	m_vUp = { 0.f, 1.f, 0.f };
}

CinematicCamera* CinematicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pTarget, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CinematicCamera* pCamera = new CinematicCamera(pGraphicDev);
	pCamera->m_pTarget = _pTarget;
	pCamera->m_pTarget->AddRef();
	if (FAILED(pCamera->Ready_GameObject(fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("CinematicCamera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CinematicCamera::Free()
{
	Safe_Release(m_pTarget);
	Engine::CCamera::Free();


}
