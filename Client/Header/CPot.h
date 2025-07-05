/**
* @file    CPot.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ����(Pot) ������Ʈ Ŭ����
* @details �÷��̾ ��� �̵��ϰų�, ��Ḧ �����ϰų�, Ư�� ��Ḧ �÷��� �� �ִ� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "ICarry.h"
#include "IPlace.h"
#include "IProcess.h"
#include "IPhysics.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CPot : public CInteract, public ICarry, public IPlace, public ICook, public IPhysics
{
protected:
	explicit CPot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPot(const CGameObject& rhs);
	virtual ~CPot();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// ICarry��(��) ���� ��ӵ�
	/**
	* @brief �ش� ������Ʈ�� ���� ��� �̵� ������ �������� Ȯ���ϴ� �Լ�.
	* @return �̵� �����ϸ� true, �Ұ����ϸ� false.
	*/
	_bool			Get_CanCarry() const override;

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::POT; }

	// IProcess��(��) ���� ��ӵ�
	_bool			Enter_Process() override;
	void			Update_Process(const _float& fTimeDelta) override;
	void			Exit_Process() override;

private:
	HRESULT		Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CPot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

	// IPlace��(��) ���� ��ӵ�
	_bool Get_CanPlace(CGameObject* pItem) override;
};