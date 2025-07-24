#include "pch.h"
#include "CStage.h"
#include "CPlayer.h"
#include "CRealPlayer.h"
#include "CMonster.h"
#include "CTerrain.h"
#include "CProtoMgr.h"
#include "CDynamicCamera.h"
#include "CSkyBox.h"
#include "CLightMgr.h"
#include "CDummyEffect.h"

#include "CSeaweed.h"
#include "CLettuce.h"
#include "CTomato.h"
#include "CCucumber.h"
#include "CFish.h"
#include "CShrimp.h"
#include "CRice.h"
#include "CPasta.h"
#include "CTomatoSoup.h"

#include "CFryingpan.h"
#include "CPot.h"
#include "CPlate.h"

#include "CIngredientStation.h"
#include "CChopStation.h"
#include "CGasStation.h"
#include "CEmptyStation.h"
#include "CSinkStation.h"
#include "CCleanPlateStation.h"
#include "CDirtyPlateStation.h"
#include "CServingStation.h"
#include "CTrashStation.h"
#include "CFloor.h"
#include "CInvisibleStation.h"
#include "CDispenserStation.h"

#include "CFakePlayer.h"
#include "CLettuceTemp.h"
#include "CPhysicsMgr.h"
#include "CEmptyStationTemp.h"

#include "CUi_Factory.h"
#include "CUi_Timer.h"
#include "CUi_Score.h"
#include "CUi_OrderMgr.h"
#include "CUi_Icon.h"
#include "CUi_CookLoding.h"
#include "CUi_CookLodingBox.h"
#include "CUi_WarningBox.h"
#include "CUi_TimeOut.h"
#include "CUi_StarScore.h"
#include "CUi_Fadeout.h"
#include "CUi_BurntFood.h"
#include "CUi_PostCard.h"
#include "CIngredient.h"
#include "Engine_Define.h"

#include "CInteractMgr.h"
#include "CFontMgr.h"
#include "CUtil.h"
#include "CInGameSystem.h"

#include "CEffectMgr.h"
#include "CObjectPoolMgr.h"
#include <CTimerMgr.h>
#include <CManagement.h>
#include <CSelectLoading.h>
#include <CSelect.h>
#include <CinematicCamera.h>


#include "CSelectGameSystem.h"
#include "COnionKing.h"
#include "CRoadTile.h"


CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{ 
}

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev, string _szCurrStage)
    : Engine::CScene(pGraphicDev)
    , m_szCurrStage(_szCurrStage)
{
}

CStage::~CStage()
{
    Free();
}

HRESULT CStage::Ready_Scene()
{
    CTimerMgr::GetInstance()->Resume_Timer(L"Timer_FPS");
    CSoundMgr::GetInstance()->Stop_All();
    if (FAILED(
        CInGameSystem::GetInstance()->Ready_CInGameSystem
        (m_szCurrStage, m_pGraphicDev, this))) {
        return E_FAIL;
    }

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

     

    if (FAILED(Ready_Light()))
        return E_FAIL;

    if (FAILED(Ready_Ingredient()))
        return E_FAIL;

    if (FAILED(CEffectMgr::GetInstance()->Reserve_Effect(L"FireEffect", 20)))
        return E_FAIL;

    if (FAILED(CEffectMgr::GetInstance()->Reserve_Effect(L"FireStartEffect", 10)))
        return E_FAIL;

    if (FAILED(CEffectMgr::GetInstance()->Reserve_Effect(L"HitEffect", 10)))
        return E_FAIL;

    if (FAILED(CEffectMgr::GetInstance()->Reserve_Effect(L"SteamEffect", 60)))
        return E_FAIL;

    if (FAILED(CEffectMgr::GetInstance()->Reserve_Effect(L"ExtinguishEffect", 30)))
        return E_FAIL;

    return S_OK;
}

