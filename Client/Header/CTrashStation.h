/**
* @file    CTrashStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��������(Trash Station) ������Ʈ Ŭ����
* @details �÷��̾ �������� ���� �� �ִ� ������ ����.
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

class CTrashStation : public CInteract, public IPlace
{
protected:
	explicit CTrashStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrashStation(const CGameObject& rhs);
	virtual ~CTrashStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CTrashStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

	// IPlace��(��) ���� ��ӵ�
	_bool Get_CanPlace(CGameObject* pItem) override;
};