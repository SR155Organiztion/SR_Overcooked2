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

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CChopStation : public CInteract, public IPlace, public IChop
{
protected:
	//�����
	CGameObject* m_pObject4;
	CGameObject* m_pObject5 = nullptr;
	//�����


	explicit CChopStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CChopStation(const CGameObject& rhs);
	virtual ~CChopStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::CHOPSTATION; }

	// IChop��(��) ���� ��ӵ�
	_bool			Enter_Process() override;
	void			Update_Process(const _float& fTimeDelta) override;
	void			Exit_Process() override;

	// IPlace��(��) ���� ��ӵ�
	_bool			Get_CanPlace(CGameObject* pItem) override;
	virtual _bool			On_Snap(CGameObject* _pGameObject) override;

private:
	HRESULT			Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

public:
	static CChopStation*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};