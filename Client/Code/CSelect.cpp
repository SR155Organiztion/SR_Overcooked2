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

#include "CUi_StageNumber.h"
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

    

    return S_OK;
}
_int CSelect::Update_Scene(const _float& fTimeDelta) {
    _int iResult = Engine::CScene::Update_Scene(fTimeDelta);
    if (iResult == -1) 
        return iResult;
    CPhysicsMgr::GetInstance()->Update_Physics(fTimeDelta);

    
    CDynamicCamera2* pCamera = dynamic_cast<CDynamicCamera2*>(CManagement::GetInstance()->Get_GameObject(L"Environment_Layer", L"DynamicCamera2"));
    
    //최초 1회 카메라세팅
    if (!m_bCameraSet) {
        CGameObject* pPlayer = CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Bus");
        pCamera->Set_Target(pPlayer);
        pCamera->Set_Perspective(CDynamicCamera2::PERSPECTIVE::THIRD);
        pCamera->Set_Offset(0.f, 4.5f, -3.f);
        m_bCameraSet = true;
    }

    pCamera->Update_GameObject(fTimeDelta);

    // ?꾩떆 ?명뭼
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


    // ?Œë ˆ?´ì–´ë¥??°ë¼?¤ë‹ˆ??ì¹´ë©”??
    if (!m_bIsMovingToNextFlag) {
        if (pCamera1) {
            CTimerMgr::GetInstance()->Resume_Timer(L"Timer_FPS");
            pCamera1->On_Focus(&vPlayerPos);
        }
    }
    else {
        // ?¤í…Œ?´ì? ê¹ƒë°œ??ê°€ë¦¬í‚¤??ì¹´ë©”??
        CFlag* pFlag = CSelectGameSystem::GetInstance()->Get_FlagByStageNum(m_iNextFlag);

        if (pFlag) {
            _vec3 vFlagPos;
            CTransform* pTransform = dynamic_cast<CTransform*>(
                    pFlag->Get_Component(ID_DYNAMIC, L"Com_Transform")
                );

            pTransform->Get_Info(INFO_POS, &vFlagPos);

            if (!m_bIsMovingToNextFlagEnd) {
                if (pCamera1->Move_To_And_Focus(&vFlagPos)) {
                    CTimerMgr::GetInstance()->Stop_Timer(L"Timer_FPS");
                    m_bIsMovingToNextFlagEnd = true;
                }
            }
            else {
                _vec3 vTargetEye = { vPlayerPos.x, vPlayerPos.y + 6.f, vPlayerPos.z - 5.f };
                CSelectGameSystem::GetInstance()->Find_By_Euclidean(&vFlagPos);
                if (pCamera1->Move_To(&vTargetEye)) {
                    m_bIsMovingToNextFlag = false;
                }
            }

    CUi_StageNumber* pStageNumber = dynamic_cast<CUi_StageNumber*>(
        CManagement::GetInstance()->Get_GameObject(L"UI_Layer", L"Ui_SelectNumber"));

    static int cnt = 0;
    if (cnt == 0) {
        cnt++;
        for (auto* Flag : *(CSelectGameSystem::GetInstance()->Get_FlagVec())) {
            _vec3 vPos = Flag->Get_Pos();
            vPos += {0.f, -0.3f, -0.5f};
            pStageNumber->Make_StageNumber((Flag->Get_StageNum()), vPos);

        }
    }
    
    //CDynamicCamera* pCamera1 = dynamic_cast<CDynamicCamera*>(
    //        CManagement::GetInstance()->Get_GameObject(L"Environment_Layer", L"DynamicCamera")
    //    );
    //CBus* pPlayer = dynamic_cast<CBus*>(
    //        CManagement::GetInstance()->Get_GameObject(L"GameObject_Layer", L"Bus")
    //    );
    //_vec3 vPlayerPos;
    //
    //if (pPlayer) {
    //    CTransform* pPlayerTransform = dynamic_cast<CTransform*>(
    //        pPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform")
    //        );
    //    pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
    //}
    //
    //// ?뚮젅?댁뼱瑜??곕씪?ㅻ땲??移대찓??
    //if (!m_bIsMovingToNextFlag) {
    //    if (pCamera1) {
    //        CTimerMgr::GetInstance()->Resume_Timer(L"Timer_FPS");
    //        pCamera1->On_Focus(&vPlayerPos);
    //    }
    //}
    //else {
    //    // ?ㅽ뀒?댁? 源껊컻??媛由ы궎??移대찓??
    //    CFlag* pFlag = CSelectGameSystem::GetInstance()->Get_FlagByStageNum(m_iNextFlag);
    //
    //    if (pFlag) {
    //        _vec3 vFlagPos;
    //        CTransform* pTransform = dynamic_cast<CTransform*>(
    //                pFlag->Get_Component(ID_DYNAMIC, L"Com_Transform")
    //            );
    //
    //        pTransform->Get_Info(INFO_POS, &vFlagPos);
    //
    //        if (!m_bIsMovingToNextFlagEnd) {
    //            if (pCamera1->Move_To_And_Focus(&vFlagPos)) {
    //                CTimerMgr::GetInstance()->Stop_Timer(L"Timer_FPS");
    //                m_bIsMovingToNextFlagEnd = true; 
    //            }
    //        }
    //        else {
    //            _vec3 vTargetEye = { vPlayerPos.x, vPlayerPos.y + 6.f, vPlayerPos.z - 5.f };
    //            CSelectGameSystem::GetInstance()->Find_By_Euclidean(&vFlagPos);
    //            if (pCamera1->Move_To(&vTargetEye)) {
    //                m_bIsMovingToNextFlag = false;
    //            }
    //        }
    //    }
    //}
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
    dynamic_cast<CBus*>(pGameObject)->Set_FirstPos(10.f, 0.f, 10.f);
    dynamic_cast<CBus*>(pGameObject)->Set_FirstScale(0.5f, 1.f, 1.f);
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
    return S_OK;
}
HRESULT	CSelect::Ready_UI_Layer(const _tchar* pLayerTag) {
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
    Engine::CGameObject* pGameObject = nullptr;

    //½ºÅ×ÀÌÁö ¹øÈ£
    pGameObject = CUi_Factory<CUi_StageNumber>::Ui_Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL; 
    if (FAILED(pLayer->Add_GameObject(L"Ui_SelectNumber", pGameObject)))
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