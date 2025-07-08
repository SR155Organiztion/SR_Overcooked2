#include "CPhysicsMgr.h"
#include "CTransform.h"

IMPLEMENT_SINGLETON(CPhysicsMgr)

CPhysicsMgr::CPhysicsMgr()
{
}

CPhysicsMgr::~CPhysicsMgr()
{
	Free();
}

void CPhysicsMgr::Update_Physics(const _float& _fTimeDelta)
{

    // 초기화
    /*for (CGameObject* pGameObject : m_physicsList)
    {
        if (auto* pPhysics = dynamic_cast<IPhysics*>(pGameObject))
            pPhysics->Set_IsGround(false);
    }*/

    // 바운딩박스 계산
    for (CGameObject* pGameObject : m_physicsList)
    {
        auto* pPhysics = dynamic_cast<IPhysics*>(pGameObject);
        auto* pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        auto* pBuffer = dynamic_cast<CVIBuffer*>(pGameObject->Get_Component(ID_STATIC, L"Com_Buffer"));
        if (!pPhysics || !pTransform || !pBuffer) continue;

        if (!pPhysics->Get_Opt()->bApplyCollision) {
            _vec3 vZero = { 0.f, 0.f, 0.f };

            pPhysics->Set_BoundingBox(
                vZero,
                vZero,
                vZero,
                vZero
            );
            continue;
        }

        const _vec3& vPos = pTransform->m_vInfo[INFO_POS];
        const _vec3& vScale = pTransform->Get_Scale();
        _vec3 vExtent = (pBuffer->Get_MaxBox() - pBuffer->Get_MinBox()) * 0.5f;
        vExtent.x *= vScale.x; vExtent.y *= vScale.y; vExtent.z *= vScale.z;

        _vec3 vNextPos = vPos + (*pTransform->Get_Velocity() * _fTimeDelta);

        pPhysics->Set_BoundingBox(
            vPos - vExtent,
            vPos + vExtent,
            vNextPos - vExtent,
            vNextPos + vExtent
        );
    }

    // 던지기 처리
    for (auto iter = m_physicsList.begin(); iter != m_physicsList.end(); iter++) {
        IPhysics* pPhys = dynamic_cast<IPhysics*>(*iter);
        CTransform* pTrans =
            dynamic_cast<CTransform*>(
                (*iter)->Get_Component(
                    COMPONENTID::ID_DYNAMIC
                    , L"Com_Transform"
                )
                );

        if (pPhys->Get_Opt()->bFirstThrown) {
            _vec3 vVel = *pPhys->Get_ThrowDir() * pPhys->Get_ThrowSpeed();
            pTrans->Set_Velocity(vVel, _fTimeDelta);
            pPhys->Get_Opt()->bFirstThrown = false;
        }

        if (pPhys->Get_Opt()->bThrown) {
            _vec3 vCurrVelocity = *pTrans->Get_Velocity();

            if (D3DXVec3Length(&vCurrVelocity) <= 0.f) {
                _vec3 vZero = { 0.f, 0.f, 0.f };

                pPhys->Get_Opt()->bThrown = false;
                pTrans->Set_Velocity(vZero, _fTimeDelta);
                continue;
            }

            pTrans->Set_Velocity(
                vCurrVelocity * (pPhys->Get_Opt()->fDeceleration)
                , _fTimeDelta
            );
        }
    }

    // 중력 및 이동 적용
    for (CGameObject* pGameObject : m_physicsList)
    {
        auto* pPhysics = dynamic_cast<IPhysics*>(pGameObject);
        auto* pTransform = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));

        if (!pPhysics || !pTransform) continue;

        IPhysics::PHYSICS_OPT* pOption = pPhysics->Get_Opt();

        if (pOption->bApplyGravity 
            //&& !*pPhysics->Get_IsGround()
            )
        {
            const _float fGravity = 9.8f  * 0.25f;              // 중력 값 수정 여깁니다
            _float fElapsed = *pPhysics->Get_GravityElased() + _fTimeDelta;
            pPhysics->Set_GravityElapsed(fElapsed);

            _vec3 vFall = _vec3(0.f, -1.f, 0.f) * fGravity * _fTimeDelta;
            pTransform->Add_Velocity(vFall);
        }

        pTransform->Move_Velocity(_fTimeDelta);

        const _float fFloorY = 0.f;
        _float fBottomY = pTransform->m_vInfo[INFO_POS].y - (pTransform->Get_Scale().y * 0.5f);

        if (fBottomY < fFloorY)
        {
            _float fSelfH = pTransform->Get_Scale().y * 0.5f;
            pTransform->Set_Pos(pTransform->m_vInfo[INFO_POS].x, fSelfH, pTransform->m_vInfo[INFO_POS].z);
            pTransform->Get_Velocity()->y = 0.f;
            pPhysics->Set_IsGround(true);
            pPhysics->Set_GravityElapsed(0.f);
        }
    }

    // 반경 기반 밀어내기 + 굴림 회전 적용 + 주변 물체 탐지
    for (CGameObject* pGameObject : m_physicsList)
    {
        auto* pPhysicsDest = dynamic_cast<IPhysics*>(pGameObject);
        if (!pPhysicsDest->Get_Opt()->bApplyCollision) continue;

        auto* pTransformSelf = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        if (!pTransformSelf) continue;

        for (CGameObject* pTargetObject : m_physicsList)
        {
            if (pGameObject == pTargetObject) continue;

            auto* pPhysicsTarget = dynamic_cast<IPhysics*>(pTargetObject);
            if (!pPhysicsTarget->Get_Opt()->bApplyCollision) continue;
            
            auto* pTransformTarget = dynamic_cast<CTransform*>(pTargetObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
            if (!pPhysicsTarget || !pTransformTarget) continue;

            _vec3 vDiff = pTransformTarget->m_vInfo[INFO_POS] - pTransformSelf->m_vInfo[INFO_POS];
            _float fDist = D3DXVec3Length(&vDiff);
            // 반경 기반 탐지
            const _float fMinDetectRaduis = 2.f;

            if (fDist < fMinDetectRaduis && fDist > 0.001f) {
                pPhysicsDest->On_Detected(pTargetObject);
            }
            ///////////////////////

            // 반경 기반 밀어내기
            const _float fMinRadius = 1.5f;

            if (fDist < fMinRadius && fDist > 0.001f)
            {
                pPhysicsDest->On_Collision(pTargetObject);
                if (!pPhysicsTarget->Get_Opt()->bApplyKnockBack)
                    continue;

                _vec3 vDir = vDiff / fDist;
                _vec3 vNewPos = pTransformSelf->m_vInfo[INFO_POS] + vDir * fMinRadius;
                pTransformTarget->Set_Pos(vNewPos.x, pTransformTarget->m_vInfo[INFO_POS].y, vNewPos.z);
                // rolling opt
                if (pPhysicsTarget->Get_Opt()->bApplyRolling) {
                    const float fRollSpeed = D3DXToRadian(360.f);
                    const float fTimeStep = 0.016f;
                    float fRollAmount = fRollSpeed * fTimeStep;

                    pTransformTarget->m_vAngle.z += fRollAmount;
                }
                
            }
            ///////////////////////////
        }
    }

    // 충돌 처리
    /*for (auto itA = m_physicsList.begin(); itA != m_physicsList.end(); ++itA)
    {
        IPhysics* pPhysicsA = dynamic_cast<IPhysics*>(*itA);
        if (!pPhysicsA) continue;
        if (!pPhysicsA->Get_Opt()->bApplyCollision)  continue;

        CTransform* pTransformA = dynamic_cast<CTransform*>((*itA)->Get_Component(ID_DYNAMIC, L"Com_Transform"));

        if (!pTransformA) continue;

        auto itB = itA;
        ++itB;
        for (; itB != m_physicsList.end(); ++itB)
        {
            IPhysics* pPhysicsB = dynamic_cast<IPhysics*>(*itB);
            CTransform* pTransformB = dynamic_cast<CTransform*>((*itA)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
            if (!pPhysicsB) continue;

            if (
                Check_AABB_Collision(pPhysicsA, pPhysicsB) &&
                (!pPhysicsA->Get_Opt()->bApplyCollision
                    || !pPhysicsB->Get_Opt()->bApplyCollision)
                )
                pPhysicsA = pPhysicsA;

            if (Check_AABB_Collision(pPhysicsA, pPhysicsB))
            {
                Resolve_Collision(pPhysicsA, pPhysicsB, pTransformA);
            }
        }
    }*/
}


