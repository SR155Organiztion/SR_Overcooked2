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
	// 이펙트를 어떻게 넣고 어떻게 호출할건지?
	// 만들어둔 이펙트를 준비해놓고
	// 호출될 때, Add_Effect에 넣어줌.
	// 이때, 키값을 넣어 미리 만들어둔 이펙트를 탐색 후 복사생성하고
	// Owner(위치를 갖고오기 위한.)에서 위치만 쏙 빼놓고
	// 컨테이너에 담을 때, 위치만 넣어놓기.
	// 오브젝트 관리?
	// 그때그때 불릴때마다 만들지?
	// 오브젝트 풀링으로 Stage때 미리 만들어두고
	// 그때그때 호출될때마다 재활용해서 쓸지?
	// 씬 호출 후 부르자.

	_int		Update_Effect(const _float dt);
	void		LateUpdate_Effect(const _float dt);
	void		Render_Effect();


	HRESULT		Ready_ProtoEffect(std::wstring EffectName, CEffect* pEffect);
	HRESULT		Reserve_Effect(std::wstring EffectName, size_t Count);
	
	// 이펙트 발생 시 1회 호출, Owner는 호출할 주체. 여기서 위치값만 받아서 쓸거임
	_bool		Play_Effect(std::wstring EffectName, CGameObject* Owner);



private:
	unordered_map<std::wstring, CEffect*> m_mapProtoEffect;
	map<std::wstring, vector<CEffect*>> m_mapEffect;

public:
	virtual void	Free() override;
};

END