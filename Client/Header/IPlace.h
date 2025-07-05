/**
* @file    IPlace.h
* @date    2025-07-02
* @author  권예지
* @brief   플레이어가 공간 위에 오브젝트를 올리거나, 공간에 올라간 오브젝트를 가져갈 수 있는 인터페이스
* @details 공간 위에 물건을 올리거나 비우는 기능을 제공.
*          올릴 수 있는 물건의 종류나 상태는 상황에 따라 달라질 수 있음.
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
	* @brief 해당 공간 위에 물건을 올리는 함수
	* @param pItem - 올릴 CGameObject* (물건)
	* @param pPlace - 기준이 되는 공간 CGameObject* (테이블 등)
	* @return true면 올리기 성공, false면 실패
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

	/**
	* @brief 공간이 가득 찼는지 확인
	* @return true면 공간이 가득 찬 상태, false면 비어 있음
	*/
	_bool Is_Full() const { return m_bFull; }
	
	/**
	* @brief 현재 공간에 올라가 있는 물건 반환
	* @return 올려진 CGameObject* 포인터, 없으면 nullptr
	*/
	virtual CGameObject* Get_PlacedItem() 
	{ 
		if (nullptr == m_pPlacedItem)
			return nullptr;

		///
		if (dynamic_cast<CIngredient*>(m_pPlacedItem) && dynamic_cast<CIngredient*>(m_pPlacedItem)->Get_Lock())
			return nullptr;
		///

		dynamic_cast<CInteract*>(m_pPlacedItem)->Set_Ground(false);
		CGameObject* pItem = m_pPlacedItem;

		Set_Empty();

		return pItem; 
	}

private:
	/**
	* @brief 해당 공간에 주어진 물건을 올릴 수 있는지 확인하는 순수가상 함수
	* @param pItem - 올릴 CGameObject* (물건)
	* @return true면 올릴 수 있음, false면 불가능
	*/
	virtual _bool Get_CanPlace(CGameObject* pItem) = 0;

	/**
    * @brief 공간을 빈 상태로 설정하는 함수.
    */
	virtual void Set_Empty()
	{
		m_bFull = false;
		m_pPlacedItem = nullptr;

		if (dynamic_cast<IProcess*>(this))
			dynamic_cast<IProcess*>(this)->Set_Progress(0.f);
	}

protected:
	CGameObject* const Get_Item() { return m_pPlacedItem; }

protected:
	_bool			m_bFull = false;			///< 불자료형 변수 (현재 공간이 가득 찼는지 여부) (true = 공간 사용 중, false = 비어있음)
	CGameObject*	m_pPlacedItem = nullptr;	///< 현재 공간 위에 올라가 있는 물건 포인터
};