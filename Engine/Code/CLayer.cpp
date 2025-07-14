#include "CLayer.h"
#include "IPhysics.h"
#include "CPhysicsMgr.h"
#include "IShadow.h"
#include "CShadow.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent* CLayer::Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(eID, pComponentTag);
}

CGameObject* CLayer::Get_GameObject(const _tchar* _pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(_pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

HRESULT CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	pGameObject->Set_SelfId(pObjTag);
	m_mapObject.insert({ pObjTag, pGameObject });

	if(dynamic_cast<IPhysics*>(pGameObject))
		CPhysicsMgr::GetInstance()->Add_PhysicsList(pGameObject);

	IShadow* pIterShadow = dynamic_cast<IShadow*>(pGameObject);
	if (pIterShadow) {
		CShadow* pShadow = CShadow::Create(pIterShadow->Get_GraphicDev());
		CTransform* pTrans = dynamic_cast<CTransform*>(
				pGameObject->Get_Component(
					COMPONENTID::ID_DYNAMIC
					, L"Com_Transform"
				)
			);

		CVIBuffer* pVIBuf = dynamic_cast<CVIBuffer*>(
			pGameObject->Get_Component(
				COMPONENTID::ID_STATIC
				, L"Com_Buffer"
				)
			);

		pShadow->Set_Transform(pTrans);
		pShadow->Set_VIBuffer(pVIBuf);

		_tchar szTag[128] = L"";
		swprintf_s(szTag, L"%s_%s", pObjTag, L"Shadow");

		m_mapObject.insert({ szTag, pShadow });
	}

	return S_OK;
}

HRESULT CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject, LPDIRECT3DDEVICE9 pGraphicDev)
{
	HRESULT hr = Add_GameObject(pObjTag, pGameObject);

	if (dynamic_cast<IShadow*>(pGameObject)) {
		CShadow* pShadow = CShadow::Create(pGraphicDev);
		CTransform* pTrans = dynamic_cast<CTransform*>(
			pGameObject->Get_Component(
				COMPONENTID::ID_DYNAMIC
				, L"Com_Transform"
			)
			);

		pShadow->Set_Transform(pTrans);
	}

	return hr;
}

HRESULT CLayer::Delete_GameObject(const _tchar* _pObjTag, const CGameObject* _pObj)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(_pObjTag));
	
	if (iter == m_mapObject.end())
		return E_FAIL;
	
	m_mapObject.erase(iter);

	return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult(0);

	for (auto& pObj : m_mapObject)
	{
		iResult = pObj.second->Update_GameObject(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& pObj : m_mapObject)
		pObj.second->LateUpdate_GameObject(fTimeDelta);
}



CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}