HRESULT CStage::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    pGameObject = CSkyBox::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"SkyBox", pGameObject)))
        return E_FAIL;

    /*pGameObject = CTerrain::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
        return E_FAIL;*/

    // dynamicCamera
    /*_float fWidth =
        dynamic_cast<CVIBuffer*>(
                pGameObject->Get_Component(
                    COMPONENTID::ID_STATIC, L"Com_Buffer"
                )
            )->Get_Width() * 0.5f;*/

    _float fWidth =
        CInGameSystem::GetInstance()->Get_MapSize().iX * 0.5f;

    _vec3	vEye{ fWidth, 10.f, -3.f };
    _vec3	vAt{ fWidth, 0.f, 4.f };
    _vec3	vUp{ 0.f , 1.f, 0.f };
    pGameObject = CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
        return E_FAIL;

    if (m_szCurrStage == "Stage3" || m_szCurrStage == "Stage4") {
        pGameObject = CRoadTile::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        if (FAILED(pLayer->Add_GameObject(L"RoadTile", pGameObject)))
            return E_FAIL;
    }

    m_mapLayer.insert({ pLayerTag, pLayer });

    for (int i = 0; i < UI_PHASE_MAX; i++) {
        m_InGameUIVec[i] = false;
    }

    return S_OK;
}

