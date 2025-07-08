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


HRESULT CInGameSystem::Ready_CInGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev)
{
    m_pGraphicDev = _pGraphicDev;
    m_stCurrStageInfo = CMapTool::GetInstance()->Get_Data(_szCurrStage);

    // 총 주문서 설정
    for (int i = 0; i < 30; i++) {
        _int iIdx = CUtil::Make_Random<_int>(0, m_stCurrStageInfo.Recipe.size());
        
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

_int CInGameSystem::Update_InGameSystem(const _float& fTimeDelta)
{


    return 0;
}

_bool CInGameSystem::Compare_FoodRecipe(set<string> _FoodSet, set<string> _RecipeSet)
{
    return TRUE;
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

void CInGameSystem::Setting_LimitTime(CGameObject* _pGameObject)
{
    //dynamic_cast<CUi_Timer*>(_pGameObject)->Set_Timer()
}

void CInGameSystem::Setting_Score(CGameObject* _pGameObject)
{
    //dynamic_cast<CUi_Score*>(_pGameObject)->Get_Score()
}

void CInGameSystem::Take_Order()
{
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
