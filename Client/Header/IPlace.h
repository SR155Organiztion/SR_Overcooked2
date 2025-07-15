/**
* @file    IPlace.h
* @date    2025-07-02
* @author  권예지
* @brief   공간 위에 물건을 올리거나 가져오는 인터페이스 클래스
* @details 플레이어가 조리대, 스테이션 등의 공간 위에 오브젝트(재료, 조리기구 등)를 올리거나 꺼낼 수 있는 기능을 제공.
*			실제로 어떤 오브젝트를 올릴 수 있는지는 상속 클래스에서 정의하며, 공간이 비었는지 여부도 관리.
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
	* @brief	해당 공간 위에 물건을 올리는 함수
	* @param	pItem - 올릴 CGameObject* (예 : 재료, 도구)
	* @param	pPlace - 기준이 되는 공간 CGameObject* (예 : 조리대)
	* @return	true면 올리기 성공, false면 실패 (이미 조리대가 찼거나 조건 미충족)
	*/
	virtual _bool Set_Place(CGameObject* pItem, CGameObject* pPlace)
	{
		if (nullptr == pItem || nullptr == pPlace)
			return false;

		if (m_bFull || !Get_CanPlace(pItem))	// 공간마다 올릴 수 있는 물건 종류나 조건이 다를 수 있음
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

		if (m_bFull || !Get_CanPlace(pItem))	// 공간마다 올릴 수 있는 물건 종류나 조건이 다를 수 있음
			return false;

		m_bFull = true;
		m_pPlacedItem = pItem;

		return true;
	}

	/**
	* @brief	공간이 가득 찼는지 여부 반환
	* @return	true면 물건이 올라가 있음, false면 비어 있음
	*/
	_bool Is_Full() const { return m_bFull; }
	
	/**
	* @brief	공간 위에 올라간 물건을 가져오는 함수
	* @return	올려져 있던 오브젝트의 포인터
	*			- nullptr이면 공간이 비어 있거나, 아이템이 잠금(Lock) 상태인 경우
	* @details 
	*			- CIngredient 타입의 아이템은 잠겨 있는 경우(Get_Lock() == true) 꺼낼 수 없음
	*			- 성공적으로 꺼냈다면, 공간 상태는 자동으로 비워지고(Set_Empty), 내부 포인터(m_pPlacedItem)도 nullptr로 초기화 됨
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
	* @brief	현재 공간에 올라가 있는 물건 확인
	* @return	올려진 CGameObject* 포인터, 없으면 nullptr
	*/
	CGameObject* const Get_Item() { return m_pPlacedItem; }

	/**
	* @brief	공간을 빈 상태로 설정하는 함수.
	* @detail	내부 포인터 초기화 및, IProcess 타입이면 진행도도 초기화
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
	* @brief	해당 공간에 주어진 물건을 올릴 수 있는지 확인하는 순수가상 함수
	* @param	pItem - 올릴 수 있는지 확인할 오브젝터 포인터
	* @return	true면 올릴 수 있음, false면 불가능
	*/
	virtual _bool Get_CanPlace(CGameObject* pItem) = 0;

protected:
	_bool			m_bFull = false;			///< 현재 공간이 사용 중인지 여부 (true: 가득 참, false: 비어 있음)
	CGameObject*	m_pPlacedItem = nullptr;	///< 공간에 올라간 오브젝트 포인터 (nullptr일 수 있음)
};