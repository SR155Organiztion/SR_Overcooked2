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
#include "CFryingpan.h"
#include "CPot.h"
#include "CPlate.h"
#include "CIngredientStation.h"
#include "CChopStation.h"
#include "CGasStation.h"
#include "CEmptyStation.h"
#include "CFloor.h"
#include "CInvisibleStation.h"
#include "CCleanPlateStation.h"

#include "CFakePlayer.h"
#include "CLettuceTemp.h"
#include "CPhysicsMgr.h"
#include "CEmptyStationTemp.h"

#include "CUi_Factory.h"
#include "CUi_Timer.h"
#include "CUi_Score.h"
#include "CUi_OrderMgr.h"
#include "CUi_Icon.h"
#include "Engine_Define.h"

#include "CInteractMgr.h"
#include "CFontMgr.h"
#include "CDirtyPlateStation.h"
#include <CSinkStation.h>
#include <CTrashStation.h>
#include <CServingStation.h>
#include "CUtil.h"
#include "CInGameSystem.h"

_tchar szStr[128] = L"";

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
}

HRESULT CStage::Ready_Scene()
{
    if (FAILED(
        CInGameSystem::GetInstance()->Ready_CInGameSystem
        (m_szCurrStage, m_pGraphicDev))) {
        return E_FAIL;
    }
    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    if (FAILED(Ready_Light()))
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

    pGameObject = CTerrain::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
        return E_FAIL;

    // dynamicCamera

    _float fWidth =
        dynamic_cast<CVIBuffer*>(
            pGameObject->Get_Component(
                COMPONENTID::ID_STATIC, L"Com_Buffer"
            )
            )->Get_Width() * 0.5f;
    _vec3	vEye{ fWidth, 10.f, -3.f };
    _vec3	vAt{ fWidth, 0.f, 4.f };
    _vec3	vUp{ 0.f , 1.f, 0.f };
    pGameObject = CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
        return E_FAIL;

    pGameObject = CFloor::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Environment_Floor", pGameObject)))
        return E_FAIL;


    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_GameObject_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    pGameObject = CRealPlayer::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
        return E_FAIL;

    // Ingredient_Object
    pGameObject = CSeaweed::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Seaweed", pGameObject)))
        return E_FAIL;

    pGameObject = CLettuce::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Lettuce", pGameObject)))
        return E_FAIL;

    pGameObject = CTomato::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Tomato", pGameObject)))
        return E_FAIL;

    pGameObject = CCucumber::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Cucumber", pGameObject)))
        return E_FAIL;

    //pGameObject = CLettuceTemp::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Ingredient_LettuceTemp", pGameObject)))
    //    return E_FAIL;

    pGameObject = CFish::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Fish", pGameObject)))
        return E_FAIL;

    pGameObject = CShrimp::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Shrimp", pGameObject)))
        return E_FAIL;

    pGameObject = CRice::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Rice", pGameObject)))
        return E_FAIL;

    pGameObject = CPasta::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Pasta", pGameObject)))
        return E_FAIL;

    // Tool_Object
    pGameObject = CPlate::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Tool_Plate", pGameObject)))
        return E_FAIL;

    pGameObject = CFryingpan::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Tool_Fryingpan", pGameObject)))
        return E_FAIL;

    pGameObject = CPot::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Tool_Pot", pGameObject)))
        return E_FAIL;
  
    // Station_Object
    //pGameObject = CIngredientStation::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Station_Ingredient", pGameObject)))
    //    return E_FAIL;

    //pGameObject = CChopStation::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Station_Chop", pGameObject)))
    //    return E_FAIL;

    //pGameObject = CGasStation::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Station_Gas", pGameObject)))
    //    return E_FAIL;

    //pGameObject = CEmptyStation::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Station_Empty", pGameObject)))
    //    return E_FAIL;

    //pGameObject = CEmptyStationTemp::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Station_Empty", pGameObject)))
    //    return E_FAIL;

    CInGameSystem::GetInstance()->Parse_GameObjectData(pLayer);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;
      Engine::CGameObject* pGameObject = nullptr;

    ///////////////////////////////////////////////////////////////////////////////////// UI_Object
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

    //레시피
        pGameObject = CUi_Factory<CUi_Order>::Ui_Create(m_pGraphicDev);
        if (nullptr == pGameObject) return E_FAIL;
        if (FAILED(pLayer->Add_GameObject(L"Ui_Object8", pGameObject)))
            return E_FAIL;

      //아이콘
         pGameObject = CUi_Factory<CUi_Icon>::Ui_Create(m_pGraphicDev);
        if (nullptr == pGameObject) return E_FAIL;
        if (FAILED(pLayer->Add_GameObject(L"Ui_Object9", pGameObject)))
            return E_FAIL;

    /*pGameObject = CUi_Factory<CUi_Order>::Ui_Create(m_pGraphicDev, GAUGE_OBJECT);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object9", pGameObject)))
        return E_FAIL;
    pGameObject = CUi_Factory<CUi_Order>::Ui_Create(m_pGraphicDev, IMAGE_OBJECT);
    if (nullptr == pGameObject) return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object10", pGameObject)))
        return E_FAIL;*/

    
    /*{
        pGameObject = CDummyEffect::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;

        if (FAILED(pLayer->Add_GameObject(L"DummyEffect", pGameObject)))
            return E_FAIL;
    }*/



    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
    _int iResult = Engine::CScene::Update_Scene(fTimeDelta);
    CPhysicsMgr::GetInstance()->Update_Physics(fTimeDelta);
    CInGameSystem::GetInstance()->Update_InGameSystem(fTimeDelta, this);
    return iResult;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{
    _vec2   vPos{ 100.f, 100.f };
    CFontMgr::GetInstance()->Render_Font(L"Font_Default", szStr, &vPos, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
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
}