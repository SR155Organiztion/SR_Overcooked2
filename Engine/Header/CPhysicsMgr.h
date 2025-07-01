#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "IPhysics.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "CTransform.h"
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CPhysicsMgr : public CBase
{
	DECLARE_SINGLETON(CPhysicsMgr)
private:
	explicit CPhysicsMgr();
	virtual ~CPhysicsMgr();

private:
	list<CGameObject*> m_physicsList;

public:
	void Add_PhysicsList(CGameObject* _pPhysics) {
		m_physicsList.push_back(_pPhysics);
	}

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
	void Update_Physics();

private:
	void Calc_Bounding(CGameObject* _pGameObject, CTransform* pTrans, CVIBuffer* pVIB);
	void Calc_All_Bounding();
	bool Check_AABB_Collision(IPhysics* _pDest, IPhysics* _pTarget);
	virtual void		Free();
};

END

