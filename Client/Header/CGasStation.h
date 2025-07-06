/**
* @file    CGasStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ���� �����̼�(Gas Station) ������Ʈ Ŭ����
* @details �÷��̾ ������ ä��ų� ���� �������� �������� �� �ִ� ����.
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

class CGasStation : public CInteract, public IPlace
{
protected:
	explicit CGasStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGasStation(const CGameObject& rhs);
	virtual ~CGasStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE				Get_InteractType() const override { return CInteract::STATION; }

	// IPlace��(��) ���� ��ӵ�
	_bool						Set_Place(CGameObject* pItem, CGameObject* pPlace) override;
	CGameObject*				Get_PlacedItem() override;

	// IPlace��(��) ���� ��ӵ�
	_bool						Get_CanPlace(CGameObject* pItem) override;

private:
	HRESULT			Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CGasStation*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free(); 
};