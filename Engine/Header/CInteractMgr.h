/**
* @file    CInteractMgr.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��ȣ�ۿ� ������Ʈ ���� �Ŵ��� Ŭ����
* @details �÷��̾ ��ȣ�ۿ��ϴ� ������Ʈ(CInteract)�� �������� �����ϴ� �̱��� �Ŵ��� Ŭ����.
*/
#pragma once
#include "CBase.h"
#include "Engine_Define.h"
class CInteract;

BEGIN(Engine)

class ENGINE_DLL CInteractMgr : public CBase
{
	DECLARE_SINGLETON(CInteractMgr)

public:
	/**
	* @enum INTERACT
	* @brief ��ȣ�ۿ� ������Ʈ ���� Ÿ��
	*/
	enum INTERACT {
		CARRY,    ///< ��� �ٴϴ� ������Ʈ ����Ʈ
		STATION,  ///< �۾��� �� ���� ������Ʈ ����Ʈ
		INTEND    ///< Ÿ�� ��
	};

private:
	explicit CInteractMgr();
	virtual ~CInteractMgr();

public:
	/**
	* @brief ������ Ÿ���� ��ȣ�ۿ� ������Ʈ ����Ʈ�� ��ȯ�ϴ� �Լ�.
	* @param eType - ��ȸ�� INTERACT Ÿ��
	* @return �ش� Ÿ���� CInteract ������ ����Ʈ ��ȯ.
	*/
	list<CInteract*>*	Get_List(INTERACT eType) const;
	/**
	* @brief ���� ���� ��� ��ȣ�ۿ� ������Ʈ ����Ʈ�� �ʱ�ȭ�ϴ� �Լ�.
	*/
	void				Clear_List();
	/**
	* @brief Ư�� Ÿ���� ��ȣ�ۿ� ������Ʈ�� ����Ʈ�� �߰��ϴ� �Լ�.
	* @param eType - �߰��� ������Ʈ�� INTERACT Ÿ��
	* @param pInteract - �߰��� CInteract ������
	*/
	void				Add_List(INTERACT eType, CInteract* pInteract);

private:
	list<CInteract*>	m_list[INTEND];	///< �� Ÿ�Ժ� ��ȣ�ۿ� ������Ʈ ����Ʈ �迭

public:
	virtual void	Free(void);
};

END