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

	_vec3 vNormal = { 0, 0, 0 };

	_float fX = min(vDestMax->x, vTargetMax->x) - max(vDestMin->x, vTargetMin->x);
	_float fY = min(vDestMax->y, vTargetMax->y) - max(vDestMin->y, vTargetMin->y);
	_float fZ = min(vDestMax->z, vTargetMax->z) - max(vDestMin->z, vTargetMin->z);

	_float fMinOverlap = min(fX, fZ);

	if (fMinOverlap == fX) vNormal = { 1, 0, 0 };
	else                      vNormal = { 0, 0, 1 };

	_vec3 vCenterDest = (*vDestMin + *vDestMax) * 0.5f;
	_vec3 vCenterTarget = (*vTargetMin + *vTargetMax) * 0.5f;
	_vec3 vDir = vCenterDest - vCenterTarget;

	if (fMinOverlap == fX)
		vNormal = (vCenterDest.x > vCenterTarget.x) ? _vec3{ 1,0,0 } : _vec3{ -1,0,0 };
	else if (fMinOverlap == fY)
		vNormal = (vCenterDest.y > vCenterTarget.y) ? _vec3{ 0,1,0 } : _vec3{ 0,-1,0 };
	else if (fMinOverlap == fZ)
		vNormal = (vCenterDest.z > vCenterTarget.z) ? _vec3{ 0,0,1 } : _vec3{ 0,0,-1 };


	D3DXVec3Normalize(&vDir, &vDir);

	if (D3DXVec3Dot(&vNormal, &vDir) < 0)
		vNormal *= -1;

	return vNormal;
}

void CPhysicsMgr::Calc_SpeedVector()
{
}

void CPhysicsMgr::Calc_RotateVector()
{
}

void CPhysicsMgr::Apply_Rotate(IPhysics* _pPhys, CTransform* _pTransform, _float _fTimeDelta)
{
	const _vec3& vVel = *_pPhys->Get_ReflectionVelocity();
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

	float fRotSpeedMultiplier = 10.f;
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
	_vec3 vDestVel = _pDestTrans->Get_Velocity();
	_vec3 vTargetVel = _pTargetTrans->Get_Velocity();
	_vec3 vRelativeVel = vDestVel - vTargetVel;
	float maxSpeed = 1.f;

	_vec3 vReflected = Reflect_Vector(vRelativeVel, _vNormal);

	if (D3DXVec3Length(&vReflected) > maxSpeed)
	{
		D3DXVec3Normalize(&vReflected, &vReflected);
		vReflected *= maxSpeed;
	}

	if (vReflected.y < 0.f)
		vReflected.y = 0.f;

	vReflected *= _pPhys->Get_Opt()->fDeceleration;

	_pDestTrans->Set_Velocity(vReflected);

	return vReflected;
}

void CPhysicsMgr::Reflect_Velocity_GroundBounce(IPhysics* _pPhys, CTransform* _pTrans)
{
	_vec3 vVel = _pTrans->Get_Velocity();

	if (vVel.y > -0.1f) {
		// 충분히 느리면 멈추기
		vVel.y = 0.f;
		_pPhys->Set_IsGround(true);
		_pPhys->Set_GravityElapsed(0.f);
	}
	else {
		// 반사 (바닥 normal: {0, 1, 0})
		_vec3 vReflected = Reflect_Vector(vVel, _vec3{ 0.f, 1.f, 0.f });

		// 감속 적용
		vReflected *= _pPhys->Get_Opt()->fDeceleration;

		// 저장
		_pTrans->Set_Velocity(vReflected);
		_pPhys->Set_GravityElapsed(0.f); // 다시 위로 튕기기 시작하므로 초기화
	}
}


void CPhysicsMgr::Deceleration_Velocity(IPhysics* _pPhys, _vec3* _vReflectVec)
{
	*_vReflectVec *= _pPhys->Get_Opt()->fDeceleration;
}

void CPhysicsMgr::Apply_Gravity(CTransform* _pTrans, _float* _pGravityElapsed, _float fDeltaTime)
{
	*_pGravityElapsed += GRAVITY * fDeltaTime;
	_vec3 vVel = { 0.f, *_pGravityElapsed, 0.f };
	_pTrans->Move_Pos(&vVel, 1.f, fDeltaTime);
}

