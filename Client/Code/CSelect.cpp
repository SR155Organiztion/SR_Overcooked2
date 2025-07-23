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
#include "CSelectGameSystem.h"
#include <CDynamicCamera.h>
#include <CDynamicCamera2.h>
#include "CFlower.h"
#include "CCastle.h"
#include "CPlant.h"
#include "CTree.h"
#include "CBus.h"
#include "CTimerMgr.h"
#include "CEffectMgr.h"

#include "CUi_StageNumber.h"
#include "CUi_StageInfo.h"
#include <CUi_Fadeout.h>

CSelect::CSelect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CSelect::~CSelect()
{
    Free();
}

HRESULT	CSelect::Ready_Scene() {
    if (FAILED(
        CSelectGameSystem::GetInstance()->Ready_CSelectGameSystem
        ("SelectMap", m_pGraphicDev, this))) {
        return E_FAIL;
    }

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    CSelectGameSystem::GetInstance()->Set_NeedFocus(true);
    CSelectGameSystem::GetInstance()->Set_CurStageNum(-1);
    CSoundMgr::GetInstance()->Play_Sound(BGM_SELECTMAP, BGM_CHANNEL);

    return S_OK;
}

_int CSelect::Update_Scene(const _float& fTimeDelta) {
    _int iResult = Engine::CScene::Update_Scene(fTimeDelta);
    if (iResult == -1)
        return iResult;
    CEffectMgr::GetInstance()->Update_Effect(fTimeDelta);
    CPhysicsMgr::GetInstance()->Update_Physics(fTimeDelta);


    CDynamicCamera2* pCamera = dynamic_cast<CDynamicCamera2*>(CManagement::GetInstance()->Get_GameObject(L"Environment_Layer", L"DynamicCamera2"));

    //최초 1회 카메라세팅
    if (!m_bCameraSet) {
        CGameObject* pPlayer = CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Bus");
        pCamera->Set_Target(pPlayer);
        pCamera->Set_Perspective(CDynamicCamera2::PERSPECTIVE::THIRD);
        pCamera->Set_Offset(0.f, 3.f, -2.f);
        m_bCameraSet = true;

        for (auto Flag : *CSelectGameSystem::GetInstance()->Get_FlagVec()) {
            if (Flag->Get_StageNum() == -1) {
                _vec3 vFlagPos;
                CComponent* FlagTransform = static_cast<CGameObject*>(Flag)->Get_Component(ID_DYNAMIC, L"Com_Transform");
                dynamic_cast<CTransform*>(FlagTransform)->Get_Info(INFO_POS, &vFlagPos);

                dynamic_cast<CBus*>(pPlayer)->Set_FirstPos(vFlagPos.x, vFlagPos.y, vFlagPos.z);
                break;
            }
        }
    }

    static bool b = true;

    //unsigned char key = '1';
    //
    //for (int i = 1; i <= CSelectGameSystem::GetInstance()->Get_FlagVec()->size(); i++) {
    //    if (GetAsyncKeyState(key++) & 0x8000) {
    //        int stage = key - '0' - 2;
    //        for (auto Flag : *CSelectGameSystem::GetInstance()->Get_FlagVec()) {
    //            if (Flag->Get_StageNum() == stage) {
    //                pCamera->Focus(static_cast<CGameObject*>(Flag), 5.f, true, true);
    //                break;
    //            }
    //        }
    //    }
    //}

    if (CSelectGameSystem::GetInstance()->Get_NeedFocus()) {
        for (auto Flag : *CSelectGameSystem::GetInstance()->Get_FlagVec()) {
            if (Flag->Get_StageNum() == (CSelectGameSystem::GetInstance()->Get_CurStageNum() + 1)) {
                pCamera->Focus(static_cast<CGameObject*>(Flag), 5.f, true, true);
                CSelectGameSystem::GetInstance()->Set_NeedFocus(false);
                break;
            }
        }
    }

    if (GetAsyncKeyState('J')) {
        if (b) {
            _vec3 test = { 10.f, 0.f, 10.f };
            pCamera->Focus(test, 10.f, false, false);
            b = false;
        }
    }
    else {
        b = true;
    }

    pCamera->Update_GameObject(fTimeDelta);

    //임시 스테이지 불러오기
    unsigned char key = '1';
    for (int i = 1; i <= m_iMapSize; i++) {
        if (GetAsyncKeyState(key++)) {
            string szStageKey = "Stage" + to_string(i);
    
            CScene* pScene = CStageLoading::Create(m_pGraphicDev, szStageKey);
            if (nullptr == pScene)
                return E_FAIL;
    
            if (FAILED(CManagement::GetInstance()->Go_Stage(pScene)))
                return E_FAIL;
        }
    }

    //스테이지 번호  Ui
    CUi_StageNumber* pStageNumber = dynamic_cast<CUi_StageNumber*>(
        CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_SelectNumber"));

    static int StageNumCnt = 0;
    if (StageNumCnt == 0) {
        StageNumCnt++;
        for (auto* Flag : *(CSelectGameSystem::GetInstance()->Get_FlagVec())) {
            _vec3 vPos = Flag->Get_Pos();
            vPos += {0.f, -0.3f, -0.5f};
            if (Flag->Get_StageNum() != -1) {
                pStageNumber->Make_StageNumber(Flag->Get_StageNum(), vPos);
            }
        }
    }

    //스테이지 정보  Ui
    CUi_StageInfo* pStageInfo = dynamic_cast<CUi_StageInfo*>(
        CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_StageInfo"));

    static int StageInfoCnt = 0;
    if (StageInfoCnt == 0) {
        StageInfoCnt++;
        for (auto* Flag : *(CSelectGameSystem::GetInstance()->Get_FlagVec())) {
            _vec3 vPos = Flag->Get_Pos();
            vPos += {0.f, 0.8f, 0.5f};
            if (Flag->Get_StageNum() != -1) {
                pStageInfo->Make_StageInfo(Flag->Get_StageNum() + 1, Flag->Get_StarNum(), vPos);
            }
        }
    }

    return iResult;

}

void CSelect::LateUpdate_Scene(const _float& fTimeDelta) {
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
  
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

    /*pGameObject = CSkyBox::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"SkyBox", pGameObject)))
        return E_FAIL;*/

    _vec3	vEye{ 10.f, 7.f, 3.f };
    _vec3	vAt{ 10.f, 0.f, 8.f };
    _vec3	vUp{ 0.f , 1.f, 0.f };
    pGameObject = CDynamicCamera2::Create(m_pGraphicDev, &vEye, &vAt, &vUp);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"DynamicCamera2", pGameObject)))
        return E_FAIL;
