#include "pch.h"
#include "CSelectGameSystem.h"
#include "CTransform.h"
#include <CFlag.h>

IMPLEMENT_SINGLETON(CSelectGameSystem)

CSelectGameSystem::CSelectGameSystem()
{
}

CSelectGameSystem::~CSelectGameSystem()
{
}

HRESULT CSelectGameSystem::Ready_CSelectGameSystem(string _szCurrStage, LPDIRECT3DDEVICE9 _pGraphicDev, CScene* _pScene)
{
	m_pGraphicDev = _pGraphicDev;
	m_stCurrStageInfo = CMapTool::GetInstance()->Get_Data(_szCurrStage);

	return S_OK;
}

HRESULT CSelectGameSystem::Parse_GameObjectData(CLayer* _pLayer)
{
	vector<S_BLOCK> vecBlock = m_stCurrStageInfo.GameObject.Block;
    
	return S_OK;
}

HRESULT CSelectGameSystem::Parse_EnviromentData(CLayer* _pLayer)
{
    vector<S_TILE> vecTile = m_stCurrStageInfo.Environment.Tile;
    vector<S_ENVOBJECT> vecEnv = m_stCurrStageInfo.Environment.EnvObject;

    if (FAILED(Parse_TileObjectData(_pLayer, &vecTile))) {
        return E_FAIL;
    }
    if (FAILED(Parse_FlagData(_pLayer, &vecEnv))) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CSelectGameSystem::Parse_EnviromentObjectData(CLayer* _pLayer)
{
    Engine::CGameObject* pGameObject = nullptr;
    CTransform* pTransform = nullptr;
    int iEnvIdx = 0;

    vector<S_ENVOBJECT> vecEnv = m_stCurrStageInfo.Environment.EnvObject;

    for (S_ENVOBJECT env : vecEnv) {
        if (env.Env_Type == "Flag") {
            _tchar szKey[128] = L"";

            wsprintf(szKey, L"SelectEnv%d", iEnvIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CFlag>(env, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;

            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
    }

    return E_NOTIMPL;
}

HRESULT CSelectGameSystem::Parse_TileObjectData(CLayer* _pLayer, vector<S_TILE>* _pVecTile)
{
	Engine::CGameObject* pGameObject = nullptr;
	CTransform* pTransform = nullptr;
	int iTileIdx = 0;

    for (S_TILE tile : *_pVecTile) {
        if (tile.Tile_Type == "TileHex") {
            TCHAR szKey[128] = L"";

            wsprintf(szKey, L"TileHex%d", iTileIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CHexTile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;

            int iQ = static_cast<int>(round(tile.vPos.x / 1.f));
            int iR = static_cast<int>(round(tile.vPos.z / (1.f * 0.75f)));

            pair<int, int> tileKey = make_pair(iQ, iR);

            m_hexTileMap.insert({ tileKey, dynamic_cast<CHexTile*>(pGameObject) });

            m_hexTileVec.push_back(dynamic_cast<CHexTile*>(pGameObject));

            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
    }

	return S_OK;
}

HRESULT CSelectGameSystem::Parse_FlagData(CLayer* _pLayer, vector<S_ENVOBJECT>* _pVecTile)
{
    Engine::CGameObject* pGameObject = nullptr;
    CTransform* pTransform = nullptr;
    int iEnvIdx = 0;

    for (S_ENVOBJECT env : *_pVecTile) {
        if (env.Env_Type == "Flag") {
            _tchar szKey[128] = L"";

            wsprintf(szKey, L"Flag%d", iEnvIdx++);

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CFlag>(env, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;

            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
    }

    return S_OK;
}

void CSelectGameSystem::Find_By_Euclidean(_vec3* _vCenterPos)
{
    _float fRadius = 5.f;

    float fTotalRadius = fRadius * fRadius;

    /*for (auto& tilePair : m_hexTileMap)
    {
        CHexTile* pTile = tilePair.second;
        _vec3 vTilePos;

        CTransform* pTileTransform =
            dynamic_cast<CTransform*>(pTile->Get_Component(ID_DYNAMIC, L"Com_Transform"));

        pTileTransform->Get_Info(INFO_POS, &vTilePos);

        float fX = vTilePos.x - _vCenterPos->x;
        float fZ = vTilePos.z - _vCenterPos->z;

        float fDistance = fX * fX + fZ * fZ;

        if (fDistance <= fTotalRadius)
        {
            pTile->Flip();
        }
    }*/

    for (auto& tile : m_hexTileVec) {
        _vec3 vTilePos;

        CTransform* pTileTransform =
            dynamic_cast<CTransform*>(tile->Get_Component(ID_DYNAMIC, L"Com_Transform"));

        pTileTransform->Get_Info(INFO_POS, &vTilePos);

        float fX = vTilePos.x - _vCenterPos->x;
        float fZ = vTilePos.z - _vCenterPos->z;

        float fDistance = fX * fX + fZ * fZ;

        if (fDistance <= fTotalRadius)
        {
            tile->Flip();
        }
    }
}



template<typename T>
void CSelectGameSystem::Parse_Position(
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
}

template<typename T>
void CSelectGameSystem::Parse_Position(
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
}

template<typename T>
void CSelectGameSystem::Parse_Position(
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
        , _stEnv.vPos.y
        , _stEnv.vPos.z
    );
}
