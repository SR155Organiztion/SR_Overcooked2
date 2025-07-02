#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "IPhysics.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "CTransform.h"
#include "CVIBuffer.h"

BEGIN(Engine)

/**
* @file    CPhysicsMgr.h
* @date    2025-07-02
* @author  안준형
* @brief   충돌 매니저
* @details IPhysics를 상속받은 객체들에 한해 충돌 처리.
*/
class ENGINE_DLL CPhysicsMgr : public CBase
{
	DECLARE_SINGLETON(CPhysicsMgr)
private:
	explicit CPhysicsMgr();
	virtual ~CPhysicsMgr();

private:
	list<CGameObject*> m_physicsList;

public:
	/**
	* @brief 충돌 리스트에 객체를 추가. Layer단에서 처리함.
	* @param _pPhysics - 리스트에 추가할 충돌 객체
	*/
	void Add_PhysicsList(CGameObject* _pPhysics) {
		_pPhysics->AddRef();
		m_physicsList.push_back(_pPhysics);
	}

	/**
	* @brief 충돌 리스트에서 객체를 삭제.
	* @param _pPhysics - 삭제할 충돌 객체
	*/
	void Delete_PhysicsList(CGameObject* _pPhysics) {
		for (auto iter = m_physicsList.begin(); iter != m_physicsList.end();) {
			if (*(iter) == _pPhysics) {
				iter = m_physicsList.erase(iter);
			}
			else {
				iter++;
			}
		}
	}
	/**
	* @brief 충돌 객체들에 한해 매 프레임 물리 체크
	*/
	void Update_Physics(const _float& fTimeDelta);

private:
	/**
	* @brief 경계 상자/구체를 계산.
	* @param _pGameObject 경계 상자를 생성할 오브젝트 객체
	* @param _pTrans 오브젝트의 transform 컴포넌트
	* @param _pVIB 오브젝트의 VIBuffer 컴포넌트
	*/
	void Calc_Bounding(CGameObject* _pGameObject, CTransform* _pTrans, CVIBuffer* _pVIB);
	/**
	* @brief 모든 물리 객체를 순회하면서 경계 생성
	*/
	void Calc_All_Bounding();
	/**
	* @brief AABB 충돌 연산
	* @param _pDest 충돌 주체의 물리 오브젝트
	* @param _pTarget 충돌 타겟의 물리 오브젝트
	* @return true면 AABB충돌 false면 충돌안함.
	*/
	bool Check_AABB_Collision(IPhysics* _pDest, IPhysics* _pTarget);
	/**
	* @brief AABB 충돌 예측 연산
	* @details 물체의 이동방향을 예측해서 충돌을 미리 계산한다.
	* @param _pDest 충돌 주체의 물리 오브젝트
	* @param _pTarget 충돌 타겟의 물리 오브젝트
	* @return true면 AABB충돌 false면 충돌안함.
	*/
	bool Check_AABB_Collision_Predict(IPhysics* _pDest, IPhysics* _pTarget);
	/**
	* @brief 충돌시 물체의 움직임 멈춤.
	* @details 물체의 이동방향을 예측해서 충돌을 미리 계산한다.
	* @param _pTrans 오브젝트의 transform 컴포넌트
	*/
	void Block_Move(CTransform* _pTrans);

	/**
	* @brief 접촉 방향 구하기
	*/
	_vec3 Calc_ContactDir(IPhysics* _pDest, IPhysics* _pTarget);
	void Calc_SpeedVector();
	void Calc_RotateVector();
	void Apply_Rotate();
	_vec3 Reflect_Vector(const _vec3 vVelocity, const _vec3 vNormal);
	_vec3 Reflect_Velocity(IPhysics* _pPhys, _vec3 _vNormal);
	void Deceleration_Velociy(IPhysics* _pPhys, _vec3* _vReflectVec);

	virtual void		Free();
};

END

