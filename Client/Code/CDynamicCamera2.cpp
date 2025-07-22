#include "pch.h"
#include "CDynamicCamera2.h"
#include "CDInputMgr.h"
#include "CTransform.h"
#include "CSelectGameSystem.h"

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
	m_fMoveSpeed = 10.f;

	return S_OK;
}

_int CDynamicCamera2::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);
	
	if (m_pTarget == nullptr)
		return 0;

	if (m_bFocus)
	{
		m_fElapsedTime += fTimeDelta;

		if (m_fElapsedTime < m_fFocusDuration) {
			if (m_bMove) {
				Cam_Move(fTimeDelta);
			}
			else {
				_vec3 vTargetEye = m_vFocusPos + m_vOffset;
				_vec3 vLook = (m_vAt - m_vEye);
				m_vEye = vTargetEye;
				m_vAt = vLook + vTargetEye;
			}
		}
		else {	
			if (m_bBack) {
				Cam_Back(fTimeDelta);
			}
			else {
				End_Focus();
			}
		}
		CSelectGameSystem::GetInstance()->Find_By_Euclidean(&m_vFocusPos, fTimeDelta);
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

void CDynamicCamera2::Set_MoveSpeed(_float _fMoveSpeed)
{
	m_fMoveSpeed = _fMoveSpeed;
}

void CDynamicCamera2::Focus(CGameObject* m_pGameObject, float _fDurationTime = 0.f, _bool _bMoveCam = false, _bool _bBackCam = false)
{
	m_pFocusTarget = m_pGameObject;
	CTransform* pTargetTransformCom = dynamic_cast<CTransform*>(m_pFocusTarget->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	pTargetTransformCom->Get_Info(INFO_POS, &m_vFocusPos);
	m_fFocusDuration = _fDurationTime;
	m_fElapsedTime = 0.f;
	m_bFocus = true;
	m_bMove = _bMoveCam;
	m_bBack = _bBackCam;
}

void CDynamicCamera2::Focus(_vec3 _vPos, float _fDurationTime = 0.f, _bool _bMoveCam = false, _bool _bBackCam = false)
{
	m_vFocusPos = _vPos;
	m_fFocusDuration = _fDurationTime;
	m_fElapsedTime = 0.f;
	m_bFocus = true;
	m_bMove = _bMoveCam;
	m_bBack = _bBackCam;
}

void CDynamicCamera2::Cam_Move(const _float _fTimeDelta)
{
	//������ 0.5�� �°� ��ƾ� �������� �̻��Ѵ����� �ȵ�
	m_vFocusPos.y = 0.5f;
	_vec3 vTargetEye = m_vFocusPos + m_vOffset;

	_vec3 vLookDir = m_vAt - m_vEye;
	_float fLookLen = D3DXVec3Length(&vLookDir);
	D3DXVec3Normalize(&vLookDir, &vLookDir);

	// y ������ �Ÿ� ���
	_vec3 vToTarget = vTargetEye - m_vEye;
	vToTarget.y = 0.f; // y�� ����
	_float fDist = D3DXVec3Length(&vToTarget);

	if (fDist < 0.3f)
	{
		m_vEye = vTargetEye;
		m_vAt = m_vEye + vLookDir * fLookLen;  // look ���� ����
		m_bMove = false;

		//Ÿ�� ������ �ϵ��ڵ� 
		CSelectGameSystem::GetInstance()->Do_Flip_Action();

		return;
	}

	// ���� �� �̵�
	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &vToTarget);
	_vec3 vMove = vDir * m_fMoveSpeed * _fTimeDelta;

	// �̵� ����
	m_vEye += vMove;
	m_vAt = m_vEye + vLookDir * fLookLen;  // look ���� ����
}

void CDynamicCamera2::Cam_Back(const _float _fTimeDelta)
{
	CTransform* _pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(ID_DYNAMIC, L"Com_Transform"));

	_vec3 vToOrigin;
	_pTargetTransform->Get_Info(INFO_POS, &vToOrigin);

	_vec3 vTargetEye = vToOrigin + m_vOffset;

	// ó�� Look ���� ��� (������ ����)
	_vec3 vLookDir = m_vAt - m_vEye;
	_float fLookLen = D3DXVec3Length(&vLookDir);
	D3DXVec3Normalize(&vLookDir, &vLookDir); // ����ȭ

	// y ������ �Ÿ� ���
	_vec3 vToTarget = vTargetEye - m_vEye;
	vToTarget.y = 0.f; // y�� ����
	_float fDist = D3DXVec3Length(&vToTarget);

	// ��ǥ ������ ���� �����߰ų� �̹� �Ѿ�ٸ� �ٷ� �����ϰ� ����
	if (fDist < 0.05f) // 0.3f ��� �� ���� ������ ���е��� �����ݴϴ�.
	{
		m_vEye = vTargetEye;
		m_vAt = m_vEye + vLookDir * fLookLen; // look ���� ����
		m_bMove = false;
		return;
	}

	// ���� �� �̵�
	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &vToTarget);

	// �̵��� �ִ� �Ÿ��� ����մϴ�.
	_float fMoveAmount = m_fMoveSpeed * _fTimeDelta;

	// ���� �Ÿ�(fDist)���� �̵��� �Ÿ��� ũ�ٸ� ���� �Ÿ���ŭ�� �̵��ϵ��� �����մϴ�.
	if (fMoveAmount > fDist)
	{
		fMoveAmount = fDist;
	}

	_vec3 vMove = vDir * fMoveAmount; // ���ѵ� �̵� �Ÿ� ����

	// �̵� ����
	m_vEye += vMove;
	m_vAt = m_vEye + vLookDir * fLookLen; // look ���� ����
}

void CDynamicCamera2::End_Focus()
{
	m_pFocusTarget = nullptr;
	m_fElapsedTime = 0.f;
	m_fFocusDuration = 0.f;
	m_bFocus = false;
	m_bMove = false;
	m_bBack = false;
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
