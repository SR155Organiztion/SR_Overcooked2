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
#include "CFloor.h"

#include "CFakePlayer.h"
#include "CLettuceTemp.h"
#include "CPhysicsMgr.h"
#include "CEmptyStationTemp.h"

#include "CUi_Factory.h"
#include "CUi_TimeLimit.h"
#include "Engine_Define.h"

#include "CInteractMgr.h"
#include "CFontMgr.h"

_tchar szStr[128] = L"";

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

    // pGameObject = CPlayer::Create(m_pGraphicDev);
    // if (nullptr == pGameObject)
    //     return E_FAIL;
    // if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
    //     return E_FAIL;

    pGameObject = CRealPlayer::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
        return E_FAIL;

    // 테스트용 가짜 플레이어
    //pGameObject = CFakePlayer::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
    //    return E_FAIL;

    pGameObject = CMonster::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Monster", pGameObject)))
        return E_FAIL;

    pGameObject = CSeaweed::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ingredient_Seaweed", pGameObject)))
        return E_FAIL;

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

    //pGameObject = CFryingpan::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Tool_Fryingpan", pGameObject)))
    //    return E_FAIL;

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

    //pGameObject = CEmptyStationTemp::Create(m_pGraphicDev);
    //if (nullptr == pGameObject)
    //    return E_FAIL;
    //if (FAILED(pLayer->Add_GameObject(L"Station_Empty", pGameObject)))
    //    return E_FAIL;

    Parse_Json(pLayer);

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
    ///���ѽð�
    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, TIMER_OBJECT);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, TIMEGAUGE_OBJECT);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object2", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, TIMEFONT_OBJECT);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object3", pGameObject)))
        return E_FAIL;

    ////////////////////////////////////////////////////////////////////////////////UI_Button
    //���� ��ư
    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, STORY_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button0", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, STORY_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button1", pGameObject)))
        return E_FAIL;


    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, ARCADE_BUTTON);
    //이케이드 버튼
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button2", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, ARCADE_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button3", pGameObject)))
        return E_FAIL;

    //���� ��ư
    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, BATTLE_BUTTON);
    //대전 버튼
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button4", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, BATTLE_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button5", pGameObject)))
        return E_FAIL;

    //�丮�� ��ư
    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, CHEF_BUTTON);
    //요리사 버튼
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button6", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, CHEF_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button7", pGameObject)))
        return E_FAIL;

    //���� ��ư
    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, STATE_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button8", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_Button>::Ui_Create(m_pGraphicDev, STATE_BUTTON);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Button9", pGameObject)))
        return E_FAIL;

    ///////////////////////////////////////////////////////////////////////////////////// UI_Object
    ///제한시간
    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, TIMER_OBJECT);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, TIMEGAUGE_OBJECT);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, TIMEFONT_OBJECT);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object", pGameObject)))
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
    tLightInfo.Direction = { 1.f, -1.f, 1.f };

    if (FAILED(CLightMgr::GetInstance()->Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
        return E_FAIL;



    return S_OK;
}

HRESULT CStage::Parse_Json(CLayer* _pLayer)
{
    Engine::CGameObject* pGameObject = nullptr;
    // Json 기반 데이터
    vector<S_BLOCK> vecBlock = CMapTool::GetInstance()->Get_Data("None").Block;
    CTransform* pTransform = nullptr;
    int iBlockIdx = 0;
    for (S_BLOCK block : vecBlock) {
        if (block.Block_Type == "Empty") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Empty%d", iBlockIdx++);

            Parse_Position<CEmptyStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "InvWall") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"InvWall%d", iBlockIdx++);

            Parse_Position<CEmptyStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Gas") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Gas%d", iBlockIdx++);

            
            Parse_Position<CGasStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Chop") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Chop%d", iBlockIdx++);

            Parse_Position<CChopStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }
    }
}

void CStage::Parse_Direction(CTransform* _pTrans, string _szDir)
{
    if (_szDir == "PX") {
        _vec3 vLook = { 1.f, 0.f, 0.f };
        _pTrans->Set_Look(&vLook);
    }
    else if (_szDir == "NX") {
        _vec3 vLook = { -1.f, 0.f, 0.f };
        _pTrans->Set_Look(&vLook);
    }
    else if (_szDir == "PZ") {
        _vec3 vLook = { 0.f, 0.f, 1.f };
        _pTrans->Set_Look(&vLook);
    }
    else {
        _vec3 vLook = { 0.f, 0.f, -1.f };
        _pTrans->Set_Look(&vLook);
    }
}

template<typename T>
void CStage::Parse_Position(
    S_BLOCK _stBlock
    , CGameObject** _pGameObject)
{
    
    *_pGameObject = T::Create(m_pGraphicDev);
    CTransform* pTransform =
        dynamic_cast<CTransform*>(
            (*_pGameObject)->Get_Component(
                    COMPONENTID::ID_DYNAMIC, L"Com_Transform"
                )
            );

    pTransform->Set_Pos(
        _stBlock.vPos.x
        , _stBlock.vPos.y
        , _stBlock.vPos.z
    );

    Parse_Direction(pTransform, _stBlock.Direction);
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