bool CPhysicsMgr::Check_AABB_Collision(IPhysics* _pPhys, IPhysics* _pOtherPhys)
{
    if (
        !_pPhys->Get_Opt()->bApplyCollision
        || !_pOtherPhys->Get_Opt()->bApplyCollision
        ) return false;

    _vec3* aMin = _pPhys->Get_NextMinBox();
    _vec3* aMax = _pPhys->Get_NextMaxBox();
    _vec3* bMin = _pOtherPhys->Get_NextMinBox();
    _vec3* bMax = _pOtherPhys->Get_NextMaxBox();

    if (IsSameBox(bMin, _pOtherPhys->Get_MinBox()) && IsSameBox(bMax, _pOtherPhys->Get_MaxBox())) {
        const float fEpsilon = 0.05f;
        bMin->x -= fEpsilon; bMax->x += fEpsilon;
        bMin->z -= fEpsilon; bMax->z += fEpsilon;
    }

    BOOL bCollides =
        (aMin->x <= bMax->x && aMax->x >= bMin->x) &&
        (aMin->y <= bMax->y && aMax->y >= bMin->y) &&
        (aMin->z <= bMax->z && aMax->z >= bMin->z);

    return bCollides;
}

bool CPhysicsMgr::Check_AABB_Collision_Actual(IPhysics* _pPhys, IPhysics* _pOtherPhys)
{
    _vec3* aMin = _pPhys->Get_MinBox();
    _vec3* aMax = _pPhys->Get_MaxBox();
    _vec3* bMin = _pOtherPhys->Get_MinBox();
    _vec3* bMax = _pOtherPhys->Get_MaxBox();

    BOOL bCollides =
        (aMin->x <= bMax->x && aMax->x >= bMin->x) &&
        (aMin->y <= bMax->y && aMax->y >= bMin->y) &&
        (aMin->z <= bMax->z && aMax->z >= bMin->z);

    return bCollides;
}


