/**
* @file		CCar.h
* @date		2025-07-19
* @author	�ǿ���
* @brief	�ΰ��� ���� ������Ʈ Ŭ����
* @details	Stage 1, 3
*			������ : 4
*			���� �ؽ��� (0 ~ 3)
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTileTex;
	class CTransform;
	class CTexture;
}

class CCar : public Engine::CGameObject
{
private:
	explicit CCar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCar(const CGameObject& rhs);
	virtual ~CCar();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	void		Set_Scale(const _float& fX, const _float& fY, const _float& fZ);

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTileTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_int			m_iFrame;

public:
	static		CCar*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};