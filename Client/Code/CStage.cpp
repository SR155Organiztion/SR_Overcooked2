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
#include "CUi_TimeLimit.h"
#include "CUi_Score.h"
#include "CUi_OrderMgr.h"
#include "Engine_Define.h"

#include "CInteractMgr.h"
#include "CFontMgr.h"
#include "CDirtyPlateStation.h"
#include <CSinkStation.h>
#include <CTrashStation.h>
#include <CServingStation.h>

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
    //제한시간
    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, IMAGE_GAUGE);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object1", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, LODING_GAUGE);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object2", pGameObject)))
        return E_FAIL;

    pGameObject = CUi_Factory<CUi_TimeLimit>::Ui_Create(m_pGraphicDev, FONT_GAUGE);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Ui_Object3", pGameObject)))
        return E_FAIL;

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
    for (int i= 0; i < 4; ++i)
    {
        pGameObject = CUi_Factory<CUi_Order>::Ui_Create(m_pGraphicDev);
        if (nullptr == pGameObject) return E_FAIL;
        if (FAILED(pLayer->Add_GameObject(L"Ui_Object8", pGameObject)))
            return E_FAIL;
    }

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

HRESULT CStage::Parse_Json(CLayer* _pLayer)
{
    Engine::CGameObject* pGameObject = nullptr;
    // Json 기반 데이터
    if (m_szCurrStage.empty()) {
        MSG_BOX("스테이지 정보가 없습니다.");
        return E_FAIL;
    }

    // block
    vector<S_BLOCK> vecBlock = CMapTool::GetInstance()->Get_Data(m_szCurrStage).Block;
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

            Parse_Position<CInvisibleStation>(block, &pGameObject);

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
        /*else if (block.Block_Type == "Plate") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Plate%d", iBlockIdx++);

            Parse_Position<CDirtyPlateStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }*/
        /*else if (block.Block_Type == "Sink_Wash") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Sink_Wash%d", iBlockIdx++);

            Parse_Position<CSinkStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }*/
        /*else if (block.Block_Type == "Sink_Plate") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Sink_Plate%d", iBlockIdx++);

            Parse_Position<CCleanPlateStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Trash") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Trash%d", iBlockIdx++);

            Parse_Position<CTrashStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }*/
        /*else if (block.Block_Type == "Serving") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Serving%d", iBlockIdx++);

            Parse_Position<CServingStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }*/

        //tiles
        vector<S_TILE> vecTile = CMapTool::GetInstance()->Get_Data(m_szCurrStage).Tiles;
        int iTileIdx = 0;

        for (S_TILE tile : vecTile) {
            if (tile.Tile_Type == "Tile1") {
                TCHAR szKey[128] = L"";

                wsprintf(szKey, L"Tile1%d", iTileIdx++);
                Parse_Position<CFloor>(tile, &pGameObject);
            }
            else if (tile.Tile_Type == "Tile2") {
                TCHAR szKey[128] = L"";

                wsprintf(szKey, L"Tile2%d", iTileIdx++);
                Parse_Position<CFloor>(tile, &pGameObject);
            }
            else if (tile.Tile_Type == "Tile3") {
                TCHAR szKey[128] = L"";

                wsprintf(szKey, L"Tile3%d", iTileIdx++);
                Parse_Position<CFloor>(tile, &pGameObject);
            }
            else if (tile.Tile_Type == "Tile5") {
                TCHAR szKey[128] = L"";

                wsprintf(szKey, L"Tile5%d", iTileIdx++);
                Parse_Position<CFloor>(tile, &pGameObject);
            }
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

template<typename T>
void CStage::Parse_Position(
    S_TILE _stTile
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
        _stTile.vPos.x
        , _stTile.vPos.y
        , _stTile.vPos.z
    );

    Parse_Direction(pTransform, _stTile.Direction);
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

    Engine::CScene::Free();
}