/**
* @file    IPlace.h
* @date    2025-06-29
* @author  권예지
* @brief   플레이어가 오브젝트를 내려놓을 수 있는 인터페이스
* @details 특정 타입의 아이템(ICarry)을 내려놓을 수 있는 공간을 정의.
*          예: 조리대, 접시 놓는 공간 등
*/
#pragma once
#include "Engine_Define.h"
#include "ICarry.h"
#include <unordered_set>

class IPlace
{
public:
	/**
	* @brief 해당 공간에 아이템을 내려놓을 수 있는지 확인하는 함수.
	* @param pCarry - 내려놓을 ICarry 포인터.
	* @return true면 내려놓기 가능, false면 불가능.
	*/
	virtual _bool Get_CanPlace(ICarry* pCarry) const = 0;
	/**
	* @brief 공간을 차 있는 상태로 설정하는 함수.
	*/
	void Set_Full() {}
	/**
    * @brief 공간을 빈 상태로 설정하는 함수.
    */
	void Set_Empty() {}
	virtual ~IPlace() {}

protected:
	/**
	* @brief 해당 공간에 놓을 수 있는 아이템 타입을 설정하는 함수.
	* @details 상속 클래스에서 m_setCarryTypes에 허용할 ICarry 타입을 직접 추가.
	*/
	virtual void Set_CarryTypes() = 0;

protected:
	_bool m_bFull = false;	///< 불자료형 변수 (현재 공간이 가득 찼는지 여부) (true = 공간 사용 중, false = 비어있음)
	unordered_set<ICarry::CARRYTYPE> m_setCarryTypes;	///< 해당 공간에 놓을 수 있는 ICarry 타입들의 집합
};