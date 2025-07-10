/**
* @file    IPlate.h
* @date    2025-07-08
* @author  �ǿ���
* @brief   ��� X
* @details (�Ƹ��� IProcess ��� ���� Ŭ������ ���� �� )��Ḧ ���� �� �ִ� ������Ʈ�� ���� �������̽�
*           �÷��̾ ���(CIngredient)�� ��� ������ ó���ϴ� ���� �������̽�.
*/
#pragma once
#include "Engine_Define.h"
class CIngredient;

class IPlate
{
public:
    /**
    * @brief �ش� ��Ḧ ���� �� �ִ��� �Ǻ��ϴ� �Լ�.
    * @param pIngredient - �������� ��� ������.
    * @return ���� �� ������ true, �Ұ����ϸ� false.
    */
	virtual _bool CanPlate(CIngredient* pIngredient) const = 0;
    /**
    * @brief �ش� ��Ḧ ��� ������ �����ϴ� �Լ�.
    * @param pIngredient - ���� ��� ������.
    * @details ���� ��Ḧ ���ÿ� �ø��� ������ ó��.
    */
	virtual void Plate(CIngredient* pIngredient) = 0;
};