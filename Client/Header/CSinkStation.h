/**
* @file    CSinkStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��ũ�� ������Ʈ Ŭ����
* @details �÷��̾ ��Ḧ ��ô�ϰų� �������� �������� �� �ִ� ���� ����.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CSinkStation : public CInteract, public IPlace
{
protected:
	explicit CSinkStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSinkStation(const CGameObject& rhs);
	virtual ~CSinkStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::SINKSTATION; }

	// IPlace��(��) ���� ��ӵ�
	_bool Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CSinkStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free(); 
};