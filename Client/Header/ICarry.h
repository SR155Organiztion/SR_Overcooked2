/**
* @file		ICarry.h
* @date		2025-07-08
* @author	�ǿ���
* @brief	��� X
* @details	(���� ũ�� ������ ����) �÷��̾ ��� �̵��ϰų� �������� �� �ִ� ������Ʈ�� �������̽�
*			���, ����, ���� �� �÷��̾ ���� ��� ������ ������Ʈ�� ���� ����� ����.
*/
#pragma once
#include "Engine_Define.h"

class ICarry
{
public:
	/**
	* @enum CARRYTYPE
	* @brief ��� ������ ������Ʈ Ÿ���� �����ϴ� ������
	*/
	enum CARRYTYPE {
		SEAWEED,	///< ��
		LETTUCE,	///< �����
		TOMATO,		///< �丶��
		CUCUMBER,	///< ����
		FISH,		///< ����
		SHRIMP,		///< �����
		MEAT,		///< ���
		MUSHROOM,	///< ����
		RICE,		///< �ҹ�
		PASTA,		///< �Ľ�Ÿ
		FRYINGPAN,	///< �Ķ�����
		POT,		///< ����
		PLATE,		///< ����
		FIREEXTINGUISHER, ///< ��ȭ��
		CTEND		///< Ÿ�� ��
	};

public:
	/**
	* @brief ���� ������Ʈ�� Ÿ���� ��ȯ�ϴ� �Լ�.
	* @return CARRYTYPE ������ �� ��ȯ.
	*/
	virtual CARRYTYPE Get_CarryType() const { return m_eType; }
	/**
	* @brief �ش� ������Ʈ�� ���� ��� �̵� ������ �������� Ȯ���ϴ� �Լ�.
	* @return �̵� �����ϸ� true, �Ұ����ϸ� false.
	*/
	virtual _bool Get_CanCarry() const = 0;

protected:
	/**
	* @brief ������Ʈ�� ��� Ÿ���� �����ϴ� �Լ�.
	* @param eType - ������ CARRYTYPE ��.
	*/
	virtual void Set_CarryType(CARRYTYPE eType) { m_eType = eType; }

protected:
	CARRYTYPE	m_eType;	///< ������ CARRYTYPE ���� (��� ������ ������Ʈ Ÿ��)
	_bool		m_bPlaced;	///< ��򰡿� �÷��� �ִ��� 
};