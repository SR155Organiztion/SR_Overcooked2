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
* @struct REVINFO
* @brief ���� ����� ���� ���� ����ü
*/
struct REVINFO {

	Engine::_vec3			m_vecRevTrans; /// ���� �ǹ� ��ġ. ���� ���°� �ƴ϶� xyz���� ���� ���� ����!
	Engine::_float			m_fRevAngleX; /// ���� �ǹ� X����� ȸ����
	Engine::_float			m_fRevAngleY; /// ���� �ǹ� Z����� ȸ����
	Engine::_float			m_fRevAngleZ; /// ���� �ǹ� Y����� ȸ����
	
};