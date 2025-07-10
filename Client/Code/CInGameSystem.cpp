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
    //if (m_pCurrOrderRecipeList == nullptr) {
    //    // 주문서 UI 게임오브젝트 가져오기
    //    CUi_Order* pOrder = dynamic_cast<CUi_Order*>(
    //        _pScene->Get_GameObject(L"UI_Layer", L"Ui_Object8")
    //        );

    //    m_pCurrOrderRecipeList = pOrder->Get_OrderData();
    //}

    m_fOrderTimeElapsed += fTimeDelta;

    if (m_fOrderTimeElapsed >= m_fOrderTImeInterval) {
        CGameObject* pGameObj = _pScene->Get_GameObject(L"UI_Layer", L"Ui_Object8");
        Take_Order(pGameObj);
        m_fOrderTimeElapsed = 0.f;
    }
    
    if (GetAsyncKeyState('U')) {
        Setting_Score(_pScene, 100);
        return 0;
    }

    if (m_qCompleteOrder.setIngredient.size() != 0) {
        _int iScore = Compare_FoodRecipe();

        if (iScore >= 0) {
            // 조리 성공
            Setting_Score(_pScene, iScore);
        }
        else {
            // 조리 실패
            Setting_Score(_pScene, -20);
        }
    }
    return 0;
}

_int CInGameSystem::Compare_FoodRecipe()
{
    for (auto iter = m_pCurrOrderRecipeList->begin()
        ; iter != m_pCurrOrderRecipeList->end();) {
        CRecipeMgr::RECIPE stCurrRecipe = (*iter).Recipe;

        if (stCurrRecipe.setIngredient.size()
            != m_qCompleteOrder.setIngredient.size()) {
            continue;
        }

        _int iCheckCnt = 0;

        for (wstring ingre : m_qCompleteOrder.setIngredient) {
            auto sIter = stCurrRecipe.setIngredient.find(ingre);

            // 재료 불일치
            if (sIter == stCurrRecipe.setIngredient.end()) {
                iter++;
                break;
            }
            iCheckCnt++ ;
        }

        if (iCheckCnt == m_qCompleteOrder.setIngredient.size()) {
            return stCurrRecipe.iPrice;
        }
    }

    return -1;
}

HRESULT CInGameSystem::Parse_GameObjectData(CLayer* _pLayer)
{
    Engine::CGameObject* pGameObject = nullptr;
    vector<S_BLOCK> vecBlock = m_stCurrStageInfo.GameObject.Block;
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
        /*else if (block.Block_Type == "InvWall") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"InvWall%d", iBlockIdx++);

            Parse_Position<CInvisibleStation>(block, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }*/
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
        }*/
        /*else if (block.Block_Type == "Trash") {
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
    }
    return S_OK;
}

void CInGameSystem::Setting_LimitTime(CGameObject* _pGameObject1, CGameObject* _pGameObject2
    , CGameObject* _pGameObject3)
{
    dynamic_cast<CUi_Timer*>(_pGameObject3)->Set_Timer(60.f);
    dynamic_cast<CUi_Timer*>(_pGameObject2)->Set_Timer(60.f);
    dynamic_cast<CUi_Timer*>(_pGameObject1)->Set_Timer(60.f);
}

void CInGameSystem::Setting_Score(CScene* _pScene, _int _iScore)
{
    CGameObject* pScoreObject1 
        = _pScene->Get_GameObject(L"UI_Layer", L"Ui_Object4");

    CGameObject* pScoreObject2
        = _pScene->Get_GameObject(L"UI_Layer", L"Ui_Object5");

    dynamic_cast<CUi_Score*>(pScoreObject1)->Set_Score(_iScore);
    dynamic_cast<CUi_Score*>(pScoreObject2)->Set_Score(_iScore);
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
        , _stTile.vPos.y
        , _stTile.vPos.z
    );

    Parse_Direction(pTransform, _stTile.Direction);
}

void CInGameSystem::Free() {

}
