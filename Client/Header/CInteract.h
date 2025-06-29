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
protected:
	explicit CInteract(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteract(const CGameObject& rhs);
	virtual ~CInteract();

protected:
	virtual		void		Free();
};