/**
* @file    IChop.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��Ḧ �� �� �ִ� ������Ʈ�� ���� �������̽�
* @details �÷��̾ ���(CIngredient)�� ��� ������ ó���ϴ� ���� �������̽�.
*/
#pragma once
#include "Engine_Define.h"
class CIngredient;

class IChop 
{
public:
    /**
    * @brief �ش� ��Ḧ �� �� �ִ��� �Ǻ��ϴ� �Լ�.
    * @param pIngredient - ����� �ϴ� ��� ������.
    * @return true�� ��� ����, false�� �Ұ���.
    */
	virtual _bool CanChop(CIngredient* pIngredient) const = 0;
    /**
    * @brief �ش� ��Ḧ ��� ������ �����ϴ� �Լ�.
    * @param pIngredient - ����� �ϴ� ��� ������.
    * @details ���� ��� ������ �����ϸ�, ���� �����̳� ���൵ ������ ����.
    */
	virtual void Chop(CIngredient* pIngredient) = 0;
};