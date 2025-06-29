/**
* @file		CIngredient.h
* @date		2025-06-29
* @author	�ǿ���
* @brief	��� Ŭ���� (���� ���� �� ���൵ ����)
* @details	�÷��̾ ���� ������ ��� ������Ʈ.
*			ICarry�� ���� ��� �̵� �����ϸ�, ���� ���� ��ȭ �� ���൵�� ����.
*/
#pragma once
#include "CInteract.h"
#include "ICarry.h"

class CIngredient : public CInteract, public ICarry
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
		BURNT,///< ź ����
		DONE///< ���� ���� �Ϸ� ���� (���ÿ� �ø� �� ����)
	};

protected:
	explicit CIngredient(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CIngredient(const CGameObject& rhs);
	virtual ~CIngredient();

public:
	// ICarry��(��) ���� ��ӵ�
	/**
	* @brief �ش� ������Ʈ�� ���� ��� �̵� ������ �������� Ȯ���ϴ� �Լ�.
	* @return �̵� �����ϸ� true, �Ұ����ϸ� false.
	*/
	_bool		Get_CanCarry() const override;

public:
	/**
	* @brief ����� ���� ���¸� ��ȯ�ϴ� �Լ�.
	* @return COOKSTATE ������ �� (RAW, CHOPPED, COOKED, BURNT, DONE �� �ϳ�) ����.
	*/
	virtual		COOKSTATE	Get_State() const;
	/**
	* @brief ����� ���� ���¸� �����ϴ� �Լ�.
	* @param eState - COOKSTATE ������ ��.
	*/
	virtual		void		Set_State(COOKSTATE eState);
	/**
	* @brief ����� ���� ���°� DONE ���� Ȯ���ϴ� �Լ�.
	* @return �Ϸ� ������ ��� true, �׷��� ������ false.
	*/
	virtual		_bool		Is_FinalStep() const;
	/**
	* @brief ����� ���¸� DONE���� �����ϴ� �Լ�.
	*/
	virtual		void		Set_Done();
	/**
	* @brief ���� ���� ���൵�� ��ȯ�ϴ� �Լ�.
	* @return ���൵ (0.0f ~ 1.0f ������ ��) ����.
	*/
	virtual		_float		Get_Progress() const;
	/**
	* @brief ���� ���൵�� ������Ű�� �Լ�.
	* @param fAdd ���� ���൵ ��.
	*/
	virtual		void		Add_Progress(const _float& fAdd);

protected:
	/**
	* @brief ���� ���¸� ���� �ܰ�� ��ȯ�ϴ� �Լ�.
	*/
	virtual		void		NextState();

private:
	COOKSTATE	m_eCookState;	///< ������ COOKSTATE ���� (����� ���� ����)
	_float		m_fProgress;	///< �Ǽ��� ���� (����� ���� ���൵) (0.0f ~ 1.0f ����)

protected:
	virtual		void		Free();
};