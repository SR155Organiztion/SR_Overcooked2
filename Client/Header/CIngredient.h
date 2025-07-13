/**
* @file		CIngredient.h
* @date		2025-06-29
* @author	권예지
* @brief	재료 클래스 (조리 상태 및 진행도 관련)
* @details	플레이어가 조작 가능한 재료 오브젝트.
*/
#pragma once
#include "CInteract.h"
class IState;

class CIngredient : public CInteract
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
		DONE,///< 최종 조리 완료 상태 (접시에 올릴 수 있음)
		BURNT,///< 탄 상태
		CS_END///< 조리상태 끝
	};

	/**
	* @enum INGREDIENT_TYPE
	* @brief 재료의 종류를 나타내는 열거형
	*/
	enum INGREDIENT_TYPE {
		SEAWEED,	///< 김
		LETTUCE,	///< 양상추
		TOMATO,		///< 토마토
		CUCUMBER,	///< 오이
		FISH,		///< 생선
		SHRIMP,		///< 새우
		RICE,		///< 쌀밥
		PASTA,		///< 파스타
		TOMATOSOUP, ///< 토마토스프
		ING_END		///< 타입 끝
	};

protected:
	explicit CIngredient(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CIngredient(const CGameObject& rhs);
	virtual ~CIngredient();

public:
	/**
	* @brief 재료의 종류를 반환하는 함수.
	* @return INGREDIENT_TYPE 열거형 값 (SEAWEED, LETTUCE, TOMATO, CUCUMBER, FISH, SHRIMP, RICE, PASTA, ING_END 중 하나) 리턴.
	*/
	virtual		INGREDIENT_TYPE		Get_IngredientType() const { return m_eIngredientType; }

	/**
	* @brief 재료의 조리 상태를 반환하는 함수.
	* @return COOKSTATE 열거형 값 (RAW, CHOPPED, COOKED, BURNT, DONE, CS_END 중 하나) 리턴.
	*/
	virtual		COOKSTATE	Get_State() const { return m_eCookState; }

	/**
	* @brief 재료의 조리 상태를 설정하는 함수.
	* @param eState - COOKSTATE 열거형 값.
	*/
	virtual		void		Set_State(COOKSTATE eState) { m_eCookState = eState; }
	 
	/**
	* @brief IState*를  변경하는 함수.
	* @param pNextState : 변경할 IState*
	*/
	virtual		void		ChangeState(IState* pNextState);

	virtual		void		Set_Lock(_bool bLocked) { m_bLocked = bLocked; }
	virtual		_bool		Get_Lock() const { return m_bLocked; }

protected:
	INGREDIENT_TYPE			m_eIngredientType;	///< 열거형 INGREDIENT_TYPE 변수 (재료의 종류)
	COOKSTATE				m_eCookState;		///< 열거형 COOKSTATE 변수 (재료의 조리 상태)
	IState*					m_pCurrentState;	///< IState* 재료 FMS
	_bool					m_bLocked;			///< 옮길 수 있거나 없는 상태 불 변수

protected:
	virtual		void		Free();
};