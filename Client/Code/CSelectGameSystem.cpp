#include "pch.h"
#include "CSelectGameSystem.h"
#include "CTransform.h"
#include "CHexTile.h"

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
	vector<S_TILE> vecTile = m_stCurrStageInfo.Environment.Tile;

    Parse_TileObjectData(_pLayer, &vecTile);

	return S_OK;
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

            Parse_Position<CHexTile>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }
    }

	return S_OK;
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