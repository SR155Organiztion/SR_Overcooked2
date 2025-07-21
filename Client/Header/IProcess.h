/**
 * @file    IProcess.h
 * @date    2025-07-08
 * @author  �ǿ���
 * @brief   ����(ó��) ������ �����ϴ� �������̽� Ŭ����
 * @details IProcess�� ��ᳪ ������ ���� �ð� ���� ó���Ǵ� ������ �����ϴ� �������̽� Ŭ����
 *          ���, ���� ��� ���� �۾��� ���� �������̽�
*			���൵ ������Ʈ, ����/�ߴ�/���� ���� ����� ����
 */
#pragma once
#include "Engine_Define.h"
#include "CTransform.h"

class IProcess
{
public:
	/**
	 * @brief	������ ������ �� �ִ��� �Ǵ��ϰ� ���� ó��
	 * @return	true: ���� ����, false: ���� ���� ������
	 */
	virtual	_bool Enter_Process() = 0;

	virtual _bool Enter_Process(const _vec3& vDir) {}

	/**
	 * @brief ������ �Ͻ� ����
	 */
	virtual void Pause_Process() { m_bProcess = false; }

	virtual ~IProcess() = default;

	/**
	 * @brief	���� ���� ������ Ȯ��
	 * @return	true: ���� ��, false: ���� �ƴ�
	 */
	_bool	Get_Process() const { return m_bProcess; }

	/**
	 * @brief ���� ���� ���൵ Ȯ��
	 * @return 0.0f ~ 1.0f ������ ���൵ (���� ������ 0.0f ~ 2.0f)
	 */
	_float	Get_Progress() const { return m_fProgress; }

	/**
	 * @brief �ܺο��� ���൵ ���� ���� ����
	 * @param fProgress ������ ���൵ (0.0f ~ 1.0f) (���� ������ 0.0f ~ 2.0f)
	 */
	void	Set_Progress(const _float& fProgress) { m_fProgress = fProgress; }

protected:
	/**
	 * @brief �� ������ ���� ���� ������Ʈ
	 * @param fTimeDelta ������ ��� �ð�
	 */
	virtual void Update_Process(const _float& fTimeDelta) = 0;

	/**
	 * @brief ���� ���� ó�� (�Ϸ� Ȥ�� ��� ��)
	 */
	virtual void Exit_Process() = 0;

	/**
	 * @brief ���� ���� ����
	 * @param bProcess true: ����, false: ����
	 */
	void	Set_Process(_bool bProcess) { m_bProcess = bProcess; } 

	/**
	 * @brief ���൵ ����
	 * @param fTimeDelta ������ �ð�
	 * @param fAdd ���൵ ���� ��� (�ӵ�)
	 */
	void	Add_Progress(const _float& fTimeDelta, const _float& fAdd) { m_fProgress += fTimeDelta * fAdd; }

protected:
	_bool	m_bProcess = false;	///< ���� ���� ������ ����
	_float	m_fProgress = 0.f;	///< ���� ���� ���൵ (0.0f ~ 1.0f) (���� ������ 0.0f ~2.0f)
};

/**
 * @class	IChop
 * @brief	��� ���(Chopping) �۾��� ���� ���� �������̽�
 * @details Į�� ��Ḧ ��� �۾��� ���õ� ���� ó���� ���
 */
class IChop : public IProcess
{
public:
	// IProcess��(��) ���� ��ӵ�
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;
};

/**
 * @class	ICook
 * @brief	��� ����(Cooking) �۾��� ���� ���� �������̽�
 * @details ���� �� ������ ��Ḧ ������ ������ ó��
 */
class ICook : public IProcess
{
public:
	// IProcess��(��) ���� ��ӵ�
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;

protected:
	/**
	 * @brief ���� ���� ����� ��ġ�� �������� �߽����� ����
	 * @param pTool       �������� (����, �� ��)
	 * @param pPlacedItem ���� ���� ������ (���)
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
	// IProcess��(��) ���� ��ӵ�
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;
};

class IExtinguish : public IProcess
{
public:
	// IProcess��(��) ���� ��ӵ�
	virtual _bool Enter_Process() = 0;
	virtual void Update_Process(const _float& fTimeDelta) = 0;
	virtual void Exit_Process() = 0;
};