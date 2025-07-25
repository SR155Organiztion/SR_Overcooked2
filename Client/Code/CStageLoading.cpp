#include "pch.h"
#include "CStageLoading.h"
#include "CFontMgr.h"
#include "CLoading.h"
#include "CSelect.h"
#include "CManagement.h"
#include "CProtoMgr.h"
#include "CStage.h"

#include "CUi_Factory.h"
#include "CUi_Timer.h"
#include "CUi_Order.h"
#include "CUi_Score.h"
#include <CUi_Fadeout.h>
#include "CUi_GameLoding.h"
#include "CUi_Connectiontpage.h"

CStageLoading::CStageLoading(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev)
{

}

CStageLoading::CStageLoading(LPDIRECT3DDEVICE9 pGraphicDev, string _szStageKey)
    : CScene(pGraphicDev)
    , m_szStageKey(_szStageKey)
{

}

CStageLoading::~CStageLoading()
{
    Free();
}

HRESULT	CStageLoading::Ready_Scene() {


    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    Ready_Sound();

    m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE, &m_iLoadingPercent);

    if (m_pLoading == nullptr) {
        return E_FAIL;
    }

    return S_OK;
}
_int CStageLoading::Update_Scene(const _float& fTimeDelta) {
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    CUi_GameLoding* GameLoading = dynamic_cast<CUi_GameLoding*>(CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_GameLoding"));
    GameLoading->Make_GameLoding(true);
    /*GameLoading->Set_Loding((_float)m_iLoadingPercent);*/
    GameLoading->Set_Loding((_float)m_iLoadingPercent);


    CUi_Connectiontpage* Connectiontpage = dynamic_cast<CUi_Connectiontpage*>(CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Connectiontpage"));
    Connectiontpage->Make_Connectiontpage();
  


    if (m_pLoading->Get_Finish())
    {
        Engine::CScene* pScene = CStage::Create(m_pGraphicDev, m_szStageKey);
        if (nullptr == pScene)
            return E_FAIL;

        if (FAILED(CManagement::GetInstance()->Set_Scene(pScene)))
            return E_FAIL;
    }

    return iExit;
}
void CStageLoading::LateUpdate_Scene(const _float& fTimeDelta) {

}

void CStageLoading::Render_Scene()
{
    _vec2   vPos{ 100.f, 100.f };

    CFontMgr::GetInstance()->Render_Font(L"Font_Default", m_pLoading->Get_String(), &vPos, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

HRESULT	CStageLoading::Ready_Environment_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}
HRESULT	CStageLoading::Ready_GameObject_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;
}
HRESULT	CStageLoading::Ready_UI_Layer(const _tchar* pLayerTag) {
    
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    //���� �ε�
    pGameObject = CUi_Factory<CUi_GameLoding>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_GameLoding", pGameObject)))
        return E_FAIL;

    ////ÆäÀÌµå ¾Æ¿ô
    //pGameObject = CUi_Factory<CUi_Fadeout>::Ui_Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Ui_Fadeout", pGameObject)))
    //    return E_FAIL;
  
        //햄버거 로딩
    pGameObject = CUi_Factory<CUi_Connectiontpage>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Connectiontpage", pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT	CStageLoading::Ready_Prototype() {
    return S_OK;
}

void CStageLoading::Ready_Sound() {
    CSoundMgr::GetInstance()->Load_Sound(STAGE1_BGM, "/DownTheRiver.wav", false, STAGE_BGM_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(STAGE2_BGM, "/TheMine.wav", false, STAGE_BGM_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(STAGE3_BGM, "/TheNeonCity.wav", false, STAGE_BGM_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(STAGE4_BGM, "/DownTheRiver.wav", false, STAGE_BGM_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(LEVEL_GO, "/LevelGo.wav", false, LEVEL_READY_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(RESULT_BGM, "/RoundResults.wav", false, STAGE_BGM_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(TIME_UP, "/TimesUpSting.wav", false, TIME_UP_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(ORDER_SUCCESS, "/SuccessfulDelivery.wav", false, ORDER_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(ORDER_FAIL, "/VO_Fail_12.wav", false, ORDER_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(ORDER_TIMEOUT, "/RecipeTimeout.wav", false, ORDER_CHANNEL);
    CSoundMgr::GetInstance()->Load_Sound(ORDER_INCOME, "/ServiceBell.wav", false, ORDER_CHANNEL);
}

CStageLoading* CStageLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, string _szStageKey) {
    CStageLoading* pSelect = new CStageLoading(pGraphicDev, _szStageKey);

    if (FAILED(pSelect->Ready_Scene())) {
        Safe_Delete(pSelect);
        MSG_BOX("StageLoading Create Failed");
        return nullptr;
    }

    return pSelect;
}

void CStageLoading::Free() {

}