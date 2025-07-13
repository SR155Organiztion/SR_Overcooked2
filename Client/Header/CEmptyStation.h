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
#include "IShadow.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CEmptyStation : public CInteract, public IPlace, public IShadow
{
protected:
	explicit CEmptyStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEmptyStation(const CGameObject& rhs);
	virtual ~CEmptyStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::EMPTYSTATION; }

	// IPlace��(��) ���� ��ӵ�
	_bool			Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT			Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

public:
	static CEmptyStation*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free(); 
};