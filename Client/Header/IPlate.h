/**
* @file    IPlate.h
* @date    2025-07-08
* @author  권예지
* @brief   사용 X
* @details (아마도 IProcess 상속 받은 클래스로 만들 듯 )재료를 담을 수 있는 오브젝트를 위한 인터페이스
*           플레이어가 재료(CIngredient)를 담는 행위를 처리하는 공통 인터페이스.
*/
#pragma once
#include "Engine_Define.h"
class CIngredient;

class IPlate
{
public:
    /**
    * @brief 해당 재료를 담을 수 있는지 판별하는 함수.
    * @param pIngredient - 담으려는 재료 포인터.
    * @return 담을 수 있으면 true, 불가능하면 false.
    */
	virtual _bool CanPlate(CIngredient* pIngredient) const = 0;
    /**
    * @brief 해당 재료를 담는 동작을 수행하는 함수.
    * @param pIngredient - 담을 재료 포인터.
    * @details 실제 재료를 접시에 올리는 동작을 처리.
    */
	virtual void Plate(CIngredient* pIngredient) = 0;
};