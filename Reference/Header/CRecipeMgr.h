/**
* @file		CRecipeMgr.h
* @date		2025-07-08
* @author	권예지
* @brief	요리 레시피 매니저 클래스 (싱글톤)
* @details	주요 기능
*			- 레시피 초기화 및 등록
*			- 주어진 태그 또는 이름으로 레시피 조회.
*			- 재료 조합 판별을 위한 데이터 제공
*/

#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRecipeMgr : public CBase
{
	DECLARE_SINGLETON(CRecipeMgr)

public:
	/**
	 * @enum RECIPETYPE
	 * @brief 레시피의 종류를 정의하는 열거형
	 */
	enum RECIPETYPE
	{
		SALAD_LETTUCE,					///< 상추 샐러드
		SALAD_LETTUCE_TOMATO,			///< 상추+토마토 샐러드
		SALAD_CUCUMBER_LETTUCE_TOMATO,	///< 상추+토마토+오이 샐러드
		SASHIMI_FISH,					///< 생선 회
		SASHIMI_SHRIMP,					///< 새우 회
		SUSHI_FISH,						///< 생선 초밥
		SUSHI_CUCUMBER,					///< 오이 초밥
		PASTA_TOMATO,					///< 토마토 파스타
		RECIPE_END						///< 끝
	};

	/**
	 * @struct RECIPE
	 * @brief 개별 레시피 정보를 담는 구조체
	 */
	typedef struct tagRecipe
	{
		RECIPETYPE		eRecipeType;	///< 레시피 종류
		int				iPrice;			///< 레피시 완성 시 획득 금액
		int				iTimeLimit;		///< 제한 시간
		set<wstring>	setIngredient;	///< 레시피 재료 목록 
	}RECIPE;

private:
	explicit CRecipeMgr();
	virtual ~CRecipeMgr();

public:
	/**
	 * @brief		레시피 태그 문자열로 레시피 정보를 조회합니다.
	 * @param[out]	tRecipe 조회된 레시피 데이터
	 * @param[in]	pRecipeTag 레시피 태그 문자열
	 * @return		성공 여부 (S_OK / E_FAIL 등)
	 */
	HRESULT			Get_Recipe(RECIPE& tRecipe, const _tchar* pRecipeTag);

	/**
	 * @brief		모든 레시피 데이터를 초기화하고 등록합니다.
	 * @return		성공 여부
	 */
	HRESULT			Ready_Recipe();

private:
	map<const _tchar*, RECIPE>	m_mapRecipe;	///< 레시피 맵 컨테이너

public:
	virtual	void	Free();
};

END