void CPhysicsMgr::Update_Physics(const _float& fTimeDelta)
{
	Calc_All_Bounding();

	for (CGameObject* pDestObj : m_physicsList) {
		CTransform* pDestTransform = Get_TransformFromGameObject(pDestObj);
		CVIBuffer* pDestVIBuffer = Get_ViBufferFromGameObject(pDestObj);

		IPhysics* pDest = dynamic_cast<IPhysics*>(pDestObj);

		// 중력 적용 로직
		if (pDest->Get_Opt()->bApplyGravity) {
			_vec3 vCurrPos;
			pDestTransform->Get_Info(INFO::INFO_POS, &vCurrPos);

			_float fHalfHeight = pDestVIBuffer->Get_Height() * pDestTransform->Get_Scale().y * 0.5f;
			_float fStandardY = vCurrPos.y - fHalfHeight;

			if (fStandardY > 0) {
				pDest->Set_IsGround(false);
				Apply_Gravity(pDestTransform, pDest->Get_GravityElased(), fTimeDelta);
			}
			else if (fStandardY <= 0) {
				pDest->Set_IsGround(true);

				if (pDest->Get_Opt()->bApplyBouncing) {
					Reflect_Velocity_GroundBounce(pDest, pDestTransform);
					_vec3 vBounceVel = pDestTransform->Get_Velocity();
					pDestTransform->Move_Pos(&vBounceVel, 1.f, fTimeDelta);
					pDestTransform->Set_Velocity({ 0.f, 0.f, 0.f });
				}
			}
		}
		else {
			pDest->Set_IsGround(true);
			pDest->Set_GravityElapsed(0.f);
		}
		// 중력 적용 종료

		for (CGameObject* pTargetObj : m_physicsList) {
			if (pDestObj == pTargetObj) continue;
			CTransform* pTargetTransform = Get_TransformFromGameObject(pTargetObj);
			CVIBuffer* pTargetVIBuffer = Get_ViBufferFromGameObject(pTargetObj);

			IPhysics* pTarget = dynamic_cast<IPhysics*>(pTargetObj);

			bool bCollision = pDest->Get_Opt()->bApplyKnockBack || pTarget->Get_Opt()->bApplyKnockBack ? Check_AABB_Collision(pDest, pTarget) : Check_AABB_Collision_Predict(pDest, pTarget);
			// bApplyKnockBack이 true이면 AABB충돌 아니면 미리 갈곳을 예측해서 AABB충돌
			if (bCollision) {
				if (pDest->Get_Opt()->bApplyKnockBack) {
					// 튕겨나가는 로직
					_vec3 vNormal = Calc_ContactDir(pDest, pTarget);
					_vec3 vReflected = Reflect_Velocity(pDest, pDestTransform, pTargetTransform, vNormal, fTimeDelta);

					pDest->Set_CollisionDir(&vNormal);
					pDest->Set_ReflectionVelocity(&vReflected);
				}
				else {
					Block_Move(pDestTransform);
				}

				if (pTarget->Get_Opt()->bApplyKnockBack) {
					// 튕겨나가는 로직
					_vec3 vNormal = Calc_ContactDir(pDest, pTarget);
					_vec3 vReflected = Reflect_Velocity(pTarget, pTargetTransform, pDestTransform, -vNormal, fTimeDelta);

					pDest->Set_CollisionDir(&vNormal);
					pDest->Set_ReflectionVelocity(&vReflected);
				}
				else {
					Block_Move(pTargetTransform);
				}
			}

			// 충돌후 움직이는 로직
			if (pTarget->Get_Opt()->bApplyKnockBack) {
				// 튕겨나가는 로직
				pTargetTransform->Move_Pos(
					pTarget->Get_ReflectionVelocity(), pDest->Get_Opt()->fReflectSpeed, fTimeDelta
				);
				Apply_Rotate(pTarget, pTargetTransform, fTimeDelta);
				Deceleration_Velocity(pTarget, pTarget->Get_ReflectionVelocity());
			}
		}

		if (pDest->Get_Opt()->bApplyKnockBack) {

			pDestTransform->Move_Pos(
				pDest->Get_ReflectionVelocity(), pDest->Get_Opt()->fReflectSpeed, fTimeDelta
			);
			Apply_Rotate(pDest, pDestTransform, fTimeDelta);
			Deceleration_Velocity(pDest, pDest->Get_ReflectionVelocity());
		}
	}
}

void CPhysicsMgr::Free()
{
	for (CGameObject* obj : m_physicsList) {
		Safe_Release<CGameObject*>(obj);
	}
}
