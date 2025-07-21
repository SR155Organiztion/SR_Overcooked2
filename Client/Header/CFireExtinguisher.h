/**
* @file    CFireExtinguisher.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��ȭ��(Fire Extinguisher) ������Ʈ Ŭ����
* @details �÷��̾ ��� �̵��ϰų� ����ϴ� ��ȭ�� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "IProcess.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CFireExtinguisher : public CInteract, public IProcess
{
protected:
	explicit CFireExtinguisher(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFireExtinguisher(const CGameObject& rhs);
	virtual ~CFireExtinguisher();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE	Get_InteractType() const override { return CInteract::EXTINGUISHER; }

	// IProcess��(��) ���� ��ӵ�
	_bool			Enter_Process();
	void			Update_Process(const _float& fTimeDelta);
	void			Exit_Process();

private:
	HRESULT			Add_Component();
	void			Set_GasStationList();
	void			Update_Extinguish();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

	_vec3			m_vLook{};
	list<CGameObject*>	m_listGasStation;

	const _float	m_fDistance = 3.f;
	const _float	m_fOffset = 0.2f;

public:
	static CFireExtinguisher* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};

