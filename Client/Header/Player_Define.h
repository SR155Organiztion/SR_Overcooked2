#pragma once
#include "Engine_Typedef.h"

/**
* @struct ACT_ID
* @brief �÷��̾��� Ư�� �ൿ�� ���� ����ü
*/
enum ACT_ID {
	ACT_CHOP,/// ��� ���� ��
	ACT_WASH,/// ������ �� ��
	ACT_EXTINGUISH,/// ��ȭ�� ��� ���� �� ��
	ACT_END
};

/**
* @struct PLAYER_ROT
* @brief �÷��̾��� �̵� ���⿡ ���� ����ü
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
* @brief �÷��̾��� ��ü�� ���� ����ü
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
* @brief Cursor�� ID
*/
enum CURSOR_ID {
	CURSOR_INGREDIENT,	/// ���
	CURSOR_TOOL,		///�ı��
	CURSOR_STATION,		///��ȣ�ۿ��� Station
	CURSOR_STATION_ON_ITEM,
	CURSOR_END
};

/**
* @struct REVINFO
* @brief ���� ����� ���� ���� ����ü
*/
struct REVINFO {

	Engine::_vec3			m_vecRevTrans; /// ���� �ǹ� ��ġ. ���� ���°� �ƴ϶� xyz���� ���� ���� ����!
	Engine::_float			m_fRevAngleX; /// ���� �ǹ� X����� ȸ����
	Engine::_float			m_fRevAngleY; /// ���� �ǹ� Z����� ȸ����
	Engine::_float			m_fRevAngleZ; /// ���� �ǹ� Y����� ȸ����
	
};



// KEY_ONCE(DIK_KEY, { �ڵ屸�� });
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