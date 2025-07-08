#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CEffect : public CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect();

public:
	virtual			HRESULT		Ready_Effect() = 0;
	virtual			_int		Update_Effect(const _float& fTimeDelta);
	virtual			void		LateUpdate_Effect(const _float& fTimeDelta);
	virtual			void		Render_Effect() = 0;

protected:
	virtual			void		Free() override;

};

END