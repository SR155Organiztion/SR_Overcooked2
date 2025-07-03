/**
* @file		CInteract.h
* @date		2025-06-29
* @author	�ǿ���
* @brief	�÷��̾�� ��ȣ�ۿ� ������ ������Ʈ Ŭ����
* @details	�÷��̾ �����Ͽ� ��ȣ�ۿ�(��: ��� �̵��ϱ�, �����ϱ� ��)�� �� �ִ� ������Ʈ�� ���� ����� ����.
*/
#pragma once
#include "CGameObject.h"

class CInteract : public CGameObject
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
	 * @brief �� ������Ʈ�� � Ÿ������ ��ȯ�ϴ� ���������Լ�
	 * @return EInteractType Ÿ��
	 */
	virtual	INTERACTTYPE	Get_InteractType() const = 0;

protected:
	virtual		void		Free();
};