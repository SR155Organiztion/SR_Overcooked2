#include "CPhysicsMgr.h"

IMPLEMENT_SINGLETON(CPhysicsMgr)

CPhysicsMgr::CPhysicsMgr()
{
}

CPhysicsMgr::~CPhysicsMgr()
{
	Free();
}

void CPhysicsMgr::Calc_Bounding(CGameObject* _pGameObject, CTransform* pTrans, CVIBuffer* pVIB)
{
	const _matrix* pMatTargetWorld = pTrans->Get_World();

	_vec3 vMin = pVIB->Get_MinBox();
	_vec3 vMax = pVIB->Get_MaxBox();
	_vec3 vNextPos = pTrans->Get_NextInfo();

	_vec3 localCorners[8] = {
		{ vMin.x, vMin.y, vMin.z },
		{ vMax.x, vMin.y, vMin.z },
		{ vMax.x, vMax.y, vMin.z },
		{ vMin.x, vMax.y, vMin.z },
		{ vMin.x, vMin.y, vMax.z },
		{ vMax.x, vMin.y, vMax.z },
		{ vMax.x, vMax.y, vMax.z },
		{ vMin.x, vMax.y, vMax.z },
	};

	_vec3 worldCorners[8];
	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&worldCorners[i], &localCorners[i], pMatTargetWorld);

	_vec3 worldMin = worldCorners[0];
	_vec3 worldMax = worldCorners[0];

	for (int i = 1; i < 8; ++i)
	{
		worldMin.x = min(worldMin.x, worldCorners[i].x);
		worldMin.y = min(worldMin.y, worldCorners[i].y);
		worldMin.z = min(worldMin.z, worldCorners[i].z);

		worldMax.x = max(worldMax.x, worldCorners[i].x);
		worldMax.y = max(worldMax.y, worldCorners[i].y);
		worldMax.z = max(worldMax.z, worldCorners[i].z);
	}

	IPhysics* pPhysics = dynamic_cast<IPhysics*>(_pGameObject);
	_vec3 vCurrPos;
	pTrans->Get_Info(INFO::INFO_POS, &vCurrPos);
	pPhysics->Set_BoundingBox(
		worldMin, worldMax,
		worldMin + (vNextPos - vCurrPos),
		worldMax + (vNextPos - vCurrPos)
	);

}

void CPhysicsMgr::Calc_All_Bounding()
{
	for (CGameObject* cObj : m_physicsList) {
		CTransform* pTransform
			= dynamic_cast<CTransform*>(
					cObj->Get_Component(
						COMPONENTID::ID_DYNAMIC, L"Com_Transform"
					)
				);

		CVIBuffer* pVIBuffer
			= dynamic_cast<CVIBuffer*>(
					cObj->Get_Component(
						COMPONENTID::ID_STATIC, L"Com_Buffer"
					)
				);

		Calc_Bounding(cObj, pTransform, pVIBuffer);
	}
}

bool CPhysicsMgr::Check_AABB_Collision(IPhysics* _pDest, IPhysics* _pTarget)
{
	if (!_pDest || !_pTarget)
		return false;

	const _vec3* vDestMin = _pDest->Get_MinBox();
	const _vec3* vDestMax = _pDest->Get_MaxBox();

	const _vec3* vTargetMin = _pTarget->Get_MinBox();
	const _vec3* vTargetMax = _pTarget->Get_MaxBox();

	bool bOverlapX = (vDestMin->x <= vTargetMax->x && vDestMax->x >= vTargetMin->x);
	bool bOverlapY = (vDestMin->y <= vTargetMax->y && vDestMax->y >= vTargetMin->y);
	bool bOverlapZ = (vDestMin->z <= vTargetMax->z && vDestMax->z >= vTargetMin->z);

	return (bOverlapX && bOverlapY && bOverlapZ);
}

