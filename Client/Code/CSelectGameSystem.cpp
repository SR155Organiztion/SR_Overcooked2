#include "pch.h"
#include "CSelectGameSystem.h"
#include "CTransform.h"

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

            size_t len = wcslen(szKey) + 1;
            wchar_t* pKey = new wchar_t[len];
            wcscpy_s(pKey, len, szKey);

            Parse_Position<CHexTile>(tile, &pGameObject);

            // 타일 맵에 넣을 키값 생성
            _tchar szTileKey[128] = L"";

            swprintf_s(szTileKey, L"%.2f,%.2f,%.2f"
                , tile.vPos.x, tile.vPos.y, tile.vPos.z);

            size_t tileLen = wcslen(szTileKey) + 1;
            _tchar* tileKey = new _tchar[tileLen];
            wcscpy_s(tileKey, tileLen, szTileKey);

            m_hexTileMap.insert({ 
                tileKey
                , dynamic_cast<CHexTile*>(pGameObject)
            });

            if (nullptr == pGameObject)
                return E_FAIL;
            if (FAILED(_pLayer->Add_GameObject(pKey, pGameObject)))
                return E_FAIL;
        }
    }

    _vec3 vTest = { 1.5, -0.25, 0.f };

    Find_Neighbor(&vTest);
    Flip_Tile();

	return S_OK;
}

void CSelectGameSystem::Find_Neighbor(_vec3* _vCenterPos)
{
    for (int i = 0; i < RANGE_END; i++) {
        for (int j = 0; j < DIR_END; j++) {
            _tchar szTileKey[128] = L"";

            swprintf_s(szTileKey, L"%.2f,%.2f,%.2f"
                , _vCenterPos->x + m_dirVec[j].x * (m_fRadius * (i + 1))
                , _vCenterPos->y + m_dirVec[j].y * (m_fRadius * (i + 1))
                , _vCenterPos->z + m_dirVec[j].z * (m_fRadius * (i + 1)));

            auto iter = m_hexTileMap.find(szTileKey);

            if (iter == m_hexTileMap.end()) {
               MSG_BOX("not find");
            }
            else {
                m_willFlipHexVec[i].push_back((*iter).second);
            }
        }
    }
}

void CSelectGameSystem::Flip_Tile()
{
    for (int i = 0; i < RANGE_END; i++) {
        for (int j = 0; j < m_willFlipHexVec[i].size(); j++) {
            m_willFlipHexVec[i][j]->Flip();
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