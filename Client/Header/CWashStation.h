/**
* @file    CWashStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��ô �۾���(Wash Station) ������Ʈ Ŭ����
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

class CWashStation : public CInteract, public IPlace
{
protected:
	explicit CWashStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWashStation(const CGameObject& rhs);
	virtual ~CWashStation();

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
	static CWashStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

	// IPlace��(��) ���� ��ӵ�
	_bool Get_CanPlace(CGameObject* pItem) override;
};