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
* @author  ������
* @brief   �浹 �Ŵ���
* @details IPhysics�� ��ӹ��� ��ü�鿡 ���� �浹 ó��.
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
	* @brief �浹 ����Ʈ�� ��ü�� �߰�. Layer�ܿ��� ó����.
	* @param _pPhysics - ����Ʈ�� �߰��� �浹 ��ü
	*/
	void Add_PhysicsList(CGameObject* _pPhysics) {
		_pPhysics->AddRef();
		m_physicsList.push_back(_pPhysics);
	}

	/**
	* @brief �浹 ����Ʈ���� ��ü�� ����.
	* @param _pPhysics - ������ �浹 ��ü
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
	* @brief �浹 ��ü�鿡 ���� �� ������ ���� üũ
	*/
	void Update_Physics(const _float& fTimeDelta);

private:
	/**
	* @brief ��� ����/��ü�� ���.
	* @param _pGameObject ��� ���ڸ� ������ ������Ʈ ��ü
	* @param _pTrans ������Ʈ�� transform ������Ʈ
	* @param _pVIB ������Ʈ�� VIBuffer ������Ʈ
	*/
	void Calc_Bounding(CGameObject* _pGameObject, CTransform* _pTrans, CVIBuffer* _pVIB);
	/**
	* @brief ��� ���� ��ü�� ��ȸ�ϸ鼭 ��� ����
	*/
	void Calc_All_Bounding();
	/**
	* @brief AABB �浹 ����
	* @param _pDest �浹 ��ü�� ���� ������Ʈ
	* @param _pTarget �浹 Ÿ���� ���� ������Ʈ
	* @return true�� AABB�浹 false�� �浹����.
	*/
	bool Check_AABB_Collision(IPhysics* _pDest, IPhysics* _pTarget);
	/**
	* @brief AABB �浹 ���� ����
	* @details ��ü�� �̵������� �����ؼ� �浹�� �̸� ����Ѵ�.
	* @param _pDest �浹 ��ü�� ���� ������Ʈ
	* @param _pTarget �浹 Ÿ���� ���� ������Ʈ
	* @return true�� AABB�浹 false�� �浹����.
	*/
	bool Check_AABB_Collision_Predict(IPhysics* _pDest, IPhysics* _pTarget);
	/**
	* @brief �浹�� ��ü�� ������ ����.
	* @details ��ü�� �̵������� �����ؼ� �浹�� �̸� ����Ѵ�.
	* @param _pTrans ������Ʈ�� transform ������Ʈ
	*/
	void Block_Move(CTransform* _pTrans);

	/**
	* @brief ���� ���� ���ϱ�
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

