/**
* @file    CInteractMgr.h
* @date    2025-07-02
* @author  �ǿ���
* @brief   ��ȣ�ۿ� ������Ʈ ���� ���� �̱��� �Ŵ��� Ŭ����
* @details �÷��̾ ��ȣ�ۿ� ������ ������Ʈ(CInteract ���)�� �������� �����ϴ� �̱��� �Ŵ��� Ŭ����.
*		   �� �� �ִ� ������Ʈ, ���� �۾���� �����Ͽ� ����Ʈ�� �����ϸ�, ����Ʈ �߰�, ��ȸ, ���� ����.
*/
#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CGameObject.h"

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
		INTEND    ///< ��
	};

private:
	explicit CInteractMgr();
	virtual ~CInteractMgr();

public:
	/**
	* @brief ������ Ÿ���� ��ȣ�ۿ� ������Ʈ ����Ʈ�� ��ȯ�ϴ� �Լ�.
	* @param eType - ��ȸ�� INTERACT Ÿ��
	* @return �ش� Ÿ���� CGameObject* ����Ʈ ���� ��ȯ.
	*/
	list<CGameObject*>*	Get_List(INTERACT eType);

	/**
	* @brief Ư�� Ÿ���� ��ȣ�ۿ� ������Ʈ�� ����Ʈ�� �߰��ϴ� �Լ�.
	* @param eType - �߰��� ������Ʈ�� INTERACT Ÿ��
	* @param pInteract - �߰��� CGameObject* ������
	*/
	void				Add_List(INTERACT eType, CGameObject* pInteract);

	/**
	* @brief Ư�� Ÿ���� ��ȣ�ۿ� ������Ʈ�� ����Ʈ���� �����ϴ� �Լ�.
	* @param eType - ������ ������Ʈ�� INTERACT Ÿ��
	* @param pInteract - �߰��� CGameObject* ������
	*/
	void				Remove_List(INTERACT eType, CGameObject* pInteract);

private:
	list<CGameObject*>	m_list[INTEND];	///< Ÿ�Ժ� ��ȣ�ۿ� ������Ʈ ����Ʈ �迭

public:
	virtual void	Free(void);
};

END