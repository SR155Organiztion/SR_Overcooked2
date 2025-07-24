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
#include "IProcess.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CSinkStation : public CInteract, public IPlace, public IWash
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
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;
	_bool			Get_CanPlace(CGameObject* pItem) override;
	CGameObject*	Get_PlacedItem() override;
	void			Set_Empty() override;

	// IWash��(��) ���� ��ӵ�
	_bool			Enter_Process() override;
	void			Update_Process(const _float& fTimeDelta) override;
	void			Exit_Process() override;

private:
	HRESULT			Add_Component();
	void			Draw_Progress();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

	CGameObject*	m_pProgressBack = nullptr;
	CGameObject*	m_pProgressFill = nullptr;
	_bool			m_bProgressVisible = false;

public:
	static CSinkStation*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();  
};