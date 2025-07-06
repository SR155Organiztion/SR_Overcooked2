#pragma once
#include "Engine_Typedef.h"

/**
* @struct ACT_ID
* @brief 플레이어의 특수 행동에 대한 열거체
*/
enum ACT_ID {
	ACT_CHOP,/// 썰고 있을 때
	ACT_WASH,/// 설거지 할 때
	ACT_EXTINGUISH,/// 소화기 들고 불을 끌 때
	ACT_END
};

/**
* @struct PLAYER_ROT
* @brief 플레이어의 이동 방향에 대한 열거체
*/
enum PLAYER_ROT {
	PLAYER_L,
	PLAYER_R,
	PLAYER_U,
	PLAYER_D,
	PLAYER_LD,
	PLAYER_RD,
	PLAYER_LU,
	PLAYER_RU,
	PLAYERROT_END
};

/**
* @struct PLAYER_NUM
* @brief 플레이어의 주체에 대한 열거체
*/
enum PLAYER_NUM {
	PLAYER_1P,
	PLAYER_2P,
	PLAYERNUM_END
};

enum HAND_ID {
	HAND_LEFT,
	HAND_RIGHT,
	HAND_END
};

/**
* @struct CURSOR_ID
* @brief Cursor의 ID
*/
enum CURSOR_ID {
	CURSOR_INGREDIENT,	/// 재료
	CURSOR_TOOL,		///식기류
	CURSOR_STATION,		///상호작용할 Station
	CURSOR_STATION_ON_ITEM,
	CURSOR_END
};

/**
* @struct REVINFO
* @brief 공전 행렬을 위한 정보 구조체
*/
struct REVINFO {

	Engine::_vec3			m_vecRevTrans; /// 공전 피벗 위치. 직접 쓰는게 아니라 xyz따로 꺼내 쓰기 위함!
	Engine::_float			m_fRevAngleX; /// 공전 피벗 X축기준 회전각
	Engine::_float			m_fRevAngleY; /// 공전 피벗 Z축기준 회전각
	Engine::_float			m_fRevAngleZ; /// 공전 피벗 Y축기준 회전각
	
};



// KEY_ONCE(DIK_KEY, { 코드구현 });
#define KEY_ONCE(KEY, BODY) \
    if (CDInputMgr::GetInstance()->Get_DIKeyState(KEY) & 0x80) \
    { \
        if (!m_bKeyCheck[KEY]) { \
            m_bKeyCheck[KEY] = true; \
            BODY \
        } \
    } \
    else \
    { \
        m_bKeyCheck[KEY] = false; \
    }