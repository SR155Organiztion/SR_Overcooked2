/**
* @file		ICarry.h
* @date		2025-06-29
* @author	권예지
* @brief	플레이어가 들고 이동하거나 내려놓을 수 있는 오브젝트용 인터페이스
* @details	재료, 도구, 접시 등 플레이어가 직접 운반 가능한 오브젝트의 공통 기능을 정의.
*/
#pragma once
#include "Engine_Define.h"

class ICarry
{
public:
	/**
	* @enum CARRYTYPE
	* @brief 운반 가능한 오브젝트 타입을 정의하는 열거형
	*/
	enum CARRYTYPE {
		SEAWEED,	///< 김
		LETTUCE,	///< 양상추
		TOMATO,		///< 토마토
		CUCUMBER,	///< 오이
		FISH,		///< 생선
		SHRIMP,		///< 김새우
		MEAT,		///< 고기
		MUSHROOM,	///< 버섯
		RICE,		///< 쌀밥
		PASTA,		///< 파스타
		FRYINGPAN,	///< 후라이팬
		POT,		///< 냄비
		PLATE,		///< 접시
		FIREEXTINGUISHER, ///< 소화기
		CTEND		///< 타입 끝
	};

public:
	/**
	* @brief 현재 오브젝트의 타입을 반환하는 함수.
	* @return CARRYTYPE 열거형 값 반환.
	*/
	virtual CARRYTYPE Get_CarryType() const { return m_eType; }
	/**
	* @brief 해당 오브젝트가 현재 들고 이동 가능한 상태인지 확인하는 함수.
	* @return 이동 가능하면 true, 불가능하면 false.
	*/
	virtual _bool Get_CanCarry() const = 0;

protected:
	/**
	* @brief 오브젝트의 운반 타입을 설정하는 함수.
	* @param eType - 설정할 CARRYTYPE 값.
	*/
	virtual void Set_CarryType(CARRYTYPE eType) { m_eType = eType; }

protected:
	CARRYTYPE m_eType;	///< 열거형 CARRYTYPE 변수 (운반 가능한 오브젝트 타입)
};