;

    CSelectGameSystem::GetInstance()->Parse_EnviromentData(pLayer);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT	CSelect::Ready_GameObject_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    pGameObject = CBus::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    _vec3 vSize = { 1.f, 1.5f, 1.5f };
    vSize *= 0.5f;
    dynamic_cast<CBus*>(pGameObject)->Set_FirstScale(vSize.x , vSize.y, vSize.z);
    dynamic_cast<CBus*>(pGameObject)->Set_FirstPos(10.f, 0.f, 10.f);
    //CInGameSystem::GetInstance()->Setting_PlayerPos(pGameObject);
    if (FAILED(pLayer->Add_GameObject(L"Bus", pGameObject)))
        return E_FAIL;


    map<string, S_STAGE>* mapJson = CMapTool::GetInstance()->Get_MapInfo();
    m_iMapSize = mapJson->size();

    pGameObject = CFlag::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Flag", pGameObject)))
        return E_FAIL;

    CSelectGameSystem::GetInstance()->Parse_GameObjectData(pLayer);

    m_mapLayer.insert({ pLayerTag, pLayer });

    if (FAILED(CEffectMgr::GetInstance()->Reserve_Effect(L"CloudEffect", 40)))
        return E_FAIL;


    return S_OK;
}
HRESULT	CSelect::Ready_UI_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    //Stage Number
    pGameObject = CUi_Factory<CUi_StageNumber>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL; 
    if (FAILED(pLayer->Add_GameObject(L"Ui_SelectNumber", pGameObject)))
        return E_FAIL; 

    //Stage Info
    pGameObject = CUi_Factory<CUi_StageInfo>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_StageInfo", pGameObject)))
        return E_FAIL;

    //페이드 아웃
    pGameObject = CUi_Factory<CUi_Fadeout>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Fadeout", pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });
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
    Engine::CScene::Free();
    CSelectGameSystem::DestroyInstance();
}