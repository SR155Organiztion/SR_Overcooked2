#include "CObjectPoolMgr.h"
#include "CPhysicsMgr.h"

IMPLEMENT_SINGLETON(CObjectPoolMgr)

CObjectPoolMgr::CObjectPoolMgr()
{
}

CObjectPoolMgr::~CObjectPoolMgr()
{
	Free();
}

HRESULT CObjectPoolMgr::Register_Object(std::wstring pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto pair = m_mapObject.find(pObjTag);

	wstring selfId = pObjTag + std::to_wstring(pair->second.size()); // ³Ñ¹ö¸µ
	const _tchar* Final_selfId = selfId.c_str();
	pGameObject->Set_SelfId(Final_selfId);

	m_mapObject[pObjTag].push_back(pGameObject);

	return S_OK;
}

CGameObject* CObjectPoolMgr::Get_Object(std::wstring pObjTag)
{
	auto iter = m_mapObject.find(pObjTag);
	if (iter == m_mapObject.end())
		return nullptr;

	auto& vecObject = iter->second;
	if (vecObject.empty())
		return nullptr;

	CGameObject* pObj = vecObject.back();
	vecObject.pop_back();

	return pObj;
}

void CObjectPoolMgr::Return_Object(std::wstring pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return;

	CPhysicsMgr::GetInstance()->Delete_PhysicsList(pGameObject);

	m_mapObject[pObjTag].push_back(pGameObject);
}

void CObjectPoolMgr::Free()
{
	for (auto& pair : m_mapObject)
	{
		for (auto& pObj : pair.second)
		{
			Safe_Release(pObj);
		}
		pair.second.clear();
	}
	m_mapObject.clear();
}