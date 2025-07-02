#pragma once

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