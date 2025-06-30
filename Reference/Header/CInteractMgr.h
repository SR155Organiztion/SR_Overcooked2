/**
* @file    CInteractMgr.h
* @date    2025-06-29
* @author  권예지
* @brief   상호작용 오브젝트 관리 매니저 클래스
* @details 플레이어가 상호작용하는 오브젝트(CInteract)를 종류별로 관리하는 싱글톤 매니저 클래스.
*/
#pragma once
#include "CBase.h"
#include "Engine_Define.h"
class CInteract;

BEGIN(Engine)

class ENGINE_DLL CInteractMgr : public CBase
{
	DECLARE_SINGLETON(CInteractMgr)

public:
	/**
	* @enum INTERACT
	* @brief 상호작용 오브젝트 구분 타입
	*/
	enum INTERACT {
		CARRY,    ///< 들고 다니는 오브젝트 리스트
		STATION,  ///< 작업대 및 고정 오브젝트 리스트
		INTEND    ///< 타입 끝
	};

private:
	explicit CInteractMgr();
	virtual ~CInteractMgr();

public:
	/**
	* @brief 지정한 타입의 상호작용 오브젝트 리스트를 반환하는 함수.
	* @param eType - 조회할 INTERACT 타입
	* @return 해당 타입의 CInteract 포인터 리스트 반환.
	*/
	list<CInteract*>*	Get_List(INTERACT eType) const;
	/**
	* @brief 관리 중인 모든 상호작용 오브젝트 리스트를 초기화하는 함수.
	*/
	void				Clear_List();
	/**
	* @brief 특정 타입의 상호작용 오브젝트를 리스트에 추가하는 함수.
	* @param eType - 추가할 오브젝트의 INTERACT 타입
	* @param pInteract - 추가할 CInteract 포인터
	*/
	void				Add_List(INTERACT eType, CInteract* pInteract);

private:
	list<CInteract*>	m_list[INTEND];	///< 각 타입별 상호작용 오브젝트 리스트 배열

public:
	virtual void	Free(void);
};

END