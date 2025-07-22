#include "CScene.h"
#include "CSoundMgr.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CComponent* CScene::Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
    auto    iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

    if (iter == m_mapLayer.end())
        return nullptr;

    return iter->second->Get_Component(eID, pObjTag, pComponentTag);
}

CGameObject* CScene::Get_GameObject(const _tchar* _pLayerTag, const _tchar* _pObjTag) {
    auto    iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));

    if (iter == m_mapLayer.end())
        return nullptr;

    return iter->second->Get_GameObject(_pObjTag);
}

HRESULT CScene::Delete_GameObject(const _tchar* _pLayerTag, const _tchar* _pObjTag, const CGameObject* _pObj)
{
    auto    iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));

    if (iter == m_mapLayer.end())
        return E_FAIL;

    iter->second->Delete_GameObject(_pObjTag, _pObj);

    return S_OK;
}

CLayer* CScene::Get_Layer(const _tchar* _pLayerTag) 
{
    auto    iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));

    if (iter == m_mapLayer.end())
        return nullptr;

    return iter->second;
}

HRESULT CScene::Ready_Scene()
{
    return S_OK;
}

_int CScene::Update_Scene(const _float& fTimeDelta)
{
    for (auto& pLayer : m_mapLayer) {
        if (pLayer.second == nullptr) continue;
        _int iResult = pLayer.second->Update_Layer(fTimeDelta);
        if (iResult == -1) return -1;
    }
        

    return 0;
}

void CScene::LateUpdate_Scene(const _float& fTimeDelta)
{
    for (auto& pLayer : m_mapLayer) {
        if (pLayer.second == nullptr) continue;
        pLayer.second->LateUpdate_Layer(fTimeDelta);
    }
        
}


void CScene::Free()
{
    for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
    m_mapLayer.clear();
    Safe_Release(m_pGraphicDev);
}
