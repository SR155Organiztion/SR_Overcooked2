#include "../Header/json.hpp"
#include "CMapTool.h"
#include <fstream>


using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_vec3, x, y, z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_BLOCK, Block_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_TILE, Tile_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_ENVIRONMENT, Env_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_CAM, vEye, vAt)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_PLAYER, P1, P2)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_STAGE, Cam, Player, Recipe, Block, Tiles, Environment)

IMPLEMENT_SINGLETON(CMapTool)

CMapTool::CMapTool()
{

    m_mapJson.clear();
}

CMapTool::~CMapTool()
{
}


HRESULT CMapTool::Load_Json()
{
    m_mapJson.clear();

    std::ifstream file("../Bin/Data/SaveData.json");

    if (!file.is_open()) {
        MSG_BOX("���� ���� ����");
        return E_FAIL;
    }

    {
        json j;
        file >> j;

        bool firstKey = false;
        // ��ȸ�ϸ� �� ��Ҹ� map<string, S_STAGE>�� ��ȯ
        for (const auto& Stage : j)
        {
            if (!Stage.is_object()) continue;

            for (auto it = Stage.begin(); it != Stage.end(); ++it)
            {

                const std::string& stageName = it.key();
                const json& stageJson = it.value();

                // ��ȯ �õ�
                S_STAGE stage = stageJson.get<S_STAGE>();
                m_mapJson[stageName] = stage;
            }
        }

        file.close();
    }
    MSG_BOX("�ε�Ϸ�");
    return S_OK;
}
        


S_STAGE CMapTool::Get_Data(string s)
{
    auto it = m_mapJson.find(s);
    if (it != m_mapJson.end())
        return it->second;

    MSG_BOX("���� ��������");
    return S_STAGE{};
}

void CMapTool::Free()
{
    m_mapJson.clear();
}