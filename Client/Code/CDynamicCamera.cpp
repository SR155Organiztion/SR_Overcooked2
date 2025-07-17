#include "pch.h"
#include "CDynamicCamera.h"
#include "CDInputMgr.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev), m_bFix(false), m_bCheck(false)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
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

	m_fSpeed = 5.f;


	return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);



	return iExit;
}

void CDynamicCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CCamera::LateUpdate_GameObject(fTimeDelta);

	Key_Input(fTimeDelta);
	if (!m_bInGame) {
		

		if (false == m_bFix)
		{
			Mouse_Move();
			Mouse_Fix();
		}
	}
	else {
		m_bFix = true;
	}
	
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);


	if (!m_bInGame) {
		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_W) & 0x80)
		{
			_vec3	vLook;
			memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

			_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeed;

			m_vEye += vLength;
			m_vAt += vLength;
		}

		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_S) & 0x80)
		{
			_vec3	vLook;
			memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

			_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeed;

			m_vEye -= vLength;
			m_vAt -= vLength;
		}

		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_D) & 0x80)
		{
			_vec3	vRight;
			memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

			_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeed;

			m_vEye += vLength;
			m_vAt += vLength;
		}

		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_A) & 0x80)
		{
			_vec3	vRight;
			memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

			_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeed;

			m_vEye -= vLength;
			m_vAt -= vLength;
		}

		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_TAB) & 0x80)
		{
			if (m_bCheck)
				return;

			m_bCheck = true;

			if (m_bFix)
				m_bFix = false;

			else
				m_bFix = true;
		}
		else
		{
			m_bCheck = false;
		}
		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_L) & 0x80) {
			if (m_bLCheck)
				return;

			m_bLCheck = true;
			m_bInGame = true;
			m_bFix = true;

			m_vEye = m_vDefaultEye;
			m_vAt = m_vDefaultAt;
			m_vUp = m_vDefaultUp;
		}
		else
		{
			m_bLCheck = false;
		}
	}
	else {
		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_L) & 0x80) {
			if (m_bLCheck)
				return;

			m_bLCheck = true;
			m_bInGame = false;
			m_bFix = false;
		}
		else
		{
			m_bLCheck = false;
		}
	}
	
	if (false == m_bFix)
		return;

}

void CDynamicCamera::Mouse_Move()
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);


	_long	dwMouseMove(0);

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;
		_matrix matRot;

		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_X))
	{
		_vec3	vUp{ 0.f, 1.f, 0.f };		

		_vec3	vLook = m_vAt - m_vEye;
		_matrix matRot;

		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

}

void CDynamicCamera::Mouse_Fix()
{
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);


}

void CDynamicCamera::On_Focus(const _vec3* _vFocus)
{
	if (m_bIsMoving) return;
	m_vAt = *_vFocus;
	_vec3 vEye = { _vFocus->x, _vFocus->y + 6.f, _vFocus->z - 5.f };

	m_vEye = vEye;
}

_bool CDynamicCamera::Move_To(const _vec3* _vPos)
{
	m_bIsMoving = true;
	const _float fMoveSpeed = 0.1f;

	_vec3 vToTarget = *_vPos - m_vEye;
	vToTarget.y = 0.f;

	_float fDist = D3DXVec3Length(&vToTarget);

	if (fDist <= 0.01f)
	{
		m_bIsMoving = false;
		return TRUE;
	}
	if (fDist < 0.3f)
	{
		_vec3 vOffset = *_vPos - m_vEye;
		vOffset.y = 0.f;

		m_vEye += vOffset;
		m_vAt += vOffset;

		m_bIsMoving = false;
		return TRUE;
	}

	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &vToTarget);
	_vec3 vMove = vDir * fMoveSpeed;

	m_vEye += vMove;
	m_vAt += vMove;

	return FALSE;
}

_bool CDynamicCamera::Move_To_And_Focus(const _vec3* _vPos)
{
	const _float fMoveSpeed = 0.1f;

	_vec3 vTargetEye = { _vPos->x, _vPos->y + 6.f, _vPos->z - 5.f };

	_vec3 vToTarget = vTargetEye - m_vEye;
	vToTarget.y = 0.f;

	_float fDist = D3DXVec3Length(&vToTarget);

	if (fDist < 0.3f)
	{
		m_vEye = vTargetEye;
		m_vAt = *_vPos;
		m_bIsMoving = false;
		return TRUE;
	}

	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &vToTarget);
	_vec3 vMove = vDir * fMoveSpeed;

	m_vEye += vMove;

	m_vAt = *_vPos;

	return FALSE;
}


void CDynamicCamera::Release_Focus()
{
	m_vAt = m_vDefaultAt;
	m_vEye = m_vDefaultEye;
	m_vUp = m_vDefaultUp;
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera* pCamera = new CDynamicCamera(pGraphicDev);
	pCamera->m_vDefaultEye = *pEye;
	pCamera->m_vDefaultAt = *pAt;
	pCamera->m_vDefaultUp = *pUp;

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("DynamicCamera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CDynamicCamera::Free()
{
	Engine::CCamera::Free();

}
