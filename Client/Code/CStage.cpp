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
#include "CEffect.h"

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
#include "CDishStation.h"
#include "CFloor.h"

#include "CFakePlayer.h"
#include "CLettuceTemp.h"
#include "CPhysicsMgr.h"
#include "CEmptyStationTemp.h"


#include "CUi_Factory.h"
#include "Engine_Define.h"

#include "CInteractMgr.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{

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

    // dynamicCamera
    _vec3	vEye{ 0.f, 10.f, -10.f };
    _vec3	vAt{ 0.f, 0.f, 1.f };
    _vec3	vUp{ 0.f , 1.f, 0.f };
    pGameObject = CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
        return E_FAIL;

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

   // pGameObject = CPlayer::Create(m_pGraphicDev);
   // if (nullptr == pGameObject)
   //     return E_FAIL;
   // if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
   //     return E_FAIL;

    /*pGameObject = CRealPlayer::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
        return E_FAIL;*/

    // 테스트용 가짜 플레이어
    pGameObject = CFakePlayer::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
        return E_FAIL;

    pGameObject = CMonster::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Monster", pGameObject)))
        return E_FAIL;

    //pGameObject = CSeaweed::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Ingredient_Seaweed", pGameObject)))
    //    return E_FAIL;
     
    pGameObject = CTomato::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Tomato", pGameObject)))
        return E_FAIL;

    //pGameObject = CShrimp::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Ingredient_Shrimp", pGameObject)))
    //    return E_FAIL;

    //pGameObject = CPasta::Create(m_pGraphicDev);
    /*pGameObject = CLettuce::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Lettuce", pGameObject)))
        return E_FAIL;*/

    pGameObject = CLettuceTemp::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_LettuceTemp", pGameObject)))
        return E_FAIL;

    pGameObject = CPasta::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Pasta", pGameObject)))
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

    pGameObject = CGasStation::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Station_Gas", pGameObject)))
        return E_FAIL;

    pGameObject = CEmptyStation::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Station_Empty", pGameObject)))
        return E_FAIL;

    pGameObject = CDishStation::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Station_Plate", pGameObject)))
        return E_FAIL;

    //pGameObject = CEmptyStationTemp::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Station_Empty", pGameObject)))
    //    return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    
    Engine::CGameObject* pGameObject = nullptr;

    
    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, START_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button", pGameObject)))
        return E_FAIL;

    /*for (_uint i = 0; i < 50; ++i)
    {
        pGameObject = CEffect::Create(m_pGraphicDev);
        if (nullptr == pGameObject)
            return E_FAIL;

        if (FAILED(pLayer->Add_GameObject(L"Effect", pGameObject)))
            return E_FAIL;
    }*/
    
   

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
    _int iResult = Engine::CScene::Update_Scene(fTimeDelta);
    CPhysicsMgr::GetInstance()->Update_Physics(fTimeDelta);
    return iResult;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{
   
}


HRESULT CStage::Ready_Light()
{
    D3DLIGHT9   tLightInfo;
    ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

    tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

    tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tLightInfo.Direction = { 1.f, -1.f, 1.f };

    if(FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
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

void CStage::Free()
{

    Engine::CScene::Free();
}