bool CPhysicsMgr::Check_AABB_Collision_Predict(IPhysics* _pDest, IPhysics* _pTarget)
{
	if (!_pDest || !_pTarget)
		return false;

	const _vec3* vDestMin = _pDest->Get_NextMinBox();
	const _vec3* vDestMax = _pDest->Get_NextMaxBox();

	const _vec3* vTargetMin = _pTarget->Get_MinBox();
	const _vec3* vTargetMax = _pTarget->Get_MaxBox();

	bool bOverlapX = (vDestMin->x <= vTargetMax->x && vDestMax->x >= vTargetMin->x);
	bool bOverlapY = (vDestMin->y <= vTargetMax->y && vDestMax->y >= vTargetMin->y);
	bool bOverlapZ = (vDestMin->z <= vTargetMax->z && vDestMax->z >= vTargetMin->z);

	return (bOverlapX && bOverlapY && bOverlapZ);
}

void CPhysicsMgr::Block_Move(CTransform* _pTrans)
{
	_vec3 vPos;
	_pTrans->Get_Info(INFO::INFO_POS, &vPos);
	_pTrans->Set_Pos(vPos.x, vPos.y, vPos.z);
}

CTransform* CPhysicsMgr::Get_TransformFromGameObject(CGameObject* _pGameObject)
{
	CTransform* pTransform
		= dynamic_cast<CTransform*>(
				_pGameObject->Get_Component(
					COMPONENTID::ID_DYNAMIC, L"Com_Transform"
				)
			);
	return pTransform;
}

CVIBuffer* CPhysicsMgr::Get_ViBufferFromGameObject(CGameObject* _pGameObject)
{
	CVIBuffer* pVIBuffer
		= dynamic_cast<CVIBuffer*>(
				_pGameObject->Get_Component(
					COMPONENTID::ID_STATIC, L"Com_Buffer"
				)
			);
	return pVIBuffer;
}

_vec3 CPhysicsMgr::Calc_ContactDir(IPhysics* _pDest, IPhysics* _pTarget)
{
	const _vec3* vDestMin = _pDest->Get_MinBox();
	const _vec3* vDestMax = _pDest->Get_MaxBox();
	const _vec3* vTargetMin = _pTarget->Get_MinBox();
	const _vec3* vTargetMax = _pTarget->Get_MaxBox();

	_vec3 vCenterDest = (*vDestMin + *vDestMax) * 0.5f;
	_vec3 vCenterTarget = (*vTargetMin + *vTargetMax) * 0.5f;
	_vec3 vDir = vCenterDest - vCenterTarget;

	_float fX = min(vDestMax->x, vTargetMax->x) - max(vDestMin->x, vTargetMin->x);
	_float fZ = min(vDestMax->z, vTargetMax->z) - max(vDestMin->z, vTargetMin->z);

	// 축별 방향 결정
	_vec3 vNormal = {
		(vDir.x >= 0.f ? 1.f : -1.f) * fX,
		0.f,
		(vDir.z >= 0.f ? 1.f : -1.f) * fZ
	};

	D3DXVec3Normalize(&vNormal, &vNormal);
	return vNormal;
}

_vec3 CPhysicsMgr::Calc_SeparationVector(IPhysics* pA, IPhysics* pB, const _vec3& vNormal)
{
	const _vec3* minA = pA->Get_MinBox();
	const _vec3* maxA = pA->Get_MaxBox();
	const _vec3* minB = pB->Get_MinBox();
	const _vec3* maxB = pB->Get_MaxBox();

	_vec3 overlap;
	overlap.x = min(maxA->x, maxB->x) - max(minA->x, minB->x);
	overlap.y = min(maxA->y, maxB->y) - max(minA->y, minB->y);
	overlap.z = min(maxA->z, maxB->z) - max(minA->z, minB->z);

	if (overlap.x <= overlap.y && overlap.x <= overlap.z)
		return _vec3{ vNormal.x * overlap.x, 0, 0 };
	else if (overlap.y <= overlap.x && overlap.y <= overlap.z)
		return _vec3{ 0, vNormal.y * overlap.y, 0 };
	else
		return _vec3{ 0, 0, vNormal.z * overlap.z };
}


