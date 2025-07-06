#include "pch.h"
#include "CMenu.h"
#include "CFontMgr.h"
#include "CUi_Factory.h"
#include "CUi_TimeLimit.h"
#include "CPhysicsMgr.h"
#include "CMapTool.h"
#include "CStage.h"
#include "CManagement.h"
#include "CStageLoading.h"
#include "CSelectLoading.h"

CMenu::CMenu(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{

}

CMenu::~CMenu()
{
    Free();
}

HRESULT	CMenu::Ready_Scene() {
    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    return S_OK;
}
_int CMenu::Update_Scene(const _float& fTimeDelta) {
    _int iResult = Engine::CScene::Update_Scene(fTimeDelta);

    // ÀÓ½Ã Å°ÀÎÇ²
    if (GetAsyncKeyState(VK_RETURN)) {
        Engine::CScene* pScene = CSelectLoading::Create(m_pGraphicDev);
        if (nullptr == pScene)
            return E_FAIL;

        if (FAILED(CManagement::GetInstance()->Set_Scene(pScene)))
            return E_FAIL;
    }

    return iResult;
}
void CMenu::LateUpdate_Scene(const _float& fTimeDelta) {

}

void CMenu::Render_Scene()
{
    _vec2 vPos = { 100, 200 };

    CFontMgr::GetInstance()->Render_Font(
        L"Font_Default"
        , L"Main Menu"
        , &vPos
        , D3DXCOLOR(0.f, 0.f, 0.f, 1.f)
    );


}

HRESULT	CMenu::Ready_Environment_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}
HRESULT	CMenu::Ready_GameObject_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}
HRESULT	CMenu::Ready_UI_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}

CMenu* CMenu::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
    CMenu* pSelect = new CMenu(pGraphicDev);

    if (FAILED(pSelect->Ready_Scene())) {
        Safe_Delete(pSelect);
        MSG_BOX("Main Menu Create Failed");
        return nullptr;
    }

    return pSelect;
}

void CMenu::Free() {

}