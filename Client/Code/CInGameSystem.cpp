#include "pch.h"
#include "CInGameSystem.h"
#include <CEmptyStation.h>
#include <CGasStation.h>
#include <CChopStation.h>
#include <CInvisibleStation.h>
#include <CDirtyPlateStation.h>
#include <CSinkStation.h>
#include <CCleanPlateStation.h>
#include <CTrashStation.h>
#include <CServingStation.h>
#include "CUi_Timer.h"
#include <CUi_Score.h>
#include "CUtil.h"
#include "CBlue33Tile.h"
#include "CBlue44Tile.h"
#include "CPink44Tile.h"
#include "CRealPlayer.h"
#include "CIngredientStation.h"
#include <CPlate.h>
#include <CFireExtinguisher.h>
#include <CFryingpan.h>
#include <CPot.h>
#include "CStoneBrownTile.h"
#include "CStoneBeigeTile.h"
#include <CCherryTree.h>
#include <CBamboo.h>
#include <CTorch.h>
#include <CHydrant.h>
#include <CCone.h>
#include <CPigeon.h>
#include <CSandbag.h>
#include <CTrafficLight.h>
#include <CTable.h>
#include <CCar.h>
#include <CBrickWall.h>
#include <CWoodWall.h>
#include <CBasket.h>
#include <CBarrier.h>

IMPLEMENT_SINGLETON(CInGameSystem)

CInGameSystem::CInGameSystem()
{
}

CInGameSystem::~CInGameSystem()
{
}


HRESULT CInGameSystem::Ready_CInGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev, CScene* _pScene)
{
    m_pGraphicDev = _pGraphicDev;
    m_stCurrStageInfo = CMapTool::GetInstance()->Get_Data(_szCurrStage);

    // 총 주문서 설정
    for (int i = 0; i < 30; i++) {
        _int iIdx = CUtil::Make_Random<_int>(0, m_stCurrStageInfo.Recipe.size()-1);
        
        CRecipeMgr::RECIPE pRecipe;
        wstring wstr = CUtil::StringToWString(m_stCurrStageInfo.Recipe[iIdx]);
        const _tchar* szStr = wstr.c_str();
        CRecipeMgr::GetInstance()->Get_Recipe(pRecipe, szStr);
    
        m_qTotalOrderRecipe.push(pRecipe);
    }

    // 제한시간 설정
    m_fTimeLimit = m_stCurrStageInfo.Time;


    return S_OK;
}

_int CInGameSystem::Update_InGameSystem(const _float& fTimeDelta, CScene* _pScene)
{
    m_fOrderTimeElapsed += fTimeDelta;

    if (m_fOrderTimeElapsed >= m_fOrderTImeInterval) {
        CGameObject* pGameObj = _pScene->Get_GameObject(L"UI_Layer", L"Ui_Object8");
        Take_Order(pGameObj);
        m_fOrderTimeElapsed = 0.f;
    }
    
    if (m_stCompleteOrder.setIngredient.size() != 0) {
        _int iScore = Compare_FoodRecipe();

        if (iScore >= 0) {
            // 조리 성공
            Setting_Score(_pScene, iScore);
            m_iSuccessCnt++;
            m_iSuccessScore += iScore;
        }
        else {
            // 조리 실패
            Setting_Score(_pScene, -20);
            m_iFailCnt++;
            m_iFailScore -= 20;
        }
        m_stCompleteOrder.setIngredient.clear();
    }
    return 0;
}

void CInGameSystem::Clear()
{
    for (int i = 0; i < m_qTotalOrderRecipe.size(); i++) {
        m_qTotalOrderRecipe.pop();
    }
}