void CPhysicsMgr::Calc_SpeedVector()
{
}

void CPhysicsMgr::Calc_RotateVector()
{
}

void CPhysicsMgr::Apply_Rotate(IPhysics* _pPhys, CTransform* _pTransform, _float _fTimeDelta)
{
	const _vec3& vVel = *_pTransform->Get_Velocity();
	float fSpeed = D3DXVec3Length(&vVel);

	if (fSpeed < 0.001f)
		return;

	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &vVel);

	_vec3 vAxis;
	_vec3 vUp = { 0, 1, 0 };
	D3DXVec3Cross(&vAxis, &vDir, &vUp);
	vAxis.x = 0.0f;

	if (D3DXVec3LengthSq(&vAxis) < 0.0001f)
		return;

	D3DXVec3Normalize(&vAxis, &vAxis);

	float fRotSpeedMultiplier = 40.f;
	float fRotAngle = fSpeed * _fTimeDelta * fRotSpeedMultiplier;

	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &vAxis, fRotAngle);

	_matrix matWorld = *_pTransform->Get_World();
	matWorld = matRot * matWorld;
	_pTransform->Set_World(&matWorld);
}

_vec3 CPhysicsMgr::Reflect_Vector(const _vec3 vVelocity, const _vec3 vNormal)
{
	_vec3 vNorm;
	D3DXVec3Normalize(&vNorm, &vNormal);

	_float fDot = D3DXVec3Dot(&vVelocity, &vNorm);
	_vec3 vReflected = vVelocity - 2.f * fDot * vNorm;
	vReflected.x *= -1;

	return vReflected;
}

_vec3 CPhysicsMgr::Reflect_Velocity(
	IPhysics* _pPhys, CTransform* _pDestTrans
	, CTransform* _pTargetTrans, _vec3 _vNormal
	, _float _fDeltaTime
)
{
	_vec3 vDestVel = *_pDestTrans->Get_Velocity();
	_vec3 vTargetVel = *_pTargetTrans->Get_Velocity();
	_vec3 vRelativeVel = vDestVel - vTargetVel;
	_vec3 vReflected = Reflect_Vector(vRelativeVel, _vNormal);

	if (vReflected.y < 0.f)
		vReflected.y = 0.f;

	vReflected *= _pPhys->Get_Opt()->fDeceleration;
	return vReflected;
}

void CPhysicsMgr::Reflect_Velocity_GroundBounce(IPhysics* _pPhys, CTransform* _pTrans)
{
	_vec3 vVel = *_pTrans->Get_Velocity();

	_vec3 vReflected = Reflect_Vector(vVel, _vec3{ 0.f, 1.f, 0.f });

	// 감속 적용
	vReflected *= _pPhys->Get_Opt()->fDeceleration;

	_pPhys->Set_GravityElapsed(0.f);

}


void CPhysicsMgr::Deceleration_Velocity(IPhysics* _pPhys, _vec3* _vReflectVec)
{
	*_vReflectVec *= _pPhys->Get_Opt()->fDeceleration;
}

void CPhysicsMgr::Apply_Gravity(CTransform* _pTrans, _float* _pGravityElapsed, _float fDeltaTime)
{
	*_pGravityElapsed += GRAVITY * fDeltaTime;
	_vec3 vVel = { 0.f, *_pGravityElapsed, 0.f };
	_pTrans->Add_Velocity(vVel);
}

