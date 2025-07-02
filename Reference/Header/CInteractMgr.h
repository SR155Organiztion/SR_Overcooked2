/**
* @file    CInteractMgr.h
* @date    2025-07-02
* @author  권예지
* @brief   상호작용 오브젝트 통합 관리 싱글톤 매니저 클래스
* @details 플레이어가 상호작용 가능한 오브젝트(CInteract 기반)를 종류별로 관리하는 싱글톤 매니저 클래스.
*		   들 수 있는 오브젝트, 고정 작업대로 구분하여 리스트로 관리하며, 리스트 추가, 조회, 삭제 가능.
*/
#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CGameObject.h"

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
		INTEND    ///< 끝
	};

private:
	explicit CInteractMgr();
	virtual ~CInteractMgr();

public:
	/**
	* @brief 지정한 타입의 상호작용 오브젝트 리스트를 반환하는 함수.
	* @param eType - 조회할 INTERACT 타입
	* @return 해당 타입의 CGameObject* 리스트 참조 반환.
	*/
	list<CGameObject*>*	Get_List(INTERACT eType);

	/**
	* @brief 특정 타입의 상호작용 오브젝트를 리스트에 추가하는 함수.
	* @param eType - 추가할 오브젝트의 INTERACT 타입
	* @param pInteract - 추가할 CGameObject* 포인터
	*/
	void				Add_List(INTERACT eType, CGameObject* pInteract);

	/**
	* @brief 특정 타입의 상호작용 오브젝트를 리스트에서 제거하는 함수.
	* @param eType - 제거할 오브젝트의 INTERACT 타입
	* @param pInteract - 추가할 CGameObject* 포인터
	*/
	void				Remove_List(INTERACT eType, CGameObject* pInteract);

private:
	list<CGameObject*>	m_list[INTEND];	///< 타입별 상호작용 오브젝트 리스트 배열

public:
	virtual void	Free(void);
};

END