void CPhysicsMgr::Resolve_Collision(IPhysics* _pSelf, IPhysics* _pOther, CTransform* _pTransform)
{
    CGameObject* pOtherObj = nullptr;

    for (CGameObject* pCandidate : m_physicsList)
    {
        if (dynamic_cast<IPhysics*>(pCandidate) == _pOther)
        {
            pOtherObj = pCandidate;
            break;
        }
    }

    if (!pOtherObj) return;

    CTransform* pOtherTransform = dynamic_cast<CTransform*>(pOtherObj->Get_Component(ID_DYNAMIC, L"Com_Transform"));
    if (!pOtherTransform) return;

    _vec3 dir = _pTransform->m_vInfo[INFO_POS] - _pTransform->m_vPrevPos;
    _vec3* pVel = _pTransform->Get_Velocity();

    if (_pOther->Get_Opt()->bApplyKnockBack)
    {
        _vec3 pushDir = dir;
        D3DXVec3Normalize(&pushDir, &pushDir);

        _vec3 pushForce = pushDir * 300.0f;
    }


    if (abs(dir.y) > abs(dir.x) && abs(dir.y) > abs(dir.z))
    {
        _pTransform->m_vInfo[INFO_POS].y = _pTransform->m_vPrevPos.y;
        pVel->y = 0.f;
        _pSelf->Set_IsGround(true);
        _pSelf->Set_GravityElapsed(0.f);

        _pTransform->m_bBlocked[1] = true;

        if (dir.y < 0.f || pOtherTransform->m_vInfo[INFO_POS].y <= 0.01f)
        {
            _vec3 stationPos = pOtherTransform->m_vInfo[INFO_POS];
            _float stationH = pOtherTransform->Get_Scale().y * 0.5f;
            _float selfH = _pTransform->Get_Scale().y * 0.5f;

            _vec3 snapPos = stationPos;
            snapPos.y = 0.f + stationH + selfH;

            _pTransform->Set_Pos(snapPos.x, snapPos.y, snapPos.z);
            *pVel = _vec3(0.f, 0.f, 0.f);
        }
    }
    else
    {
        if (abs(dir.x) > abs(dir.z))
        {
            _pTransform->m_vInfo[INFO_POS].x = _pTransform->m_vPrevPos.x;
            pVel->x = 0.f;
            _pTransform->m_bBlocked[0] = true; // X방향 차단
        }
        else
        {
            _pTransform->m_vInfo[INFO_POS].z = _pTransform->m_vPrevPos.z;
            pVel->z = 0.f;
            _pTransform->m_bBlocked[2] = true; // Z방향 차단
        }
    }

    if (_pSelf->Get_Opt()->bApplyBouncing)
    {
        pVel->y *= -0.5f;
    }
    else if (_pSelf->Get_Opt()->bApplyRolling)
    {
        pVel->x *= _pSelf->Get_Opt()->fDeceleration;
        pVel->z *= _pSelf->Get_Opt()->fDeceleration;
    }
}

