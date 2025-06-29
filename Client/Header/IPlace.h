/**
* @file    IPlace.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   �÷��̾ ������Ʈ�� �������� �� �ִ� �������̽�
* @details Ư�� Ÿ���� ������(ICarry)�� �������� �� �ִ� ������ ����.
*          ��: ������, ���� ���� ���� ��
*/
#pragma once
#include "Engine_Define.h"
#include "ICarry.h"
#include <unordered_set>

class IPlace
{
public:
	/**
	* @brief �ش� ������ �������� �������� �� �ִ��� Ȯ���ϴ� �Լ�.
	* @param pCarry - �������� ICarry ������.
	* @return true�� �������� ����, false�� �Ұ���.
	*/
	virtual _bool Get_CanPlace(ICarry* pCarry) const = 0;
	/**
	* @brief ������ �� �ִ� ���·� �����ϴ� �Լ�.
	*/
	void Set_Full() {}
	/**
    * @brief ������ �� ���·� �����ϴ� �Լ�.
    */
	void Set_Empty() {}
	virtual ~IPlace() {}

protected:
	/**
	* @brief �ش� ������ ���� �� �ִ� ������ Ÿ���� �����ϴ� �Լ�.
	* @details ��� Ŭ�������� m_setCarryTypes�� ����� ICarry Ÿ���� ���� �߰�.
	*/
	virtual void Set_CarryTypes() = 0;

protected:
	_bool m_bFull = false;	///< ���ڷ��� ���� (���� ������ ���� á���� ����) (true = ���� ��� ��, false = �������)
	unordered_set<ICarry::CARRYTYPE> m_setCarryTypes;	///< �ش� ������ ���� �� �ִ� ICarry Ÿ�Ե��� ����
};