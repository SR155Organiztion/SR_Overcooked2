#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CEffect.h"


BEGIN(Engine)

class ENGINE_DLL CEffectMgr : public CBase
{
	DECLARE_SINGLETON(CEffectMgr)
private:
	explicit CEffectMgr();
	virtual ~CEffectMgr();

public:
	_bool		Play_Effect(std::wstring EffectName, CGameObject* Owner);

public:
	_int		Update_Effect(const _float dt);
	void		LateUpdate_Effect(const _float dt);

public:
	HRESULT		Ready_ProtoEffect(std::wstring EffectName, CEffect* pEffect);
	HRESULT		Reserve_Effect(std::wstring EffectName, size_t Count);
	
private:
	unordered_map<std::wstring, CEffect*> m_mapProtoEffect;
	map<std::wstring, vector<CEffect*>> m_mapEffect;

public:
	virtual void	Free() override;
};

END