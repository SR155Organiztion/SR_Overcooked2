/**
* @file		CIngredient.h
* @date		2025-06-29
* @author	�ǿ���
* @brief	��� Ŭ���� (���� ���� �� ���൵ ����)
* @details	�÷��̾ ���� ������ ��� ������Ʈ.
*			ICarry�� ���� ��� �̵� �����ϸ�, IState�� ���� ���� ���� ��ȭ�� �����ϰ�, ���൵�� ����.
*/
#pragma once
#include "CInteract.h"
#include "IPhysics.h"
class IState;

class CIngredient : public CInteract, public IPhysics
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
	virtual		INGREDIENT_TYPE		Get_Type() const { return m_eIngredientType; }

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
	* @brief ���� ���� ���൵�� ��ȯ�ϴ� �Լ�.
	* @return ���൵ (0.0f ~ 1.0f ������ ��) ����.
	*/
	virtual		_float		Get_Progress() const { return m_fProgress; }

	/**
	* @brief ���� ���൵�� �����ϴ� �Լ�.
	* @param fProgress :������ ���൵ ��. (0.f ~ 1.f)
	*/
	virtual		void		Set_Progress(const _float& fProgress);

	/**
	* @brief ���� ���൵�� ������Ű�� �Լ�.
	* @param fAdd : ���� ���൵ ��. (����Ʈ : 0.005f, ���� : 0.f ~ 1.f)
	*/
	virtual		void		Add_Progress(const _float& fTimeDelta, const _float& fAdd = 0.1f);
	 
	/**
	* @brief IState*��  �����ϴ� �Լ�.
	* @param pNextState : ������ IState*
	*/
	virtual		void		ChangeState(IState* pNextState);

	virtual		void		Set_Lock(_bool bLocked) { m_bLocked = bLocked; }
	virtual		_bool		Get_Lock() const { return m_bLocked; }

private:
	virtual		bool		Check_Progress();

protected:
	INGREDIENT_TYPE			m_eIngredientType;	///< ������ INGREDIENT_TYPE ���� (����� ����)
	COOKSTATE				m_eCookState;	///< ������ COOKSTATE ���� (����� ���� ����)
	IState*					m_pCurrentState;	///< IState* ��� FMS
	_float					m_fProgress;	///< �Ǽ��� ���� (����� ���� ���൵) (0.0f ~ 1.0f ����) 

	_bool					m_bLocked;

protected:
	virtual		void		Free();
};