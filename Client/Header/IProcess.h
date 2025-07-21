/**
 * @file    IProcess.h
 * @date    2025-07-08
 * @author  권예지
 * @brief   가공(처리) 동작을 정의하는 인터페이스 클래스
 * @details IProcess는 재료나 음식이 일정 시간 동안 처리되는 과정을 정의하는 인터페이스 클래스
 *          썰기, 조리 등과 같은 작업의 공통 인터페이스
*			진행도 업데이트, 시작/중단/종료 등의 기능을 제공
 */
#pragma once
#include "Engine_Define.h"
#include "CTransform.h"

class IProcess
{
public:
	/**
	 * @brief	가공을 시작할 수 있는지 판단하고 시작 처리
	 * @return	true: 시작 성공, false: 시작 조건 불충족
	 */
	virtual	_bool Enter_Process() = 0;

	virtual _bool Enter_Process(const _vec3& vDir) {}

	/**
	 * @brief 가공을 일시 중지
	 */
	virtual void Pause_Process() { m_bProcess = false; }

	virtual ~IProcess() = default;

	/**
	 * @brief	현재 가공 중인지 확인
	 * @return	true: 가공 중, false: 가공 아님
	 */
	_bool	Get_Process() const { return m_bProcess; }

	/**
	 * @brief 현재 가공 진행도 확인
	 * @return 0.0f ~ 1.0f 범위의 진행도 (가열 조리는 0.0f ~ 2.0f)
	 */
	_float	Get_Progress() const { return m_fProgress; }

	/**
	 * @brief 외부에서 진행도 값을 직접 설정
	 * @param fProgress 설정할 진행도 (0.0f ~ 1.0f) (가열 조리는 0.0f ~ 2.0f)
	 */
	void	Set_Progress(const _float& fProgress) { m_fProgress = fProgress; }

protected:
	/**
	 * @brief 매 프레임 가공 상태 업데이트
	 * @param fTimeDelta 프레임 경과 시간
	 */
	virtual void Update_Process(const _float& fTimeDelta) = 0;

	/**
	 * @brief 가공 종료 처리 (완료 혹은 취소 시)
	 */
	virtual void Exit_Process() = 0;

	/**
	 * @brief 가공 상태 설정
	 * @param bProcess true: 시작, false: 중지
	 */
	void	Set_Process(_bool bProcess) { m_bProcess = bProcess; } 

	/**
	 * @brief 진행도 누적
	 * @param fTimeDelta 프레임 시간
	 * @param fAdd 진행도 증가 계수 (속도)
	 */
	void	Add_Progress(const _float& fTimeDelta, const _float& fAdd) { m_fProgress += fTimeDelta * fAdd; }

protected:
	_bool	m_bProcess = false;	///< 현재 가공 중인지 여부
	_float	m_fProgress = 0.f;	///< 현재 가공 진행도 (0.0f ~ 1.0f) (가열 조리는 0.0f ~2.0f)
};

/**
 * @class	IChop
 * @brief	재료 썰기(Chopping) 작업을 위한 가공 인터페이스
 * @details 칼로 재료를 써는 작업과 관련된 가공 처리를 담당
 */
class IChop : public IProcess
{
public:
	// IProcess을(를) 통해 상속됨
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;
};

/**
 * @class	ICook
 * @brief	재료 조리(Cooking) 작업을 위한 가공 인터페이스
 * @details 냄비나 팬 위에서 재료를 익히는 과정을 처리
 */
class ICook : public IProcess
{
public:
	// IProcess을(를) 통해 상속됨
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;

protected:
	/**
	 * @brief 조리 도중 재료의 위치를 조리도구 중심으로 고정
	 * @param pTool       조리도구 (냄비, 팬 등)
	 * @param pPlacedItem 조리 중인 아이템 (재료)
	 */
	virtual void Update_ContentPosition(CGameObject* pTool, CGameObject* pPlacedItem)
	{
		if (pPlacedItem)
		{
			_vec3 vPos{};
			dynamic_cast<CTransform*>(pTool->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vPos);
			dynamic_cast<CTransform*>(pPlacedItem->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vPos.x, vPos.y, vPos.z);
		}
	}
};

class IWash : public IProcess
{
public:
	// IProcess을(를) 통해 상속됨
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;
};

class IExtinguish : public IProcess
{
public:
	// IProcess을(를) 통해 상속됨
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;
};