void CPhysicsMgr::Update_Physics(const _float& fTimeDelta)
{
	Calc_All_Bounding();

	// 중력 처리
	for (CGameObject* pObj : m_physicsList)
	{
		CTransform* pTrans = Get_TransformFromGameObject(pObj);
		CVIBuffer* pVIB = Get_ViBufferFromGameObject(pObj);
		IPhysics* pPhys = dynamic_cast<IPhysics*>(pObj);

		if (pPhys->Get_Opt()->bApplyGravity)
		{
			_vec3 vCurrPos;
			pTrans->Get_Info(INFO::INFO_POS, &vCurrPos);
			_float fHalfHeight = pVIB->Get_Height() * pTrans->Get_Scale().y * 0.5f;
			_float fStandardY = vCurrPos.y - fHalfHeight;

			if (fStandardY > 0)
			{
				pPhys->Set_IsGround(false);
				Apply_Gravity(pTrans, pPhys->Get_GravityElased(), fTimeDelta);
			}
			else
			{
				pPhys->Set_IsGround(true);
				pTrans->Set_Pos(vCurrPos.x, fHalfHeight, vCurrPos.z);
				_vec3 vVel = *pTrans->Get_Velocity();
				pTrans->Set_Velocity({ vVel.x, 0.f, vVel.z });

				if (pPhys->Get_Opt()->bApplyBouncing)
				{
					Reflect_Velocity_GroundBounce(pPhys, pTrans);
				}

				pPhys->Set_GravityElapsed(0.f);
			}
		}
		else
		{
			pPhys->Set_IsGround(true);
			pPhys->Set_GravityElapsed(0.f);
		}
	}

	// 충돌 처리
	for (CGameObject* pDestObj : m_physicsList)
	{
		CTransform* pDestTrans = Get_TransformFromGameObject(pDestObj);
		IPhysics* pDest = dynamic_cast<IPhysics*>(pDestObj);

		for (CGameObject* pTargetObj : m_physicsList)
		{
			if (pDestObj == pTargetObj) continue;

			CTransform* pTargetTrans = Get_TransformFromGameObject(pTargetObj);
			IPhysics* pTarget = dynamic_cast<IPhysics*>(pTargetObj);

			bool bWillCollide = Check_AABB_Collision_Predict(pDest, pTarget);
			bool bIsColliding = Check_AABB_Collision(pDest, pTarget);

			if (!(bWillCollide || bIsColliding)) continue;

			_vec3 vNormal = Calc_ContactDir(pDest, pTarget);

			_vec3 vSeparation = Calc_SeparationVector(pDest, pTarget, vNormal);
			pDestTrans->Add_Pos(vSeparation);

			if (pDest->Get_Opt()->bApplyKnockBack)
			{
				_vec3 vReflected = Reflect_Velocity(pDest, pDestTrans, pTargetTrans, vNormal, fTimeDelta);
				pDestTrans->Set_Velocity(vReflected);
			}
			else
			{
				Block_Move(pDestTrans);
			}

			if (pTarget->Get_Opt()->bApplyKnockBack)
			{
				_vec3 vReflected = Reflect_Velocity(pTarget, pTargetTrans, pDestTrans, -vNormal, fTimeDelta);
				pTargetTrans->Set_Velocity(vReflected);
			}
			else
			{
				Block_Move(pTargetTrans);
			}
		}
	}

	// 위치 이동, 회전, 감속 처리
	for (CGameObject* pObj : m_physicsList)
	{
		CTransform* pTrans = Get_TransformFromGameObject(pObj);
		IPhysics* pPhys = dynamic_cast<IPhysics*>(pObj);

		if (pPhys->Get_Opt()->bApplyKnockBack || pPhys->Get_Opt()->bApplyGravity)
		{
			pTrans->Move_Velocity(fTimeDelta);
			Apply_Rotate(pPhys, pTrans, fTimeDelta);
			Deceleration_Velocity(pPhys, pTrans->Get_Velocity());
		}
	}

	Calc_All_Bounding();
}


void CPhysicsMgr::Free()
{
	for (CGameObject* obj : m_physicsList) {
		Safe_Release<CGameObject*>(obj);
	}
}
