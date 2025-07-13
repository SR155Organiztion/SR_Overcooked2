/**
* @file		CInteract.h
* @date		2025-07-08
* @author	�ǿ���
* @brief	�÷��̾�� ��ȣ�ۿ� ������ ������Ʈ �߻� Ŭ����
* @details	CInteract�� �÷��̾���� ��ȣ�ۿ��� ������ ��� ������Ʈ�� ���� ����� �����ϴ� ���̽� Ŭ����.
*			��: ���(Ingredient), ���� ����(FryingPan, Pot), ����(Plate), �����̼Ƿ�(ChopStation, SinkStation ��)
*			�ֿ� ������δ� ��ȣ�ۿ� Ÿ�� ����, ���� �ɼ� ����(Ground ����, �浹 ���� ��), ��ȯ ���� �ʱ�ȭ ���� �ֽ��ϴ�.
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
	 * @brief	��ȣ�ۿ� ������ ������Ʈ ������ �����ϴ� ������
	 */
	enum INTERACTTYPE
	{
		UNKNOWN,		///< ������ Ÿ��
		INGREDIENT,		///< ���
		FRYINGPAN,		///< �Ķ�����
		POT,			///< ����
		PLATE,			///< ����
		STATION,		///< �����̼� (����, ��ũ, �� ������ �۾���)
		CHOPSTATION,	///< ���� �����̼�
		SINKSTATION,	///< ��ũ �����̼� (���� ��ô)
		EMPTYSTATION,	///< �� �����̼�
		ITEND			///< Ÿ�� ��
	};

protected:
	explicit CInteract(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteract(const CGameObject& rhs);
	virtual ~CInteract();

public:
	/**
	 * @brief	�÷��̾ ��ȣ�ۿ��� �� �ִ� ������Ʈ�� ���̶���Ʈ�� �Ѱ� ���� �Լ�
	 * @param	bHighlight true�� �����ϸ� ���̶���Ʈ Ȱ��ȭ, false�� �����ϸ� ��Ȱ��ȭ
	 */
	void	Set_Highlight(_bool bHighlight) { m_bHighlight = bHighlight; }

	/**
	 * @brief	�� ������Ʈ�� Ground(�ٴ�)���� ���θ� ��ȯ�ϴ� �Լ�.
	 * @return	Ground�̸� true, �ƴϸ� false
	 */
	_bool	Get_Ground() const { return m_bGround; }

	/**
	 * @brief	�� ������Ʈ�� Ground(�ٴ�)���� ���θ� �����ϴ� �Լ�.
	 * @param	bGround true�� �����ϸ� Ground, false�� �����ϸ� ��Ȱ��ȭ
	 * @details ���� �� �߷� ���� �� �浹, ������, �˹� ���ε� �Բ� ���ŵ˴ϴ�.
	 */
	void	Set_Ground(_bool bGround)
	{
		m_bGround = bGround;
		m_stOpt.bApplyGravity = !bGround;
		m_stOpt.bApplyCollision = !bGround;	//�ӽ�

		CTransform* pTransform = dynamic_cast<CTransform*>(Get_Component(ID_DYNAMIC, L"Com_Transform"));
		pTransform->Rotation(ROT_Z, -pTransform->m_vAngle[2]);

		if (FRYINGPAN == Get_InteractType() || POT == Get_InteractType() || PLATE == Get_InteractType())
			return;

		m_stOpt.bApplyRolling = !bGround;
		m_stOpt.bApplyKnockBack = !bGround;
	}

	/**
	 * @brief	������Ʈ �浹 ���� ����
	 * @param	bCollision true�� �浹 Ȱ��ȭ, false�� ��Ȱ��ȭ
	 */
	void	Set_Collision(_bool bCollision) { m_stOpt.bApplyCollision = bCollision; }

	/**
	 * @brief	�� ������Ʈ�� � Ÿ������ ��ȯ�ϴ� ���������Լ�
	 * @return	INTERACTTYPE ������ ��
	 */
	virtual	INTERACTTYPE	Get_InteractType() const = 0;

protected:
	_bool		m_bGround;	///< Ground ���� (�߷� �� ���� ���뿡 ����)
	_tchar		m_szTemp[128];		///< ������ �ӽ� ���ڿ� ����
	_bool		m_bHighlight;

protected:
	virtual		void		Free();
};