HRESULT CStage::Ready_GameObject_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    if (FAILED(CInGameSystem::GetInstance()->Parse_GameObjectData(pLayer))) {
        return E_FAIL;
    }

    CGameObject* pPlayer1 =
        CManagement::GetInstance()->Get_GameObject(
            L"GameObject_Layer"
            , L"Player1"
        );

    CGameObject* pPlayer2 =
        CManagement::GetInstance()->Get_GameObject(
            L"GameObject_Layer"
            , L"Player2"
        );
    
    pGameObject = CinematicCamera::Create(m_pGraphicDev, pLayer->Get_GameObject(L"Player1"));
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"CinematicCamera1", pGameObject)))
        return E_FAIL;

    pGameObject = CinematicCamera::Create(m_pGraphicDev, pLayer->Get_GameObject(L"Player2"));
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"CinematicCamera2", pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    //디스펜서테스트
    //pGameObject = CDispenserStation::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //dynamic_cast<CDispenserStation*>(pGameObject)->Set_TypeIngredientStation(L"Dispenser_Rice");
    //if (FAILED(pLayer->Add_GameObject(L"Station_Dispenser", pGameObject)))
    //    return E_FAIL;

    // OnionKing 테스트
    pGameObject = COnionKing::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"OnionKing", pGameObject)))
        return E_FAIL;

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
      Engine::CGameObject* pGameObject = nullptr;

   
    //제한시간
      Engine::CGameObject* pGameObject2 = nullptr;
      Engine::CGameObject* pGameObject3 = nullptr;
    pGameObject = CUi_Factory<CUi_Timer>::Ui_Create(m_pGraphicDev, IMAGE_GAUGE);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object1", pGameObject)))
        return E_FAIL;

    pGameObject2 = CUi_Factory<CUi_Timer>::Ui_Create(m_pGraphicDev, LODING_GAUGE);
    if (nullptr == pGameObject2)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object2", pGameObject2)))
        return E_FAIL;

    pGameObject3 = CUi_Factory<CUi_Timer>::Ui_Create(m_pGraphicDev, FONT_GAUGE);
    if (nullptr == pGameObject3)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object3", pGameObject3)))
        return E_FAIL;

    CInGameSystem::GetInstance()->Setting_LimitTime(pGameObject, pGameObject2, pGameObject3);

    //점수
    
    pGameObject = CUi_Factory<CUi_Score>::Ui_Create(m_pGraphicDev, IMAGE_GAUGE);
    // 점수 초기화
    dynamic_cast<CUi_Score*>(pGameObject)->Set_Score(
        dynamic_cast<CUi_Score*>(pGameObject)->Get_Score() * -1
    );
    
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object4", pGameObject)))
        return E_FAIL;
    
    pGameObject = CUi_Factory<CUi_Score>::Ui_Create(m_pGraphicDev, LODING_GAUGE);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object5", pGameObject)))
        return E_FAIL;
   
    //코인 애니메이션
    pGameObject = CUi_Factory<CUi_Score>::Ui_Create(m_pGraphicDev, IMAGE2_GAUGE);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object6", pGameObject)))
        return E_FAIL;
 
    pGameObject = CUi_Factory<CUi_Score>::Ui_Create(m_pGraphicDev, FONT_GAUGE);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object7", pGameObject)))
       return E_FAIL;
    
    //아이콘
    pGameObject = CUi_Factory<CUi_Icon>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object9", pGameObject)))
      return E_FAIL;


    //레시피
    pGameObject = CUi_Factory<CUi_Order>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object8", pGameObject)))
        return E_FAIL;

    CInGameSystem::GetInstance()->Set_OrderList(pGameObject);

    // 쿡 로딩 BOX
    pGameObject = CUi_Factory<CUi_CookLodingBox>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object10", pGameObject)))
        return E_FAIL;

    // 쿡 로딩 게이지
    pGameObject = CUi_Factory<CUi_CookLoding>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object11", pGameObject)))
        return E_FAIL;

    // 워닝박스
    pGameObject = CUi_Factory<CUi_WarningBox>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object12", pGameObject)))
        return E_FAIL;

    // 레디 고 타임아웃
    pGameObject = CUi_Factory<CUi_TimeOut>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_TimeOut", pGameObject)))
        return E_FAIL;

    // 총점정리
    pGameObject = CUi_Factory<CUi_StarScore>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_StarScore", pGameObject)))
        return E_FAIL;

    //페이드 아웃
    pGameObject = CUi_Factory<CUi_Fadeout>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Fadeout", pGameObject)))
        return E_FAIL; 

    //칠판
    pGameObject = CUi_Factory<CUi_Board>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Board", pGameObject)))
        return E_FAIL;

    //음식 탈 때 경고창
    pGameObject = CUi_Factory<CUi_BurntFood>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_BurntFood", pGameObject)))
        return E_FAIL;

    //메인
    pGameObject = CUi_Factory<CUi_PostCard>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_BurntFood", pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_Ingredient()
{
    Engine::CGameObject* pGameObject = nullptr;

    for (_int i = 0; i < 10; ++i) {
        pGameObject = CSeaweed::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        CObjectPoolMgr::GetInstance()->Register_Object(L"Seaweed", pGameObject);
        
        pGameObject = CLettuce::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        CObjectPoolMgr::GetInstance()->Register_Object(L"Lettuce", pGameObject);

        pGameObject = CTomato::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        CObjectPoolMgr::GetInstance()->Register_Object(L"Tomato", pGameObject);

        pGameObject = CCucumber::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        CObjectPoolMgr::GetInstance()->Register_Object(L"Cucumber", pGameObject);

        pGameObject = CFish::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        CObjectPoolMgr::GetInstance()->Register_Object(L"Fish", pGameObject);

        pGameObject = CShrimp::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        CObjectPoolMgr::GetInstance()->Register_Object(L"Shrimp", pGameObject);

        pGameObject = CRice::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        CObjectPoolMgr::GetInstance()->Register_Object(L"Rice", pGameObject);

        pGameObject = CPasta::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        CObjectPoolMgr::GetInstance()->Register_Object(L"Pasta", pGameObject);

        pGameObject = CTomatoSoup::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;
        CObjectPoolMgr::GetInstance()->Register_Object(L"Tomatosoup", pGameObject);
    }
   
    return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
    _int iResult = Engine::CScene::Update_Scene(fTimeDelta);
    CEffectMgr::GetInstance()->Update_Effect(fTimeDelta);
    CPhysicsMgr::GetInstance()->Update_Physics(fTimeDelta);
    CInGameSystem::GetInstance()->Update_InGameSystem(fTimeDelta, this);

    //실험용

    //CUi_Fadeout* pFadeout = dynamic_cast<CUi_Fadeout*>(CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Fadeout"));
    //pFadeout->Make_Fadeout(1);

    //실험용

    
    CUi_TimeOut* pTimeUI =
        dynamic_cast<CUi_TimeOut*>(
            CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_TimeOut")
        );

    CUi_Timer* pTimerUI =
        dynamic_cast<CUi_Timer*>(
            CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_Object1")
        );

    _bool bIsEvent = CInGameSystem::GetInstance()->Get_Event().bEvent;

    CLayer* pLayer = nullptr;
    if (!m_pBoardUI) {
        CTimerMgr::GetInstance()->Stop_Timer(L"Timer_FPS");
        for (auto& val : m_mapLayer) {
            if (lstrcmpW(val.first, L"UI_Layer") == 0) {
                pLayer = val.second;
            }
        }

        m_pBoardUI = dynamic_cast<CUi_Board*>(
            pLayer->Get_GameObject(L"Ui_Board")
            );
        _int iIdx = Get_NumberEndOfString(m_szCurrStage);

        m_pBoardUI->Make_Board(TRUE, iIdx);

        //m_pBoardUI->On_Off(TRUE);
    }

    if (m_bIsBoardOpen) {
        if (GetAsyncKeyState(VK_SPACE)) {
            _int iIdx = Get_NumberEndOfString(m_szCurrStage);

            m_pBoardUI->Make_Board(FALSE, iIdx);
            m_bIsBoardOpen = FALSE;
        }
    }
    
    static _int iPatternCnt = 0;

    if (bIsEvent && !m_bDoPattern && iPatternCnt == 0) {
        _float fTime = pTimerUI->Get_Timer();

        _float fEventTime = 
            CInGameSystem::GetInstance()->Get_Event().fEventTime;

        // 이벤트 실행
        if (fTime >= fEventTime) {
            COnionKing* pOnionKing =
                dynamic_cast<COnionKing*>(
                    CManagement::GetInstance()->
                    Get_GameObject(
                        L"GameObject_Layer", L"OnionKing"
                    )
                    );

            pOnionKing->Set_Active(TRUE);
            pOnionKing->Set_State(COnionKing::ONION_DANCE);

            _bool isOnionWalkEnd = pOnionKing->Get_WalkEnd();

            if (isOnionWalkEnd) {
                CTimerMgr::GetInstance()->Stop_Timer(L"Timer_FPS");
                m_bDoPattern = TRUE;

                CLayer* pLayer = nullptr;

                for (auto& val : m_mapLayer) {
                    if (lstrcmpW(val.first, L"GameObject_Layer") == 0) {
                        pLayer = val.second;
                    }
                }

                CRealPlayer* pPlayer1 = dynamic_cast<CRealPlayer*>(
                    pLayer->Get_GameObject(L"Player1")
                    );

                CRealPlayer* pPlayer2 = dynamic_cast<CRealPlayer*>(
                    pLayer->Get_GameObject(L"Player2")
                    );

                pPlayer1->Start_SurprisedAnimaition();
                pPlayer2->Start_SurprisedAnimaition();

                CInGameSystem::GetInstance()->Push_InOrder(this);

                iPatternCnt++;
            }
            
        }
        
    }
    else if (m_bDoPattern) {
        m_fPatternTimeElapsed += CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_Free");

        if (m_fPatternTimeElapsed >= m_fPatternTime) {
            m_fPatternTimeElapsed = 0.f;
            CTimerMgr::GetInstance()->Resume_Timer(L"Timer_FPS");
            m_bDoPattern = FALSE;
        }
    }
    
    // Ready And Go
    if (m_bIsEnter && !m_bIsBoardOpen) {
        
        // sound
        if (m_eCurrUI == GAME_READY) {

            if (m_iPlayReadyCnt == 0) {
                m_pReadyChannel =
                    CSoundMgr::GetInstance()
                    ->Play_Sound(LEVEL_READY1, LEVEL_READY_CHANNEL, TRUE);
                m_iPlayReadyCnt++;
            }

            if (!CSoundMgr::GetInstance()->Get_IsPlaying(m_pReadyChannel)) {
                m_eCurrUI = GAME_START;
                CSoundMgr::GetInstance()->Stop_Sound(LEVEL_READY_CHANNEL, m_pReadyChannel);
            }
        }
        else if (m_eCurrUI == GAME_START) {

            if (m_iPlayStartCnt == 0) {
                m_pStartChannel =
                    CSoundMgr::GetInstance()
                    ->Play_Sound(LEVEL_GO, LEVEL_READY_CHANNEL, TRUE);
                m_iPlayStartCnt++;
            }

            if (!CSoundMgr::GetInstance()->Get_IsPlaying(m_pStartChannel)) {
                m_eCurrUI = GAME_PLAY;
                CSoundMgr::GetInstance()->Stop_Sound(LEVEL_READY_CHANNEL, m_pStartChannel);
            }
        }
        else if (m_eCurrUI == GAME_PLAY) {
            m_bIsEnter = false;
            CTimerMgr::GetInstance()->Resume_Timer(L"Timer_FPS");

            if (m_szCurrStage == "Stage1") {
                m_pBGMChannel = CSoundMgr::GetInstance()->Play_Sound(STAGE1_BGM, STAGE_BGM_CHANNEL, true, 0.1f);
            }
            else if (m_szCurrStage == "Stage2") {
                m_pBGMChannel = CSoundMgr::GetInstance()->Play_Sound(STAGE2_BGM, STAGE_BGM_CHANNEL, true, 0.1f);
            }
            else if (m_szCurrStage == "Stage3") {
                m_pBGMChannel = CSoundMgr::GetInstance()->Play_Sound(STAGE3_BGM, STAGE_BGM_CHANNEL, true, 0.1f);
            }
            else if (m_szCurrStage == "Stage4") {
                m_pBGMChannel = CSoundMgr::GetInstance()->Play_Sound(STAGE4_BGM, STAGE_BGM_CHANNEL, true, 0.1f);
            }
        }
    }

    if (m_eCurrUI == GAME_END) {
        const _float fTimer_Free = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_Free");
        if (!m_pTimeUpChannel) {
            CSoundMgr::GetInstance()->Stop_All();

            m_pTimeUpChannel =
                CSoundMgr::GetInstance()->
                Play_Sound(
                    TIME_UP, TIME_UP_CHANNEL, true, 0.1f
                );
        }

        if (m_fEndGameUITimeElapsed <= m_fEndGameUITimeInterval) {
            m_fEndGameUITimeElapsed += fTimer_Free;
        }
        else {
            CUi_StarScore* pStarScore =
                dynamic_cast<CUi_StarScore*>(
                        CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_StarScore")
                    );
            CInGameSystem* pSystem = CInGameSystem::GetInstance();

            pStarScore->Set_DeliveredScore(pSystem->Get_SuccessScore());
            pStarScore->Set_FailedScore(pSystem->Get_FailScore());
            pStarScore->Set_TotalScore(pSystem->Get_Score());
            _int iStarCnt = pSystem->Culc_Star(m_szCurrStage, pStarScore);

            if (m_iResultStartCnt == 0) {
                CSoundMgr::GetInstance()->Play_Sound(RESULT_BGM, REUSLT_BGM_CHANNEL, true, 0.1f);
                m_iResultStartCnt++;
            }

            pStarScore->Show();

            if (GetAsyncKeyState(VK_RETURN)) {                    
                if (FAILED(CManagement::GetInstance()->Back_Select()))
                    return E_FAIL;

                auto StageVec = CSelectGameSystem::GetInstance()->Get_ClearStageMap();
                _int CurStageNum = CSelectGameSystem::GetInstance()->Get_CurStageNum();
                CSoundMgr::GetInstance()->Stop_Sound(REUSLT_BGM_CHANNEL);
                auto& StageInfo = (*StageVec)[CurStageNum];
                if (iStarCnt != -1 && iStarCnt != 0) { // <<클리어 조건 달성시 
                    CSelectGameSystem::GetInstance()->Set_NeedFocus(true);
                    StageInfo.bClear = true;
                    
                };

                //StageInfo.iStar = pSystem->Get_Score();
                StageInfo.iStar = iStarCnt;
                return iResult;
            }
        }
    }

    if (pTimeUI->Get_TimeOut()) {
        CTimerMgr::GetInstance()->Stop_Timer(L"Timer_FPS");
        m_eCurrUI = GAME_END;
    }

    if (GetAsyncKeyState('B')) {
        CSoundMgr::GetInstance()->Stop_All();
        if (FAILED(CManagement::GetInstance()->Back_Select()))
            return E_FAIL;

        return iResult;
    }

    return iResult;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
    CEffectMgr::GetInstance()->LateUpdate_Effect(fTimeDelta);

}

