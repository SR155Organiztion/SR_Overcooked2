/**
* @file		C
.h
* @date		2025-06-30
* @author	서원호
* @brief	맵툴 매니저
* @details  맵 데이터 입출력을 총관리하는 매니저
*/

#pragma once
#include "CBase.h"
#include "Engine_Define.h"


/**
* @struct S_BLOCK
* @brief 블럭 구조체
*/
struct S_BLOCK {
	string Block_Type;
	_vec3 vPos;
	string Direction;
	string Item;
};

/**
* @struct S_Tile
* @brief 타일 구조체
*/

struct S_TILE {
	string Tile_Type;
	_vec3 vPos;
	string Direction;
};

/**
* @struct S_BLOCK
* @brief 블럭 구조체
*/
struct S_GAMEOBJECT {
	std::vector<S_BLOCK> Block;
};

/**
* @struct S_ENVOjbect
* @brief 환경 오브젝트의 오브젝트 구조체
*/

struct S_ENVOBJECT {
	string Env_Type;
	_vec3 vPos;
	float fAngle;
	_vec3 vScale;
};

/**
* @struct S_ENVIRONMENT
* @brief 환경 오브젝트 구조체
*/

struct S_ENVIRONMENT {
	std::vector<S_TILE> Tile;
	std::vector<S_ENVOBJECT> EnvObject;
};

/**
* @struct S_CAM
* @brief 카메라를 저장할 벡터 구조체
*/
struct S_MAPSIZE {
	int iX;
	int iY;
};

/**
* @struct S_PLAYER
* @brief 카메라를 저장할 벡터 구조체
*/
struct S_PLAYER {
	_vec3 P1;
	_vec3 P2;
};

/**
* @struct S_EVENT
* @brief 이벤트 여부를 저장할 구조체
*/
struct S_EVENT {
	bool bEvent;
	float fEventTime;
};

/**
* @struct S_STAGE
* @brief 스테이지의 모든 정보를 가진 구조체
*/
struct S_STAGE {
	S_MAPSIZE MapSize;
	S_PLAYER Player;
	float Time;
	S_EVENT Event;
	std::vector<string> Recipe;
	S_GAMEOBJECT GameObject;
	S_ENVIRONMENT Environment;
};

///////////////////////////////////////////////////////////////////////////////
BEGIN(Engine)
class ENGINE_DLL CMapTool : public CBase
{
	DECLARE_SINGLETON(CMapTool)

private:
	explicit CMapTool();
	virtual ~CMapTool();

public:
	HRESULT Load_Json();				///json 데이터 불러오기
	map<string, S_STAGE>* Get_MapInfo() {
		return &m_mapJson;
	}

public:
	S_STAGE Get_Data(string s);			///데이터 맵 가져오기

private:
	map<string, S_STAGE> m_mapJson;		///전체 스테이지의 데이터값

private:
	virtual void		Free();

};

END
