#include "CManagement.h"
#include "CRenderer.h"
#include <CSoundMgr.h>
#include "CTimerMgr.h"
#include "CSoundMgr.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement() : m_pScene(nullptr)
{
}

CManagement::~CManagement()
{
    Free();
}

CComponent* CManagement::Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
    if (nullptr == m_pScene)
        return nullptr;

    return m_pScene->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

CGameObject* CManagement::Get_GameObject(const _tchar* _pLayerTag, const _tchar* _pObjTag)
{
    if (!m_pScene)
        return nullptr;

    return m_pScene->Get_GameObject(_pLayerTag, _pObjTag);
}

HRESULT CManagement::Delete_GameObject(const _tchar* _pLayerTag, const _tchar* _pObjTag, const CGameObject* pObj)
{
    if (!m_pScene)
        return E_FAIL;

    m_pScene->Delete_GameObject(_pLayerTag, _pObjTag, pObj);

    return S_OK;
}

CLayer* CManagement::Get_Layer(const _tchar* _pLayerTag)
{
    if (!m_pScene)
        return nullptr;

    return m_pScene->Get_Layer(_pLayerTag);
}

HRESULT CManagement::Set_Scene(CScene* pScene)
{
    if (nullptr == pScene)
        return  E_FAIL;

    Safe_Release(m_pScene);

    CRenderer::GetInstance()->Clear_RenderGroup();
    m_pScene = pScene;

    return S_OK;
}

_int CManagement::Update_Scene(const _float& fTimeDelta)
{
    if (nullptr == m_pScene)
        return -1;
    CSoundMgr::GetInstance()->Update(fTimeDelta);
    return m_pScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Scene(const _float& fTimeDelta)
{
    m_pScene->LateUpdate_Scene(fTimeDelta);
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    m_pScene->Render_Scene();
    CRenderer::GetInstance()->Render_GameObject(pGraphicDev);

}

HRESULT CManagement::Go_Stage(CScene* pScene)
{
    if (m_pScene)
        m_SceneStack.push(m_pScene);

    CRenderer::GetInstance()->Clear_RenderGroup();
    m_pScene = pScene;
    CSoundMgr::GetInstance()->Stop_Sound(BGM_CHANNEL);
    return S_OK;

}

HRESULT CManagement::Back_Select()
{
    if (m_SceneStack.empty()) {
        MSG_BOX("No scene to return to");
        return E_FAIL;
    }

    Safe_Release(m_pScene);                 // ���� �� ����
    CRenderer::GetInstance()->Clear_RenderGroup();

    m_pScene = m_SceneStack.top();          // ���� �� ����
    m_SceneStack.pop();
    
    CTimerMgr::GetInstance()->Resume_Timer(L"Timer_FPS");
    CSoundMgr::GetInstance()->Stop_All();
    CSoundMgr::GetInstance()->Play_Sound(BGM_SELECTMAP, BGM_CHANNEL);
    return S_OK;
}


void CManagement::Free()
{
    Safe_Release(m_pScene);
}
