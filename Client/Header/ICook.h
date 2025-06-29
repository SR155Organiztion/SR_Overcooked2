/**
* @file    ICook.h
* @date    2025-06-29
* @author  권예지
* @brief   재료를 가열 조리할 수 있는 오브젝트를 위한 인터페이스
* @details 플레이어가 재료(CIngredient)를 가열하는 행위를 처리하는 공통 인터페이스.
*/
#pragma once
#include "Engine_Define.h"
class CIngredient;

class ICook
{
public:
    /**
    * @brief 해당 재료를 가열 조리할 수 있는지 판별하는 함수.
    * @param pIngredient - 가열 조리하려는 재료 포인터.
    * @return true면 가열 조리 가능, false면 불가능.
    */
	virtual _bool CanCook(CIngredient* pIngredient) const = 0;
    /**
    * @brief 해당 재료를 조리하는 동작을 수행합니다.
    * @param pIngredient 조리 대상 재료 포인터
    * @details 실제 조리 동작을 처리하며, 상태 변경이나 진행도 증가 등이 포함될 수 있습니다.
    */
	virtual void Cook(CIngredient* pIngredient) = 0;
};