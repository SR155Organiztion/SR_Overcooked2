/**
* @file    CEmptyStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   �� �۾���(Empty Station) ������Ʈ Ŭ����
* @details �÷��̾ �����Ӱ� �������� �������� �� �ִ� �� ���� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"
#include "IPhysics.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CEmptyStationTemp : public CInteract, public IPlace, public IPhysics
{
protected:
	explicit CEmptyStationTemp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEmptyStationTemp(const CGameObject& rhs);
	virtual ~CEmptyStationTemp();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::INGREDIENT; }

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CEmptyStationTemp* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

	// IPlace��(��) ���� ��ӵ�
	_bool Get_CanPlace(CGameObject* pObj) override;
};