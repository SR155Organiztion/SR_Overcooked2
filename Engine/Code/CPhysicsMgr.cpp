#include "CPhysicsMgr.h"

IMPLEMENT_SINGLETON(CPhysicsMgr)

CPhysicsMgr::CPhysicsMgr()
{
}

CPhysicsMgr::~CPhysicsMgr()
{
}

void CPhysicsMgr::Calc_Bounding(CGameObject* _pGameObject, CTransform* pTrans, CVIBuffer* pVIB)
{
	const _matrix* matTargetWorld = pTrans->Get_World();
	_vec3 vCenter;
	pTrans->Get_Info(INFO::INFO_POS, &vCenter);

	IPhysics* pPhysics = dynamic_cast<IPhysics*>(_pGameObject);

}

void CPhysicsMgr::Calc_All_Bounding()
{
	
}

bool CPhysicsMgr::Check_AABB_Collision(IPhysics* _pDest, IPhysics* _pTarget)
{
	return (
		_pDest->Get_BoxMin()->x <= _pTarget->Get_BoxMax()->x 
		&& _pDest->Get_BoxMax()->x >= _pTarget->Get_BoxMin()->x 
		&& _pDest->Get_BoxMin()->y <= _pTarget->Get_BoxMax()->y 
		&& _pDest->Get_BoxMax()->y >= _pTarget->Get_BoxMin()->y 
		&& _pDest->Get_BoxMin()->z <= _pTarget->Get_BoxMax()->z 
		&& _pDest->Get_BoxMax()->z >= _pTarget->Get_BoxMin()->z
		);
}

void CPhysicsMgr::Update_Physics()
{
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

			if (Check_AABB_Collision(
				pDest
				, pTarget
			)) {
				if (pDest->Get_Opt()->applyKnockBack) {
					// TODO: 乘归 贸府
				}
				else {
					_vec3 vPos;
					pDestTransform->Get_Info(INFO::INFO_POS, &vPos);
				}

				if (pTarget->Get_Opt()->applyKnockBack) {
					// TODO: 乘归 贸府
				}
				else {
					_vec3 vPos;
					pTargetTransform->Get_Info(INFO::INFO_POS, &vPos);
				}
			}
		}
	}
}

void CPhysicsMgr::Free()
{
}
