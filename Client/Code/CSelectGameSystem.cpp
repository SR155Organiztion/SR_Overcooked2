#include "pch.h"
#include "CSelectGameSystem.h"
#include "CScene.h"
#include "CTransform.h"

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
	return E_NOTIMPL;
}

HRESULT CSelectGameSystem::Parse_GameObjectData(CLayer* _pLayer)
{
	vector<S_BLOCK> vecBlock = m_stCurrStageInfo.GameObject.Block;
	vector<S_TILE> vecTile = m_stCurrStageInfo.Environment.Tile;



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

            //Parse_Position<CEmptyStation>(tile, &pGameObject);

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(szKey, pGameObject)))
                return E_FAIL;
        }
    }

	return E_NOTIMPL;
}
