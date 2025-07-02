#pragma once

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