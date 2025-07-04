/**
* @file    CChopStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��� ��� �۾��� ������Ʈ Ŭ����
* @details �÷��̾ ��Ḧ �÷����� �� �� �ִ� �۾��� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"
#include "IProcess.h"

#include "IPhysics.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CChopStation : public CInteract, public IPlace, public IChop//, public IPhysics
{
protected:
	explicit CChopStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CChopStation(const CGameObject& rhs);
	virtual ~CChopStation();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

	// IChop��(��) ���� ��ӵ�
	_bool		Enter_Process() override;
	void		Update_Process(const _float& fTimeDelta) override;
	void		Exit_Process() override;

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CChopStation*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

	// IPlace��(��) ���� ��ӵ�
	_bool		Get_CanPlace(CGameObject* pItem) override;
};