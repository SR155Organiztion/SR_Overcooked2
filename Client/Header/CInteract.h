/**
* @file		CInteract.h
* @date		2025-06-29
* @author	권예지
* @brief	플레이어와 상호작용 가능한 오브젝트 클래스
* @details	플레이어가 접근하여 상호작용(예: 들고 이동하기, 조리하기 등)할 수 있는 오브젝트의 공통 기능을 정의.
*/
#pragma once
#include "CGameObject.h"
#include "IPhysics.h"
#include "CTransform.h"

class CInteract : public CGameObject, public IPhysics
{
public:
	// 상호작용 가능한 오브젝트의 타입 열거형
	enum INTERACTTYPE
	{
		UNKNOWN,
		INGREDIENT,
		FRYINGPAN,
		POT,
		PLATE,
		STATION,
		ITEND
	};

protected:
	explicit CInteract(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteract(const CGameObject& rhs);
	virtual ~CInteract();

public:
	/**
	 * @brief 이 오브젝트가 Ground(바닥)인지 여부를 반환하는 함수.
	 * @return Ground이면 true, 아니면 false
	 */
	_bool	Get_Ground() const { return m_bGround; }

	/**
	 * @brief 이 오브젝트가 Ground(바닥)인지 여부를 설정하는 함수.
	 * @param bGround true로 설정하면 Ground, false로 설정하면 비활성화
	 */
	void	Set_Ground(_bool bGround)
	{
		m_bGround = bGround;
		m_stOpt.bApplyGravity = !bGround;
		CTransform* pTransform = dynamic_cast<CTransform*>(Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->Rotation(ROT_Z, -pTransform->m_vAngle[2]);

		if (POT == Get_InteractType() || FRYINGPAN == Get_InteractType() || PLATE == Get_InteractType())
			return;

		m_stOpt.bApplyRolling = !bGround;
		m_stOpt.bApplyKnockBack = !bGround;
	}

	void	Set_Collision(_bool bCollision) { m_stOpt.bApplyCollision = bCollision; }

	/**
	 * @brief 이 오브젝트가 어떤 타입인지 반환하는 순수가상함수
	 * @return EInteractType 타입
	 */
	virtual	INTERACTTYPE	Get_InteractType() const = 0;

protected:
	_bool		m_bGround = false;
	_tchar		m_szTemp[128];	// 디버깅 위해 임시로 사용

protected:
	virtual		void		Free();
};