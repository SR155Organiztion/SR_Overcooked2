/**
* @file    IChop.h
* @date    2025-06-29
* @author  권예지
* @brief   재료를 썰 수 있는 오브젝트를 위한 인터페이스
* @details 플레이어가 재료(CIngredient)를 써는 행위를 처리하는 공통 인터페이스.
*/
#pragma once
#include "Engine_Define.h"
class CIngredient;

class IChop 
{
public:
    /**
    * @brief 해당 재료를 썰 수 있는지 판별하는 함수.
    * @param pIngredient - 썰고자 하는 재료 포인터.
    * @return true면 썰기 가능, false면 불가능.
    */
	virtual _bool CanChop(CIngredient* pIngredient) const = 0;
    /**
    * @brief 해당 재료를 써는 동작을 수행하는 함수.
    * @param pIngredient - 썰고자 하는 재료 포인터.
    * @details 실제 썰기 동작을 수행하며, 상태 변경이나 진행도 증가가 포함.
    */
	virtual void Chop(CIngredient* pIngredient) = 0;
};