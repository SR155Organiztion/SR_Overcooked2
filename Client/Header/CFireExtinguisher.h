/**
* @file    CFireExtinguisher.h
* @date    2025-06-29
* @author  권예지
* @brief   소화기(Fire Extinguisher) 오브젝트 클래스
* @details 플레이어가 들고 이동하거나 사용하는 소화기 오브젝트.
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

	void			Set_Look(const _vec3& vLook) { m_vLook = vLook; }

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::EXTINGUISHER; }

	// IProcess을(를) 통해 상속됨
	_bool			Enter_Process();
	_bool			Enter_Process(const _vec3& vDir) override;
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

	const _float	m_fInterval = 0.05f;
	_float			m_fTime = 0.f;

	static const _vec3 vDiagonal[12];

public:
	static CFireExtinguisher* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};

