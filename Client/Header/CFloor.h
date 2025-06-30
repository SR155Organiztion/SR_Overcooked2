/**
* @file    CFloor.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ���� �� �ٴ�(Floor) ������Ʈ Ŭ����
* @details ĳ���Ͱ� �̵��ϰų� �浹�� �� �ִ� ����� ���� �ʰ� �ؽ��ĸ� ���� �ٴ� ������ �����ϴ� ������Ʈ.
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CFloor : public CGameObject
{
protected:
	explicit CFloor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFloor(const CGameObject& rhs);
	virtual ~CFloor();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CFloor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};