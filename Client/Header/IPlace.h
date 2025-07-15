/**
* @file    IPlace.h
* @date    2025-07-02
* @author  �ǿ���
* @brief   ���� ���� ������ �ø��ų� �������� �������̽� Ŭ����
* @details �÷��̾ ������, �����̼� ���� ���� ���� ������Ʈ(���, �����ⱸ ��)�� �ø��ų� ���� �� �ִ� ����� ����.
*			������ � ������Ʈ�� �ø� �� �ִ����� ��� Ŭ�������� �����ϸ�, ������ ������� ���ε� ����.
*/
#pragma once
#include "Engine_Define.h"
#include "ICarry.h"
#include "CTransform.h"
#include "CIngredient.h"
#include "IProcess.h"

class IPlace
{
public:
	virtual		~IPlace() {}

	/**
	* @brief	�ش� ���� ���� ������ �ø��� �Լ�
	* @param	pItem - �ø� CGameObject* (�� : ���, ����)
	* @param	pPlace - ������ �Ǵ� ���� CGameObject* (�� : ������)
	* @return	true�� �ø��� ����, false�� ���� (�̹� �����밡 á�ų� ���� ������)
	*/
	virtual _bool Set_Place(CGameObject* pItem, CGameObject* pPlace)
	{
		if (nullptr == pItem || nullptr == pPlace)
			return false;

		if (m_bFull || !Get_CanPlace(pItem))	// �������� �ø� �� �ִ� ���� ������ ������ �ٸ� �� ����
			return false;

		CTransform* pItemTransform = dynamic_cast<CTransform*>(pItem->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		CTransform* pPlaceTransform = dynamic_cast<CTransform*>(pPlace->Get_Component(ID_DYNAMIC, L"Com_Transform"));

		_vec3 vPlacePos{}, vPlaceScale{}, vItemScale{};
		pPlaceTransform->Get_Info(INFO_POS, &vPlacePos);
		vPlaceScale = pPlaceTransform->Get_Scale();
		vItemScale = pItemTransform->Get_Scale();

		pItemTransform->Set_Pos(vPlacePos.x, vPlacePos.y + vPlaceScale.y * 0.5f + vItemScale.y * 0.5f, vPlacePos.z);
		dynamic_cast<CInteract*>(pItem)->Set_Ground(true);

		m_bFull = true;
		m_pPlacedItem = pItem;

		return true;
	}

	virtual _bool Set_Place(CGameObject* pItem)
	{
		if (nullptr == pItem)
			return false;

		if (m_bFull || !Get_CanPlace(pItem))	// �������� �ø� �� �ִ� ���� ������ ������ �ٸ� �� ����
			return false;

		m_bFull = true;
		m_pPlacedItem = pItem;

		return true;
	}

	/**
	* @brief	������ ���� á���� ���� ��ȯ
	* @return	true�� ������ �ö� ����, false�� ��� ����
	*/
	_bool Is_Full() const { return m_bFull; }
	
	/**
	* @brief	���� ���� �ö� ������ �������� �Լ�
	* @return	�÷��� �ִ� ������Ʈ�� ������
	*			- nullptr�̸� ������ ��� �ְų�, �������� ���(Lock) ������ ���
	* @details 
	*			- CIngredient Ÿ���� �������� ��� �ִ� ���(Get_Lock() == true) ���� �� ����
	*			- ���������� ���´ٸ�, ���� ���´� �ڵ����� �������(Set_Empty), ���� ������(m_pPlacedItem)�� nullptr�� �ʱ�ȭ ��
	*/
	virtual CGameObject* Get_PlacedItem() 
	{ 
		if (nullptr == m_pPlacedItem)
			return nullptr;

		if (dynamic_cast<CIngredient*>(m_pPlacedItem) && dynamic_cast<CIngredient*>(m_pPlacedItem)->Get_Lock())
			return nullptr;

		dynamic_cast<CInteract*>(m_pPlacedItem)->Set_Ground(false);
		CGameObject* pItem = m_pPlacedItem;

		Set_Empty();

		return pItem; 
	}

	/**
	* @brief	���� ������ �ö� �ִ� ���� Ȯ��
	* @return	�÷��� CGameObject* ������, ������ nullptr
	*/
	CGameObject* const Get_Item() { return m_pPlacedItem; }

	/**
	* @brief	������ �� ���·� �����ϴ� �Լ�.
	* @detail	���� ������ �ʱ�ȭ ��, IProcess Ÿ���̸� ���൵�� �ʱ�ȭ
	*/
	virtual void Set_Empty()
	{
		m_bFull = false;
		m_pPlacedItem = nullptr;

		if (dynamic_cast<IProcess*>(this))
			dynamic_cast<IProcess*>(this)->Set_Progress(0.f);
	}

private:
	/**
	* @brief	�ش� ������ �־��� ������ �ø� �� �ִ��� Ȯ���ϴ� �������� �Լ�
	* @param	pItem - �ø� �� �ִ��� Ȯ���� �������� ������
	* @return	true�� �ø� �� ����, false�� �Ұ���
	*/
	virtual _bool Get_CanPlace(CGameObject* pItem) = 0;

protected:
	_bool			m_bFull = false;			///< ���� ������ ��� ������ ���� (true: ���� ��, false: ��� ����)
	CGameObject*	m_pPlacedItem = nullptr;	///< ������ �ö� ������Ʈ ������ (nullptr�� �� ����)
};