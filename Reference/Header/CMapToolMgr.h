/**
* @file		C
.h
* @date		2025-06-30
* @author	����ȣ
* @brief	���� �Ŵ���
* @details  �� ������ ������� �Ѱ����ϴ� �Ŵ���
*/

#pragma once
#include "CBase.h"
#include "Engine_Define.h"


/**
* @struct S_BLOCK
* @brief �� ����ü
*/
struct S_BLOCK {
	string Block_Type;
	_vec3 vPos;
	string Direction;
};

/**
* @struct S_Tile
* @brief Ÿ�� ����ü
*/
struct S_TILE {
	string Tile_Type;
	_vec3 vPos;
	string Direction;
};

/**
* @struct S_ENVIRONMENT
* @brief ȯ�� ������ ����ü
*/
struct S_ENVIRONMENT {
	string Env_Type;
	_vec3 vPos;
	_vec3 Direction;
};

/**
* @struct S_CAM
* @brief ī�޶� ������ ���� ����ü
*/
struct S_CAM {
	_vec3 vEye;
	_vec3 vAt;
};
/**
* @struct S_PLAYER
* @brief ī�޶� ������ ���� ����ü
*/
struct S_PLAYER {
	_vec3 P1;
	_vec3 P2;
};

/**
* @struct S_STAGE
* @brief ���������� ��� ������ ���� ����ü
*/
struct S_STAGE {
	S_CAM Cam;
	S_PLAYER Player;
	std::vector<string> Recipe;
	std::vector<S_BLOCK> Block;
	std::vector<S_TILE> Tiles;
	std::vector<S_ENVIRONMENT> Environment;
};

///////////////////////////////////////////////////////////////////////////////
BEGIN(Engine)
class ENGINE_DLL CMapToolMgr : public CBase
{
	DECLARE_SINGLETON(CMapToolMgr)

private:
	explicit CMapToolMgr();
	virtual ~CMapToolMgr();

public:
	HRESULT Load_Json();				///json ������ �ҷ�����

public:
	S_STAGE Get_Data(string s);			///������ �� ��������

private:
	map<string, S_STAGE> m_mapJson;		///��ü ���������� �����Ͱ�

private:
	virtual void		Free();

};

END
