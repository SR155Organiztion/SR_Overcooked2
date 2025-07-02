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

_vec3 CPhysicsMgr::Calc_ContactDir(IPhysics* _pDest, IPhysics* _pTarget)
{
	const _vec3* vDestMin	= _pDest->Get_MinBox();
	const _vec3* vDestMax	= _pDest->Get_MaxBox();
	const _vec3* vTargetMin = _pTarget->Get_MinBox();
	const _vec3* vTargetMax = _pTarget->Get_MaxBox();

	_vec3 vDestCenter = (*vDestMin + *vDestMax) * 0.5f;
	_vec3 vTargetCenter = (*vTargetMin + *vTargetMax) * 0.5f;

	// 두 물체의 중심 벡터의 차를 구해 접촉 방향을 구한다.
	_vec3 vDir = vTargetCenter - vDestCenter;
	D3DXVec3Normalize(&vDir, &vDir);

	return vDir;
}

void CPhysicsMgr::Calc_SpeedVector()
{
}

void CPhysicsMgr::Calc_RotateVector()
{
}

void CPhysicsMgr::Apply_Rotate()
{
}

_vec3 CPhysicsMgr::Reflect_Vector(const _vec3 vVelocity, const _vec3 vNormal)
{
	_vec3 vNorm;
	D3DXVec3Normalize(&vNorm, &vNormal);

	float dot = D3DXVec3Dot(&vVelocity, &vNorm);

	_vec3 vReflected = vVelocity - 2.f * dot * vNorm;

	return vReflected;
}

_vec3 CPhysicsMgr::Reflect_Velocity(IPhysics* _pPhys, _vec3 _vNormal)
{
	_vec3 vVel = _vNormal * _pPhys->Get_Opt()->fReflectSpeed;
	_vec3 vReflected = Reflect_Vector(vVel, _vNormal);
	//vReflected.y = 0.f;
	if (vReflected.y < 0) vReflected.y = 0;
	Deceleration_Velociy(_pPhys, &vReflected);

	return vReflected;
}

void CPhysicsMgr::Deceleration_Velociy(IPhysics* _pPhys, _vec3* _vReflectVec)
{
	*_vReflectVec *= _pPhys->Get_Opt()->fDeceleration;
}

void CPhysicsMgr::Update_Physics(const _float& fTimeDelta)
{
	Calc_All_Bounding();

	for (CGameObject* pDestObj : m_physicsList) {
		CTransform* pDestTransform
			= dynamic_cast<CTransform*>(
				pDestObj->Get_Component(
						COMPONENTID::ID_DYNAMIC, L"Com_Transform"
					)
				);

		CVIBuffer* pDestVIBuffer
			= dynamic_cast<CVIBuffer*>(
				pDestObj->Get_Component(
						COMPONENTID::ID_STATIC, L"Com_Buffer"
					)
				);

		for (CGameObject* pTargetObj : m_physicsList) {
			if (pDestObj == pTargetObj) continue;

			IPhysics* pDest = dynamic_cast<IPhysics*>(pDestObj);
			IPhysics* pTarget = dynamic_cast<IPhysics*>(pTargetObj);

			CTransform* pTargetTransform
				= dynamic_cast<CTransform*>(
					pTargetObj->Get_Component(
							COMPONENTID::ID_DYNAMIC, L"Com_Transform"
						)
					);

			CVIBuffer* pTargetVIBuffer
				= dynamic_cast<CVIBuffer*>(
						pDestObj->Get_Component(
							COMPONENTID::ID_STATIC, L"Com_Buffer"
						)
					);

			if (Check_AABB_Collision_Predict(
				pDest
				, pTarget
			)) {
				if (pDest->Get_Opt()->bApplyKnockBack) {
					// TODO: 넉백 처리
					_vec3 vNormal = Calc_ContactDir(pDest, pTarget);
					_vec3 vReflected = Reflect_Velocity(pDest, vNormal);

					pDest->Set_CollisionDir(&vNormal);
					pDest->Set_ReflectionVelocity(&vReflected);
				}
				else {
					Block_Move(pDestTransform);
				}

				if (pTarget->Get_Opt()->bApplyKnockBack) {
					// TODO: 넉백 처리
					_vec3 vNormal = Calc_ContactDir(pDest, pTarget);
					_vec3 vReflected = Reflect_Velocity(pTarget, -vNormal);

					pDest->Set_CollisionDir(&vNormal);
					pDest->Set_ReflectionVelocity(&vReflected);
				}
				else {
					Block_Move(pTargetTransform);
				}
			}

			if (pTarget->Get_Opt()->bApplyKnockBack) {
				
				pTargetTransform->Move_Pos(
					pTarget->Get_ReflectionVelociy(), 5.f, fTimeDelta
				);
				Deceleration_Velociy(pTarget, pTarget->Get_ReflectionVelociy());
			}
		}
	}
}

void CPhysicsMgr::Free()
{
	for (CGameObject* obj : m_physicsList) {
		Safe_Release<CGameObject*>(obj);
	}
}
