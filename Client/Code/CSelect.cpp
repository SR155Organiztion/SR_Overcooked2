#include "pch.h"
#include "CSelect.h"
#include "CFontMgr.h"
#include "CUi_Factory.h"
#include "CUi_Timer.h"
#include "CPhysicsMgr.h"
#include "CMapTool.h"
#include "CStage.h"
#include "CManagement.h"
#include "CStageLoading.h"

CSelect::CSelect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CSelect::~CSelect()
{
    Free();
}

HRESULT	CSelect::Ready_Scene() {
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    return S_OK;
}
_int CSelect::Update_Scene(const _float& fTimeDelta) {
    _int iResult = Engine::CScene::Update_Scene(fTimeDelta);
    CPhysicsMgr::GetInstance()->Update_Physics(fTimeDelta);
    // 임시 키 입력
    unsigned char key = '1';
    for (int i = 1; i <= m_iMapSize; i++) {
        if (GetAsyncKeyState(key++)) {
            string szStageKey = "Stage" + to_string(i);

            CScene* pScene = CStageLoading::Create(m_pGraphicDev, szStageKey);
            if (nullptr == pScene)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Set_Scene(pScene)))
                return E_FAIL;
        }
    }
    return iResult;
}
void CSelect::LateUpdate_Scene(const _float& fTimeDelta) {

}

void CSelect::Render_Scene()
{
    _vec2 vPos = { 100, 200 };

    TCHAR szStr[128] = L"";
    swprintf_s(szStr, L"Size : %d", m_iMapSize);

    CFontMgr::GetInstance()->Render_Font(
        L"Font_Default"
        , szStr
        , &vPos
        , D3DXCOLOR(0.f, 0.f, 0.f, 1.f)
    );

    for (int i = m_iMapSize; i > 0 ; i--) {
        vPos.y -= 50;
        TCHAR szStr[128] = L"";
        swprintf_s(szStr, L"Stage%d >> Press %d", i, i);

        CFontMgr::GetInstance()->Render_Font(
            L"Font_Default"
            , szStr
            , &vPos
            , D3DXCOLOR(0.f, 0.f, 0.f, 1.f)
        );
    }

}

HRESULT	CSelect::Ready_Environment_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}
HRESULT	CSelect::Ready_GameObject_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    map<string, S_STAGE>* mapJson = CMapTool::GetInstance()->Get_MapInfo();
    m_iMapSize = mapJson->size();
    /*for (auto iter = mapJson->begin(); iter != mapJson->end(); iter++) {
        
    }*/

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}
HRESULT	CSelect::Ready_UI_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}

HRESULT	CSelect::Ready_Prototype() {
    return S_OK;
}

CSelect* CSelect::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
    CSelect* pSelect = new CSelect(pGraphicDev);

    if (FAILED(pSelect->Ready_Scene())) {
        Safe_Delete(pSelect);
        MSG_BOX("Select Create Failed");
        return nullptr;
    }

    return pSelect;
}

void CSelect::Free() {

}