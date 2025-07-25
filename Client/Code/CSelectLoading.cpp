#include "pch.h"
#include "CSelectLoading.h"
#include "CFontMgr.h"
#include "CLoading.h"
#include "CSelect.h"
#include "CManagement.h"
#include "CProtoMgr.h"
#include "CSprite.h"
#include "CUi_StageInfo.h"
#include <CUi_Fadeout.h>
#include <CUi_Factory.h>
#include "CUi_GameLoding.h"
#include "CUi_Connectiontpage.h"

CSelectLoading::CSelectLoading(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{

}

CSelectLoading::~CSelectLoading()
{
    Free();
}

HRESULT	CSelectLoading::Ready_Scene() {

    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_SELECT, &m_iLoadingPercent);

    if (m_pLoading == nullptr) {
        return E_FAIL;
    }



    return S_OK;
}

_int CSelectLoading::Update_Scene(const _float& fTimeDelta) {
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    CUi_GameLoding* GameLoading = dynamic_cast<CUi_GameLoding*>(CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_GameLoding"));
    GameLoading->Make_GameLoding(true);
    GameLoading->Set_Loding(m_iLoadingPercent);

    CUi_Connectiontpage* Connectiontpage = dynamic_cast<CUi_Connectiontpage*>(CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Connectiontpage"));
    Connectiontpage->Make_Connectiontpage();

    if (true == m_pLoading->Get_Finish())
    {
        Engine::CScene* pScene = CSelect::Create(m_pGraphicDev);
        if (nullptr == pScene)
            return E_FAIL;

        if (FAILED(CManagement::GetInstance()->Set_Scene(pScene)))
            return E_FAIL;
    }
    return iExit;
}
void CSelectLoading::LateUpdate_Scene(const _float& fTimeDelta) {

}

void CSelectLoading::Render_Scene()
{
    _vec2   vPos{ 100.f, 100.f };

    CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_pLoading->Get_String(), &vPos, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

HRESULT	CSelectLoading::Ready_Environment_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}
HRESULT	CSelectLoading::Ready_GameObject_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}
HRESULT	CSelectLoading::Ready_UI_Layer(const _tchar* pLayerTag) {
   
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    //게임 로딩
    pGameObject = CUi_Factory<CUi_GameLoding>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_GameLoding", pGameObject)))
        return E_FAIL;

    //햄버거 로딩
    pGameObject = CUi_Factory<CUi_Connectiontpage>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Connectiontpage", pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}

HRESULT	CSelectLoading::Ready_Prototype() {
    return S_OK;
}

CSelectLoading* CSelectLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
    CSelectLoading* pSelect = new CSelectLoading(pGraphicDev);

    if (FAILED(pSelect->Ready_Scene())) {
        Safe_Delete(pSelect);
        MSG_BOX("SelectLoading Create Failed");
        return nullptr;
    }

    return pSelect;
}

void CSelectLoading::Free() {

}