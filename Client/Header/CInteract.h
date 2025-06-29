/**
* @file		CInteract.h
* @date		2025-06-29
* @author	권예지
* @brief	플레이어와 상호작용 가능한 오브젝트 클래스
* @details	플레이어가 접근하여 상호작용(예: 들고 이동하기, 조리하기 등)할 수 있는 오브젝트의 공통 기능을 정의.
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