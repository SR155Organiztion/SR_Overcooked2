#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "IPhysics.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "CVIBuffer.h"
#include <tchar.h>

BEGIN(Engine)

/**
* @file    CPhysicsMgr.h
* @date    2025-07-02
* @author  안준형
* @brief   충돌 매니저
* @details IPhysics를 상속받은 객체들에 한해 충돌 처리.
*/
class CTransform;
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
		static _int idx = 1;
		_pPhysics->AddRef();
		dynamic_cast<IPhysics*>(_pPhysics)->Set_Idx(idx++);
		m_physicsList.push_back(_pPhysics);
	}

	void Delete_PhysicsList(CGameObject* _pGameObject) {
		for (auto iter = m_physicsList.begin(); 
			iter != m_physicsList.end(); iter++) {

			if (_tcscmp((*iter)->Get_SelfId(), _pGameObject->Get_SelfId())
				== 0) {
				m_physicsList.erase(iter);
				break;
			}
		}
	}
	void Update_Physics(const _float& _fTimeDelta);
	_bool Check_AnyCollision(CTransform* _pTransform, const _vec3& _vTargetPos);
	void Render_BoundingBoxes(LPDIRECT3DDEVICE9 pDevice);

private:
	bool Check_AABB_Collision(IPhysics* _pPhys, IPhysics* _pOtherPhys);
	bool Check_AABB_Collision_Actual(IPhysics* _pPhys, IPhysics* _pOtherPhys);
	void Resolve_Collision(CGameObject* _pGameObject, IPhysics* _pSelf, IPhysics* _pOther, CTransform* _pTransform);
	_bool IsSameBox(const _vec3* _pA, const _vec3* _pB, float _fEpsilon = 0.01f);

private:
	virtual void		Free();
};

END

