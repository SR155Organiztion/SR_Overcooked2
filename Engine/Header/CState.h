/**
* @mainpage CState 사용법
* @section intro 소개
* - State의 구조 파악
* @section 구조
* - Object의 Update타이밍에 Update_State와 TestForExit를 순회
* - TestForExit은 탈출조건을 검사하는 함수로 매 프레임마다 호출함. 이 때 탈출 조건 성립 시, Change_State 호출을 여기서 하면 됨
* - Change_State 시, 전환하는 State의 Enter_State 호출로 진입함
* @section 주의사항
* - State 구현 시, 한 클래스 안에 여러 State를 구성하는 것을 추천
* -# Idle, Move, Attack 등 State가 여러개라 해서 각각 따로 헤더를 만들지 않는걸 권장
* - State는 필요한 함수 및 멤버 변수만 가지고 있는 묶음의 개념임
* - 이 설명을 읽고도 이해하기 어려울 시 참고
* -# State 구현 - CPlayerState.h 및 cpp
* -# FSMComponent 생성 - CRealPlayer.cpp
*/


#pragma once
#include "CGameObject.h"

class CState
{
public:
	virtual	void		Enter_State(CGameObject* Owner) = 0;
	virtual	void		Update_State(CGameObject* Owner, const _float& fTimeDelta) = 0;
	virtual	void		TestForExit_State(CGameObject* Owner) = 0;
};


