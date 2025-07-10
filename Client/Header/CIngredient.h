/**
* @file		CIngredient.h
* @date		2025-06-29
* @author	�ǿ���
* @brief	��� Ŭ���� (���� ���� �� ���൵ ����)
* @details	�÷��̾ ���� ������ ��� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
class IState;

class CIngredient : public CInteract
{
public:
	/**
	* @enum COOKSTATE
	* @brief ����� ���� ���¸� ��Ÿ���� ������
	*/
	enum COOKSTATE { 
		RAW,///< �������� ���� ���� (�� ���)
		CHOPPED,///< �� ����
		COOKED,///< ���� ���� 
		DONE,///< ���� ���� �Ϸ� ���� (���ÿ� �ø� �� ����)
		BURNT,///< ź ����
		CS_END///< �������� ��
	};

	/**
	* @enum INGREDIENT_TYPE
	* @brief ����� ������ ��Ÿ���� ������
	*/
	enum INGREDIENT_TYPE {
		SEAWEED,	///< ��
		LETTUCE,	///< �����
		TOMATO,		///< �丶��
		CUCUMBER,	///< ����
		FISH,		///< ����
		SHRIMP,		///< ����
		RICE,		///< �ҹ�
		PASTA,		///< �Ľ�Ÿ
		TOMATOSOUP, ///< �丶�佺��
		ING_END		///< Ÿ�� ��
	};

protected:
	explicit CIngredient(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CIngredient(const CGameObject& rhs);
	virtual ~CIngredient();

public:
	/**
	* @brief ����� ������ ��ȯ�ϴ� �Լ�.
	* @return INGREDIENT_TYPE ������ �� (SEAWEED, LETTUCE, TOMATO, CUCUMBER, FISH, SHRIMP, RICE, PASTA, ING_END �� �ϳ�) ����.
	*/
	virtual		INGREDIENT_TYPE		Get_IngredientType() const { return m_eIngredientType; }

	/**
	* @brief ����� ���� ���¸� ��ȯ�ϴ� �Լ�.
	* @return COOKSTATE ������ �� (RAW, CHOPPED, COOKED, BURNT, DONE, CS_END �� �ϳ�) ����.
	*/
	virtual		COOKSTATE	Get_State() const { return m_eCookState; }

	/**
	* @brief ����� ���� ���¸� �����ϴ� �Լ�.
	* @param eState - COOKSTATE ������ ��.
	*/
	virtual		void		Set_State(COOKSTATE eState) { m_eCookState = eState; }
	 
	/**
	* @brief IState*��  �����ϴ� �Լ�.
	* @param pNextState : ������ IState*
	*/
	virtual		void		ChangeState(IState* pNextState);

	virtual		void		Set_Lock(_bool bLocked) { m_bLocked = bLocked; }
	virtual		_bool		Get_Lock() const { return m_bLocked; }

protected:
	INGREDIENT_TYPE			m_eIngredientType;	///< ������ INGREDIENT_TYPE ���� (����� ����)
	COOKSTATE				m_eCookState;		///< ������ COOKSTATE ���� (����� ���� ����)
	IState*					m_pCurrentState;	///< IState* ��� FMS
	_bool					m_bLocked;			///< �ű� �� �ְų� ���� ���� �� ����

protected:
	virtual		void		Free();
};