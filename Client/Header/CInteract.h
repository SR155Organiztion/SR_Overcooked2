/**
* @file		CInteract.h
* @date		2025-06-29
* @author	�ǿ���
* @brief	�÷��̾�� ��ȣ�ۿ� ������ ������Ʈ Ŭ����
* @details	�÷��̾ �����Ͽ� ��ȣ�ۿ�(��: ��� �̵��ϱ�, �����ϱ� ��)�� �� �ִ� ������Ʈ�� ���� ����� ����.
*/
#pragma once
#include "CGameObject.h"
#include "IPhysics.h"
#include "CTransform.h"

class CInteract : public CGameObject, public IPhysics
{
public:
	// ��ȣ�ۿ� ������ ������Ʈ�� Ÿ�� ������
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
	 * @brief �� ������Ʈ�� Ground(�ٴ�)���� ���θ� ��ȯ�ϴ� �Լ�.
	 * @return Ground�̸� true, �ƴϸ� false
	 */
	_bool	Get_Ground() const { return m_bGround; }

	/**
	 * @brief �� ������Ʈ�� Ground(�ٴ�)���� ���θ� �����ϴ� �Լ�.
	 * @param bGround true�� �����ϸ� Ground, false�� �����ϸ� ��Ȱ��ȭ
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
	 * @brief �� ������Ʈ�� � Ÿ������ ��ȯ�ϴ� ���������Լ�
	 * @return EInteractType Ÿ��
	 */
	virtual	INTERACTTYPE	Get_InteractType() const = 0;

protected:
	_bool		m_bGround = false;
	_tchar		m_szTemp[128];	// ����� ���� �ӽ÷� ���

protected:
	virtual		void		Free();
};