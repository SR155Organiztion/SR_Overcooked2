#include "CTransform.h"
#include "CPhysicsMgr.h"

CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform& rhs) 
	: CComponent(rhs), m_vScale(rhs.m_vScale), m_vAngle(rhs.m_vAngle)
{
	for (_uint i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_matWorld = rhs.m_matWorld;
}

CTransform::~CTransform()
{
}

void CTransform::Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3 dir = *pDir;

	if (m_bBlocked[0]) dir.x = 0.f;
	if (m_bBlocked[1]) dir.y = 0.f;
	if (m_bBlocked[2]) dir.z = 0.f;

	_vec3 vOffset = dir * fSpeed * fTimeDelta;
	_vec3 vTarget = m_vInfo[INFO_POS] + vOffset;

	if (!CPhysicsMgr::GetInstance()->Check_AnyCollision(this, vTarget))
	{
		m_vPrevPos = m_vInfo[INFO_POS];
		m_vInfo[INFO_POS] = vTarget;
		m_vNextPos = m_vInfo[INFO_POS];
	}
}

void CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;


	_matrix	matRot = *Compute_LookAtTarget(pTargetPos);

	_matrix	matScale, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

const _matrix* CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	_vec3	vDir = *pTargetPos - m_vInfo[INFO_POS];
	//D3DXVec3Normalize(&vDir, &vDir);

	//_vec3	vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);

	_vec3	vAxis;
	//D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);

	_vec3	vUp;
	//D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]);

	//_float	fDot = D3DXVec3Dot(&vDir, &vUp);

	//_float	fAngle = acosf(fDot);

	_matrix	matRot;
	// D3DXMatrixRotationAxis(&matRot, &vAxis, fAngle);


	return D3DXMatrixRotationAxis(&matRot, 
								D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir),
								acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), 
												  D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int CTransform::Update_Component(const _float& fTimeDelta)
{
	m_bBlocked[0] = false;
	m_bBlocked[1] = false;
	m_bBlocked[2] = false;
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기 변환

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	// 회전 변환

	_matrix		matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_uint i = 0; i < INFO_END; ++i)
	{
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
	}

	return 0;
}

void CTransform::LateUpdate_Component()
{

}

CComponent* CTransform::Clone()
{
	return new CTransform(*this);
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pTransform = new CTransform(pGraphicDev);

	if (FAILED(pTransform->Ready_Transform()))
	{
		Safe_Release(pTransform);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pTransform;
}

void CTransform::Free()
{
	CComponent::Free();

}
