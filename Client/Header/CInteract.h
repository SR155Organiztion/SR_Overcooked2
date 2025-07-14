/**
* @file		CInteract.h
* @date		2025-07-08
* @author	권예지
* @brief	플레이어와 상호작용 가능한 오브젝트 추상 클래스
* @details	CInteract는 플레이어와의 상호작용이 가능한 모든 오브젝트의 공통 기능을 정의하는 베이스 클래스.
*			예: 재료(Ingredient), 조리 도구(FryingPan, Pot), 접시(Plate), 스테이션류(ChopStation, SinkStation 등)
*			주요 기능으로는 상호작용 타입 구분, 물리 옵션 설정(Ground 여부, 충돌 여부 등), 변환 정보 초기화 등이 있습니다.
*/
#pragma once
#include "CGameObject.h"
#include "IPhysics.h"
#include "CTransform.h"

class CInteract : public CGameObject, public IPhysics
{
public:
	/**
	 * @enum	INTERACTTYPE
	 * @brief	상호작용 가능한 오브젝트 종류를 정의하는 열거형
	 */
	enum INTERACTTYPE
	{
		UNKNOWN,		///< 미정의 타입
		INGREDIENT,		///< 재료
		FRYINGPAN,		///< 후라이팬
		POT,			///< 냄비
		PLATE,			///< 접시
		STATION,		///< 스테이션 (도마, 싱크, 빈 제외한 작업대)
		CHOPSTATION,	///< 도마 스테이션
		SINKSTATION,	///< 싱크 스테이션 (접시 세척)
		EMPTYSTATION,	///< 빈 스테이션
		ITEND			///< 타입 끝
	};

protected:
	explicit CInteract(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteract(const CGameObject& rhs);
	virtual ~CInteract();

public:
	/**
	 * @brief	플레이어가 상호작용할 수 있는 오브젝트에 하이라이트를 켜고 끄는 함수
	 * @param	bHighlight true로 설정하면 하이라이트 활성화, false로 설정하면 비활성화
	 */
	void	Set_Highlight(_bool bHighlight) { m_bHighlight = bHighlight; }

	/**
	 * @brief	이 오브젝트가 Ground(바닥)인지 여부를 반환하는 함수.
	 * @return	Ground이면 true, 아니면 false
	 */
	_bool	Get_Ground() const { return m_bGround; }

	/**
	 * @brief	이 오브젝트가 Ground(바닥)인지 여부를 설정하는 함수.
	 * @param	bGround true로 설정하면 Ground, false로 설정하면 비활성화
	 * @details 설정 시 중력 적용 및 충돌, 구르기, 넉백 여부도 함께 갱신됩니다.
	 */
	void	Set_Ground(_bool bGround)
	{
		m_bGround = bGround;
		m_stOpt.bApplyGravity = !bGround;
		m_stOpt.bApplyCollision = !bGround;	//임시

		CTransform* pTransform = dynamic_cast<CTransform*>(Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->Rotation(ROT_Z, -pTransform->m_vAngle[2]);

		if (FRYINGPAN == Get_InteractType() || POT == Get_InteractType() || PLATE == Get_InteractType())
			return;

		m_stOpt.bApplyRolling = !bGround;
		m_stOpt.bApplyKnockBack = !bGround;
	}

	/**
	 * @brief	오브젝트 충돌 여부 설정
	 * @param	bCollision true면 충돌 활성화, false면 비활성화
	 */
	void	Set_Collision(_bool bCollision) { m_stOpt.bApplyCollision = bCollision; }

	/**
	 * @brief	이 오브젝트가 어떤 타입인지 반환하는 순수가상함수
	 * @return	INTERACTTYPE 열거형 값
	 */
	virtual	INTERACTTYPE	Get_InteractType() const = 0;

protected:
	_bool		m_bGround;	///< Ground 여부 (중력 및 물리 적용에 영향)
	_tchar		m_szTemp[128];		///< 디버깅용 임시 문자열 버퍼
	_bool		m_bHighlight;

protected:
	virtual		void		Free();
};