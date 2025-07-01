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

void CPhysicsMgr::Update_Physics()
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
						pDestObj->Get_Component(
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
				if (pDest->Get_Opt()->applyKnockBack) {
					// TODO: 乘归 贸府
				}
				else {
					Block_Move(pDestTransform);
				}

				if (pTarget->Get_Opt()->applyKnockBack) {
					// TODO: 乘归 贸府
				}
				else {
					Block_Move(pTargetTransform);

				}
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
