#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CEffect : public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect();
	void	Render_GameObject() {}

public:
	virtual			HRESULT		Ready_Effect() { return S_OK; } // �� �������̵� �� ��
	virtual			_int		Update_Effect(const _float& fTimeDelta);
	virtual			void		LateUpdate_Effect(const _float& fTimeDelta);
	virtual			void		Render_Effect() {} // �� �������̵� �� ��

public:
	void		Set_EffectActive(_bool Active) { m_bActive = Active; }
	_bool		Get_EffectActive() { return m_bActive; }

protected:
	_bool		m_bActive;
	_matrix		m_matTrans;

public:
	virtual CEffect* Clone() { return nullptr; } // �� �������̵� �� �� 

protected:
	virtual			void		Free() override;

};

END