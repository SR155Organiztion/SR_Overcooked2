/**
* @file    ICook.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��Ḧ ���� ������ �� �ִ� ������Ʈ�� ���� �������̽�
* @details �÷��̾ ���(CIngredient)�� �����ϴ� ������ ó���ϴ� ���� �������̽�.
*/
#pragma once
#include "Engine_Define.h"
class CIngredient;

class ICook
{
public:
    /**
    * @brief �ش� ��Ḧ ���� ������ �� �ִ��� �Ǻ��ϴ� �Լ�.
    * @param pIngredient - ���� �����Ϸ��� ��� ������.
    * @return true�� ���� ���� ����, false�� �Ұ���.
    */
	virtual _bool CanCook(CIngredient* pIngredient) const = 0;
    /**
    * @brief �ش� ��Ḧ �����ϴ� ������ �����մϴ�.
    * @param pIngredient ���� ��� ��� ������
    * @details ���� ���� ������ ó���ϸ�, ���� �����̳� ���൵ ���� ���� ���Ե� �� �ֽ��ϴ�.
    */
	virtual void Cook(CIngredient* pIngredient) = 0;
};