_int CInGameSystem::Compare_FoodRecipe()
{
    for (auto iter = m_pCurrOrderRecipeList->begin()
        ; iter != m_pCurrOrderRecipeList->end();) {
        CRecipeMgr::RECIPE stCurrRecipe = (*iter).Recipe;

        if (stCurrRecipe.setIngredient.size()
            != m_stCompleteOrder.setIngredient.size()) {
            iter++;
            continue;
        }

        _int iCheckCnt = 0;

        for (wstring ingre : m_stCompleteOrder.setIngredient) {
            auto sIter = stCurrRecipe.setIngredient.find(ingre);

            // 재료 불일치
            if (sIter == stCurrRecipe.setIngredient.end()) {
                iter++;
                break;
            }
            iCheckCnt++ ;
        }

        // 일치
        if (iCheckCnt == m_stCompleteOrder.setIngredient.size()) {
            (*iter).m_bRemove = true;
            (*iter).m_bProcess = false;
            return stCurrRecipe.iPrice;
        }
    }

    return -1;
}

HRESULT CInGameSystem::Parse_GameObjectData(CLayer* _pLayer)
{
    vector<S_BLOCK> vecBlock = m_stCurrStageInfo.GameObject.Block;
    vector<S_TILE> vecTile = m_stCurrStageInfo.Environment.Tile;

    if (FAILED(Parse_BlockObjectData(_pLayer, &vecBlock))) {
        return E_FAIL;
    }
   
    if (FAILED(Parse_TileObjectData(_pLayer, &vecTile))) {
        return E_FAIL;
    }

    if (FAILED(Parse_ETCData(_pLayer))) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CInGameSystem::Parse_EnviromentData(CLayer* _pLayer) {
    vector<S_ENVOBJECT> vecEnv = m_stCurrStageInfo.Environment.EnvObject;

    Engine::CGameObject* pGameObject = nullptr;
    CTransform* pTransform = nullptr;
    int iEnvIdx = 0;

    for (S_ENVOBJECT env : vecEnv) {
        if (env.Env_Type == "CherryTree") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"CherryTree%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CCherryTree>(env, &pGameObject);
            Parse_Scale<CCherryTree>(env, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Bamboo") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Bamboo%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CBamboo>(env, &pGameObject);
            Parse_Scale<CBamboo>(env, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Torch") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Torch%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CTorch>(env, &pGameObject);
            Parse_Scale<CTorch>(env, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Hydrant") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Hydrant%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CHydrant>(env, &pGameObject);
            Parse_Scale<CHydrant>(env, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Cone") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Cone%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CCone>(env, &pGameObject);
            Parse_Scale<CCone>(env, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Pigeon") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Pigeon%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CPigeon>(env, &pGameObject);
            Parse_Scale<CPigeon>(env, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Sandbag") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Sandbag%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CSandbag>(env, &pGameObject);
            Parse_Scale<CSandbag>(env, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "TrafficLight_1") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"TrafficLight_1%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CTrafficLight>(env, &pGameObject);
            Parse_Scale<CTrafficLight>(env, &pGameObject);
            dynamic_cast<CTrafficLight*>(pGameObject)->Set_Texture(CTrafficLight::L_FWD);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "TrafficLight_2") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"TrafficLight_2%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CTrafficLight>(env, &pGameObject);
            Parse_Scale<CTrafficLight>(env, &pGameObject);
            dynamic_cast<CTrafficLight*>(pGameObject)->Set_Texture(CTrafficLight::R_FWD);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "TrafficLight_3") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"TrafficLight_3%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CTrafficLight>(env, &pGameObject);
            Parse_Scale<CTrafficLight>(env, &pGameObject);
            dynamic_cast<CTrafficLight*>(pGameObject)->Set_Texture(CTrafficLight::R_LEFT);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Table_1") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Table_1%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CTable>(env, &pGameObject);
            Parse_Scale<CTable>(env, &pGameObject);
            dynamic_cast<CTable*>(pGameObject)->Set_Texture(CTable::BLACK);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Table_2") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Table_2%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CTable>(env, &pGameObject);
            Parse_Scale<CTable>(env, &pGameObject);
            dynamic_cast<CTable*>(pGameObject)->Set_Texture(CTable::WHITE);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Table_3") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Table_3%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CTable>(env, &pGameObject);
            Parse_Scale<CTable>(env, &pGameObject);
            dynamic_cast<CTable*>(pGameObject)->Set_Texture(CTable::GREEN);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Table_4") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Table_4%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CTable>(env, &pGameObject);
            Parse_Scale<CTable>(env, &pGameObject);
            dynamic_cast<CTable*>(pGameObject)->Set_Texture(CTable::CHECK);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "StoneWall_1") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"StoneWall_1%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CBrickWall>(env, &pGameObject);
            Parse_Scale<CBrickWall>(env, &pGameObject);
            dynamic_cast<CBrickWall*>(pGameObject)->Set_Texture(CBrickWall::REDBROWN);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "StoneWall_2") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"StoneWall_2%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CBrickWall>(env, &pGameObject);
            Parse_Scale<CBrickWall>(env, &pGameObject);
            dynamic_cast<CBrickWall*>(pGameObject)->Set_Texture(CBrickWall::PINKBROWN);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
                }
        else if (env.Env_Type == "WoodWall") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"WoodWall%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CWoodWall>(env, &pGameObject);
            Parse_Scale<CWoodWall>(env, &pGameObject);
            //dynamic_cast<CWoodWall*>(pGameObject)->Set_Texture(CTable::CHECK);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Basket") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Basket%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CBasket>(env, &pGameObject);
            Parse_Scale<CBasket>(env, &pGameObject);
            //dynamic_cast<CCar*>(pGameObject)->Set_Texture(CTable::CHECK);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (env.Env_Type == "Barrier") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Barrier%d", iEnvIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CBarrier>(env, &pGameObject);
            Parse_Scale<CBarrier>(env, &pGameObject);
            //dynamic_cast<CCar*>(pGameObject)->Set_Texture(CTable::CHECK);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
    }
}

HRESULT CInGameSystem::Parse_BlockObjectData(CLayer* _pLayer, vector<S_BLOCK>* _pVecBlock)
{
    Engine::CGameObject* pGameObject = nullptr;
    CTransform* pTransform = nullptr;
    int iBlockIdx = 0;

    for (S_BLOCK block : *_pVecBlock) {
        if (block.Block_Type == "Empty") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Empty%d", iBlockIdx++);
            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CEmptyStation>(block, &pGameObject);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "InvWall") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"InvWall%d", iBlockIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CInvisibleStation>(block, &pGameObject);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Gas") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Gas%d", iBlockIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CGasStation>(block, &pGameObject);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Chop") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Chop%d", iBlockIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CChopStation>(block, &pGameObject);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Plate") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Plate%d", iBlockIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CDirtyPlateStation>(block, &pGameObject);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Sink_Wash") {
            Parse_Position<CSinkStation>(block, &pGameObject);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(L"Sink_Wash", pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Sink_Plate") {
            Parse_Position<CCleanPlateStation>(block, &pGameObject);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(L"Sink_Plate", pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Trash") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Trash%d", iBlockIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CTrashStation>(block, &pGameObject);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (block.Block_Type == "Serving") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Serving%d", iBlockIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CServingStation>(block, &pGameObject);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Creater%d", iBlockIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            pGameObject = CIngredientStation::Create(m_pGraphicDev);
            CIngredientStation * pStation = dynamic_cast<CIngredientStation*>(pGameObject);
            
            const _tchar* szType = CUtil::ConvertToWChar(block.Block_Type);
            pStation->Set_TypeIngredientStation(szType);

            CTransform* pTransform =
                dynamic_cast<CTransform*>(
                        (pGameObject)->Get_Component(
                            COMPONENTID::ID_DYNAMIC, L"Com_Transform"
                        )
                    );

            pTransform->Set_Pos(
                block.vPos.x
                , block.vPos.y
                , block.vPos.z
            );

            Parse_Direction(pTransform, block.Direction);
            Parse_OnStationToolData(_pLayer, &block, pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
            }
    }

    return S_OK;
}

HRESULT CInGameSystem::Parse_TileObjectData(CLayer* _pLayer, vector<S_TILE>* _pVecTile)
{
    Engine::CGameObject* pGameObject = nullptr;
    CTransform* pTransform = nullptr;
    int iTileIdx = 0;

    for (S_TILE tile : *_pVecTile) {
        if (tile.Tile_Type == "Tile_Blue33") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Tile_Blue33_%d", iTileIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CBlue33Tile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (tile.Tile_Type == "Tile_Blue44") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Tile_Blue44_%d", iTileIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CBlue44Tile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (tile.Tile_Type == "Tile_Pink44") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Tile_Pink44_%d", iTileIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CPink44Tile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (tile.Tile_Type == "Tile_StoneBrown") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Tile_StoneBrown_%d", iTileIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CStoneBrownTile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (tile.Tile_Type == "Tile_StoneBeige") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Tile_StoneBeige_%d", iTileIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CStoneBeigeTile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
        else if (tile.Tile_Type == "Tile_Roof") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Tile_Roof%d", iTileIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CStoneBeigeTile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }

        else if (tile.Tile_Type == "Tile_Pond") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Tile_Pond%d", iTileIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CStoneBeigeTile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }

        else if (tile.Tile_Type == "Tile_Road") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"Tile_Road%d", iTileIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CStoneBeigeTile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
                }
        
    }

    return S_OK;
}

HRESULT CInGameSystem::Parse_OnStationToolData(CLayer* _pLayer, S_BLOCK* _pBlock, CGameObject* _pGameObject)
{
    if (_pBlock->Item.empty())
        return E_FAIL;

    string szItem = _pBlock->Item;
    CGameObject* pPlaceObj = nullptr;

    IPlace* pPlace = dynamic_cast<IPlace*>(_pGameObject);

    if (!pPlace)
        return E_FAIL;

    _tchar szKey[128] = L"";

    if (szItem == "Plate") {
        static int iPlateIdx = 0;
        pPlaceObj = CPlate::Create(m_pGraphicDev);
        wsprintf(szKey, L"Plate_%d", iPlateIdx++);
    }
    else if (szItem == "Extinguisher") {
        pPlaceObj = CFireExtinguisher::Create(m_pGraphicDev);
        wsprintf(szKey, L"Extinguisher");
    }
    else if (szItem == "Frypan") {
        static int iFrypanIdx = 0;
        pPlaceObj = CFryingpan::Create(m_pGraphicDev);
        wsprintf(szKey, L"Frypan_%d", iFrypanIdx++);
    }
    else if (szItem == "Pot") {
        static int iPotIdx = 0;
        pPlaceObj = CPot::Create(m_pGraphicDev);
        wsprintf(szKey, L"Pot_%d", iPotIdx++);
    }

    if (pPlaceObj) {
        // 조리도구
        CTransform* pPlaceObjTransform = 
            dynamic_cast<CTransform*>(
                    pPlaceObj->Get_Component(
                        ID_DYNAMIC, L"Com_Transform"
                    )
                );

        pPlaceObjTransform->Set_Pos(
            _pBlock->vPos.x
            , _pBlock->vPos.y + 0.5f
            , _pBlock->vPos.z
        );
        
        pPlace->Set_Place(pPlaceObj);

        size_t len = wcslen(szKey) + 1;
        wchar_t* pKey = new wchar_t[len];
        wcscpy_s(pKey, len, szKey);

        _pLayer->Add_GameObject(pKey, pPlaceObj);
    }

    return S_OK;
}

HRESULT CInGameSystem::Parse_ETCData(CLayer* _pLayer)
{
    Engine::CGameObject* pGameObject = nullptr;

    if (m_stCurrStageInfo.Player.P1.y > 0.f) {
        pGameObject = CRealPlayer::Create(m_pGraphicDev);
        dynamic_cast<CRealPlayer*>(pGameObject)->Set_PlayerFirstPos(
            m_stCurrStageInfo.Player.P1.x
            , m_stCurrStageInfo.Player.P1.y
            , m_stCurrStageInfo.Player.P1.z
        );

        if (nullptr == pGameObject)
            return E_FAIL;
        if (FAILED(_pLayer->Add_GameObject(L"Player", pGameObject)))
            return E_FAIL;
    }

    return S_OK;
}

void CInGameSystem::Setting_LimitTime(CGameObject* _pGameObject1, CGameObject* _pGameObject2
    , CGameObject* _pGameObject3)
{
    dynamic_cast<CUi_Timer*>(_pGameObject3)->Set_Timer(m_fTimeLimit);
    dynamic_cast<CUi_Timer*>(_pGameObject2)->Set_Timer(m_fTimeLimit);
    dynamic_cast<CUi_Timer*>(_pGameObject1)->Set_Timer(m_fTimeLimit);
}

void CInGameSystem::Setting_Score(CScene* _pScene, _int _iScore)
{
    CGameObject* pScoreObject1 
        = _pScene->Get_GameObject(L"UI_Layer", L"Ui_Object4");

    //CGameObject* pScoreObject2
    //    = _pScene->Get_GameObject(L"UI_Layer", L"Ui_Object5");

    dynamic_cast<CUi_Score*>(pScoreObject1)->Set_Score(_iScore);
    //dynamic_cast<CUi_Score*>(pScoreObject2)->Set_Score(_iScore);
}

void CInGameSystem::Take_Order(CGameObject* _pGameObject)
{
    if (m_qTotalOrderRecipe.empty())
        return;
    if (m_pCurrOrderRecipeList->size() >= 6)
        return;
    CRecipeMgr::RECIPE recipe = m_qTotalOrderRecipe.front();
    m_qTotalOrderRecipe.pop();
    dynamic_cast<CUi_Order*>(_pGameObject)->Make_Order(recipe);
}

void CInGameSystem::Parse_Direction(CTransform* _pTrans, string _szDir)
{
    if (_szDir == "PX") {
        _pTrans->m_vAngle.y = D3DXToRadian(0.f);
    }
    else if (_szDir == "NX") {
        _pTrans->m_vAngle.y = D3DXToRadian(180.f);
    }
    else if (_szDir == "PZ") {
        _pTrans->m_vAngle.y = D3DXToRadian(90.f);
    }
    else { // "NZ"
        _pTrans->m_vAngle.y = D3DXToRadian(270.f);
    }
}

void CInGameSystem::Parse_Direction(CTransform* _pTrans, float _fAngle)
{
    _pTrans->m_vAngle.y = D3DXToRadian(_fAngle);
}

template<typename T>
void CInGameSystem::Parse_Position(
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
void CInGameSystem::Parse_Position(
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
        , _stTile.vPos.y+0.1f
        , _stTile.vPos.z
    );

    Parse_Direction(pTransform, _stTile.Direction);
}

template<typename T>
void CInGameSystem::Parse_Position(
    S_ENVOBJECT _stEnv
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
        _stEnv.vPos.x
        , _stEnv.vPos.y + 0.1f
        , _stEnv.vPos.z
    );

    Parse_Direction(pTransform, _stEnv.fAngle);
}

template<typename T>
void CInGameSystem::Parse_Scale(S_ENVOBJECT _stEnv, CGameObject** _pGameObject)
{
    dynamic_cast<T*>(*_pGameObject)->Set_Scale(
        _stEnv.vScale.x
        , _stEnv.vScale.y
        , _stEnv.vScale.z
    );
}

void CInGameSystem::Free() {
    Clear();
    m_pCurrOrderRecipeList->clear();
}
