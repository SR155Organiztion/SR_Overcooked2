#include "CObjectPoolMgr.h"
#include "CPhysicsMgr.h"
#include "CTransform.h"

IMPLEMENT_SINGLETON(CObjectPoolMgr)

CObjectPoolMgr::CObjectPoolMgr()
{
	m_mapObject[L"Plate_"] = std::vector<CGameObject*>();
}

CObjectPoolMgr::~CObjectPoolMgr()
{
	Free();
}

HRESULT CObjectPoolMgr::Register_Object(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto pair = m_mapObject.find(pObjTag);
	if (pair == m_mapObject.end())
	{
		m_mapObject[pObjTag] = std::vector<CGameObject*>();
		pair = m_mapObject.find(pObjTag);
	}
	_int Number = pair->second.size();
	_int szLength = _sctprintf(_T("%s%d"), pObjTag, Number);
	_tchar* buf = static_cast<_tchar*>(std::malloc((szLength + 1) * sizeof(_tchar)));
	if (!buf)
		return E_FAIL;

	_stprintf_s(buf, szLength + 1, _T("%s%d"), pObjTag, Number);
	pGameObject->Set_SelfId(buf);

	m_mapObject[pObjTag].push_back(pGameObject);

	return S_OK;
}

CGameObject* CObjectPoolMgr::Get_Object(const _tchar* pObjTag)
{	
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
	if (iter == m_mapObject.end())
		return nullptr;

	auto& vecObject = iter->second;
	if (vecObject.empty())
		return nullptr;

	CGameObject* pObj = vecObject.back();

	vecObject.pop_back();

	return pObj;
}

CGameObject* CObjectPoolMgr::Get_Object(const _tchar* pObjTag, const _vec3 vPos)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
	if (iter == m_mapObject.end())
		return nullptr;

	auto& vecObject = iter->second;
	if (vecObject.empty())
		return nullptr;

	CGameObject* pObj = vecObject.back();

	CTransform* pTransform = dynamic_cast<CTransform*>(pObj->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
	pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);

	vecObject.pop_back();

	return pObj;
}

void CObjectPoolMgr::Return_Object(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return;

	CPhysicsMgr::GetInstance()->Delete_PhysicsList(pGameObject);

	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
	if (iter == m_mapObject.end())
		return;

	iter->second.push_back(pGameObject);
}

_bool CObjectPoolMgr::Is_Empty(const _tchar* pObjTag)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
	if (iter == m_mapObject.end())
		return true;

	if (iter->second.empty())
		return true;

	return false;	
}

void CObjectPoolMgr::Set_Empty(const _tchar* pObjTag)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
	if (iter == m_mapObject.end())
		return;

	iter->second.clear();
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