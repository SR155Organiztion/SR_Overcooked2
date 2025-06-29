/**
* @file		CIngredient.h
* @date		2025-06-29
* @author	권예지
* @brief	재료 클래스 (조리 상태 및 진행도 관련)
* @details	플레이어가 조작 가능한 재료 오브젝트.
*			ICarry를 통해 들고 이동 가능하며, 조리 상태 변화 및 진행도를 관리.
*/
#pragma once
#include "CInteract.h"
#include "ICarry.h"

class CIngredient : public CInteract, public ICarry
{
public:
	/**
	* @enum COOKSTATE
	* @brief 재료의 조리 상태를 나타내는 열거형
	*/
	enum COOKSTATE { 
		RAW,///< 가공되지 않은 상태 (생 재료)
		CHOPPED,///< 썬 상태
		COOKED,///< 익힌 상태
		BURNT,///< 탄 상태
		DONE///< 최종 조리 완료 상태 (접시에 올릴 수 있음)
	};

protected:
	explicit CIngredient(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CIngredient(const CGameObject& rhs);
	virtual ~CIngredient();

public:
	// ICarry을(를) 통해 상속됨
	/**
	* @brief 해당 오브젝트가 현재 들고 이동 가능한 상태인지 확인하는 함수.
	* @return 이동 가능하면 true, 불가능하면 false.
	*/
	_bool		Get_CanCarry() const override;

public:
	/**
	* @brief 재료의 조리 상태를 반환하는 함수.
	* @return COOKSTATE 열거형 값 (RAW, CHOPPED, COOKED, BURNT, DONE 중 하나) 리턴.
	*/
	virtual		COOKSTATE	Get_State() const;
	/**
	* @brief 재료의 조리 상태를 설정하는 함수.
	* @param eState - COOKSTATE 열거형 값.
	*/
	virtual		void		Set_State(COOKSTATE eState);
	/**
	* @brief 재료의 조리 상태가 DONE 인지 확인하는 함수.
	* @return 완료 상태일 경우 true, 그렇지 않으면 false.
	*/
	virtual		_bool		Is_FinalStep() const;
	/**
	* @brief 재료의 상태를 DONE으로 설정하는 함수.
	*/
	virtual		void		Set_Done();
	/**
	* @brief 현재 조리 진행도를 반환하는 함수.
	* @return 진행도 (0.0f ~ 1.0f 범위의 값) 리턴.
	*/
	virtual		_float		Get_Progress() const;
	/**
	* @brief 조리 진행도를 증가시키는 함수.
	* @param fAdd 더할 진행도 값.
	*/
	virtual		void		Add_Progress(const _float& fAdd);

protected:
	/**
	* @brief 조리 상태를 다음 단계로 전환하는 함수.
	*/
	virtual		void		NextState();

private:
	COOKSTATE	m_eCookState;	///< 열거형 COOKSTATE 변수 (재료의 조리 상태)
	_float		m_fProgress;	///< 실수형 변수 (재료의 조리 진행도) (0.0f ~ 1.0f 범위)

protected:
	virtual		void		Free();
};