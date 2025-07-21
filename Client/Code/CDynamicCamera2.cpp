#include "pch.h"
#include "CDynamicCamera2.h"
#include "CDInputMgr.h"
#include "CTransform.h"

CDynamicCamera2::CDynamicCamera2(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{
}

CDynamicCamera2::~CDynamicCamera2()
{
}

HRESULT CDynamicCamera2::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if (FAILED(Engine::CCamera::Ready_GameObject()))
		return E_FAIL;

	m_ePerspeective = FIRST;

	return S_OK;
}

_int CDynamicCamera2::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);
	
	if (m_pTarget == nullptr)
		return 0;

	if (GetAsyncKeyState('J')) {
		_vec3 test = { 10.f, 0.f, 10.f };
		Focus(test, 3.f);
	}


	if (m_bFocus && m_fFocusDuration > 0.f)
	{
		m_fElapsedTime += fTimeDelta;

		if (m_pFocusTarget != nullptr) {
			CTransform* _pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		}
		else {
			m_vEye += m_vFocusPos;
			m_vAt += m_vFocusPos;
		}

		if (m_fElapsedTime >= m_fFocusDuration)
			End_Focus();

		return iExit;
	}

	if (m_ePerspeective == PERSPECTIVE::FIRST) {
		CTransform* _pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		_pTargetTransform->Get_Info(INFO_POS, &m_vEye);
		_vec3 vLook;
		_pTargetTransform->Get_Info(INFO_LOOK, &vLook);

		m_vAt = m_vEye + vLook;

		_pTargetTransform->Get_Info(INFO_UP, &m_vUp);
		
	}
	else if (m_ePerspeective == PERSPECTIVE::THIRD){
		CTransform* _pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		
		_pTargetTransform->Get_Info(INFO_POS, &m_vAt);
		
		m_vEye = m_vAt + m_vOffset;

	}

	return iExit;
}

void CDynamicCamera2::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CCamera::LateUpdate_GameObject(fTimeDelta);
}

void CDynamicCamera2::Set_Target(CGameObject* _pGameObject)
{
	m_pTarget = _pGameObject;
}

void CDynamicCamera2::Set_Offset(const _float _fX, const _float _fY, const _float _fZ)
{
	m_vOffset = { _fX, _fY, _fZ };
}

void CDynamicCamera2::Set_Perspective(PERSPECTIVE _ePerspective)
{
	m_ePerspeective = _ePerspective;
}

void CDynamicCamera2::Focus(CGameObject* m_pGameObject, float _fDurationTime = 0.f)
{
	m_pFocusTarget = m_pGameObject;
	m_fFocusDuration = _fDurationTime;
	m_bFocus = true;
}

void CDynamicCamera2::Focus(_vec3 _vPos, float _fDurationTime)
{
	m_vFocusPos = _vPos;
	m_fFocusDuration = _fDurationTime;
	m_bFocus = true;
}

void CDynamicCamera2::End_Focus()
{
	m_pFocusTarget = nullptr;
	m_fElapsedTime = 0.f;
	m_bFocus = false;
}


CDynamicCamera2* CDynamicCamera2::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera2* pCamera = new CDynamicCamera2(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("DynamicCamera2 Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CDynamicCamera2::Free()
{
	Engine::CCamera::Free();

}