_bool CPhysicsMgr::IsSameBox(const _vec3* _pA, const _vec3* _pB, float epsilon)
{
    return fabsf(_pA->x - _pB->x) < epsilon &&
        fabsf(_pA->y - _pB->y) < epsilon &&
        fabsf(_pA->z - _pB->z) < epsilon;
}

_bool CPhysicsMgr::Check_AnyCollision(CTransform* _pTransform, const _vec3& _vTargetPos)
{
    if (!_pTransform)
        return false;
    CGameObject* pSelfObj = nullptr;
    for (CGameObject* pObj : m_physicsList)
    {
        CTransform* pCandidateTransform = dynamic_cast<CTransform*>(pObj->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        if (pCandidateTransform == _pTransform)
        {
            pSelfObj = pObj;
            break;
        }
    }
    if (!pSelfObj) return false;

    IPhysics* pSelfPhysics = dynamic_cast<IPhysics*>(pSelfObj);
    if (!pSelfPhysics) return false;

    CVIBuffer* pSelfBuffer = dynamic_cast<CVIBuffer*>(pSelfObj->Get_Component(ID_STATIC, L"Com_Buffer"));
    if (!pSelfBuffer) return false;

    _vec3 extent = (pSelfBuffer->Get_MaxBox() - pSelfBuffer->Get_MinBox()) * 0.5f;
    _vec3 selfMin = _vTargetPos - extent;
    _vec3 selfMax = _vTargetPos + extent;

    for (CGameObject* pOther : m_physicsList)
    {
        if (pOther == pSelfObj) continue;

        IPhysics* pOtherPhysics = dynamic_cast<IPhysics*>(pOther);
        if (!pOtherPhysics) continue;

        _vec3* otherMin = pOtherPhysics->Get_NextMinBox();
        _vec3* otherMax = pOtherPhysics->Get_NextMaxBox();

        if (selfMin.x <= otherMax->x && selfMax.x >= otherMin->x &&
            selfMin.y <= otherMax->y && selfMax.y >= otherMin->y &&
            selfMin.z <= otherMax->z && selfMax.z >= otherMin->z)
        {
            return true;
        }
    }

    return false;
}



void CPhysicsMgr::Free()
{
	for (CGameObject* obj : m_physicsList) {
		Safe_Release<CGameObject*>(obj);
	}
}