void CStage::Render_Scene()
{
    CUi_TimeOut* pTimeUI = 
        dynamic_cast<CUi_TimeOut*>(
            CManagement::GetInstance()->Get_GameObject
                (L"UI_Layer", L"Ui_TimeOut")
            );

    CLayer* pLayer = nullptr;

    for (auto& val : m_mapLayer) {
        if (lstrcmpW(val.first, L"GameObject_Layer") == 0) {
            pLayer = val.second;
        }
    }    

    if (pLayer && m_bDoPattern) {
        CinematicCamera* pPlayer1Camera = dynamic_cast<CinematicCamera*>(pLayer->Get_GameObject(L"CinematicCamera1"));
        CinematicCamera* pPlayer2Camera = dynamic_cast<CinematicCamera*>(pLayer->Get_GameObject(L"CinematicCamera2"));

        CGameObject* pPlayer1 = dynamic_cast<CinematicCamera*>(pLayer->Get_GameObject(L"Player1"));
        CGameObject* pPlayer2 = dynamic_cast<CinematicCamera*>(pLayer->Get_GameObject(L"Player2"));

        D3DVIEWPORT9 fullViewPort{};
        fullViewPort.X = 0;
        fullViewPort.Y = 0;
        fullViewPort.Width = WINCX;
        fullViewPort.Height = WINCY * 0.5f;
        fullViewPort.MinZ = 0.f;
        fullViewPort.MaxZ = 1.f;

        D3DVIEWPORT9 viewPort1{};
        viewPort1.X = 0;
        viewPort1.Y = WINCY * 0.5f;
        viewPort1.Width = WINCX * 0.5f;
        viewPort1.Height = WINCY * 0.5f;;
        viewPort1.MinZ = 0.f;
        viewPort1.MaxZ = 1.f;

        D3DVIEWPORT9 viewPort2{};
        viewPort2.X = WINCX * 0.5f;
        viewPort2.Y = WINCY * 0.5f;
        viewPort2.Width = WINCX * 0.5f;
        viewPort2.Height = WINCY * 0.5f;;
        viewPort2.MinZ = 0.f;
        viewPort2.MaxZ = 1.f;

        if (pPlayer1Camera && pPlayer2Camera
            ) {
            m_pGraphicDev->GetViewport(&m_matStoreViewPort);
            m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &m_matStoreProjection);
            m_pGraphicDev->GetTransform(D3DTS_VIEW, &m_matStoreView);

            m_pGraphicDev->SetViewport(&viewPort1);
            m_pGraphicDev->SetTransform(D3DTS_VIEW, pPlayer1Camera->Get_View());
            m_pGraphicDev->SetTransform(D3DTS_PROJECTION, pPlayer1Camera->Get_Projection());
            CRenderer::GetInstance()->Render_GameObject(m_pGraphicDev, FALSE);

            m_pGraphicDev->SetViewport(&viewPort2);
            m_pGraphicDev->SetTransform(D3DTS_VIEW, pPlayer2Camera->Get_View());
            m_pGraphicDev->SetTransform(D3DTS_PROJECTION, pPlayer2Camera->Get_Projection());
            CRenderer::GetInstance()->Render_GameObject(m_pGraphicDev, FALSE);

            m_pGraphicDev->SetViewport(&m_matStoreViewPort);
            m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matStoreView);
            m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matStoreProjection);
        }
    }
    
    

    if (!m_bIsBoardOpen) {
        switch (m_eCurrUI)
        {
        case CStage::GAME_READY:
            pTimeUI->Set_Ready(TRUE);
            pTimeUI->Set_Go(FALSE);
            break;
        case CStage::GAME_START:
            pTimeUI->Set_Ready(FALSE);
            pTimeUI->Set_Go(TRUE);
            break;
        case CStage::GAME_PLAY:
            pTimeUI->Set_Ready(FALSE);
            pTimeUI->Set_Go(FALSE);
            break;
        case CStage::GAME_END:
            pTimeUI->Set_Ready(FALSE);
            pTimeUI->Set_Go(FALSE);
            break;
        case CStage::UI_PHASE_MAX:
            break;
        default:
            break;
        }
    }
}


HRESULT CStage::Ready_Light()
{
    D3DLIGHT9   tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

    tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

    tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Direction = { 0.f, -1.f, 1.f };

    if (FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
        return E_FAIL;

    return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStage* pLogo = new CStage(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        Safe_Release(pLogo);
        MSG_BOX("Logo Create Failed");
        return nullptr;
    }

    return pLogo;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev, string _szStageKey)
{
    CStage* pStage = new CStage(pGraphicDev, _szStageKey);

    if (FAILED(pStage->Ready_Scene()))
    {
        Safe_Release(pStage);
        MSG_BOX("Stage Create Failed");
        return nullptr;
    }

    return pStage;
}

void CStage::Free()
{
    //Safe_Delete(m_stCurrStageInfo);
    Engine::CScene::Free();
    CInGameSystem::DestroyInstance();
    CPhysicsMgr::DestroyInstance();
    CEffectMgr::GetInstance()->AllStop_Effect();
    /*Safe_Delete(m_pBGMChannel);
    Safe_Delete(m_pResultChannel);
    Safe_Delete(m_pStartChannel);
    Safe_Delete(m_pResultChannel);*/
}