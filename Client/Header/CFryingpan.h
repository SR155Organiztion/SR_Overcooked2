/**
* @file    CFryingpan.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   �Ķ�����(Frying Pan) ������Ʈ Ŭ����
* @details �÷��̾ ��� �̵��ϰų�, ��Ḧ �����ϰų�, Ư�� ��Ḧ �÷��� �� �ִ� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"
#include "IProcess.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CFryingpan : public CInteract, public IPlace, public ICook
{
protected:
	explicit CFryingpan(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFryingpan(const CGameObject& rhs);
	virtual ~CFryingpan();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

public:
	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::FRYINGPAN; }

	// IProcess��(��) ���� ��ӵ�
	_bool			Enter_Process() override;
	void			Update_Process(const _float& fTimeDelta) override;
	void			Exit_Process() override;

	// IPlace��(��) ���� ��ӵ�
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace) override;
	_bool			Get_CanPlace(CGameObject* pItem) override;
	void			Set_Empty() override;

	void			Set_GasStation(_bool bGasStation) { m_bGasStation = bGasStation; }

private:
	HRESULT			Add_Component();
	void			Draw_Progress();
	void			Draw_Warning(const _float& fTimeDelta);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

	_bool			m_bGasStation;

	CGameObject*	m_pProgressBack = nullptr;
	CGameObject*	m_pProgressFill = nullptr;
	_bool			m_bProgressVisible = false;

	CGameObject*	m_pWarning = nullptr;
	_bool			m_bWarningVisible = false;
	const _float	m_fIntervalInit = 0.5f;
	_float			m_fInterval = 0.5f;
	_float			m_fTime = 0.f;

public:
	static		CFryingpan* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};