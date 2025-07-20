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
	/**
	* @brief ProtoEffect를 등록하는 함수. 처음 1회만 등록할 것
	* @param EffectName - 등록할 이펙트 이름 "이펙트"
	* - pEffect - 등록할 ProtoEffect 
	*/
	HRESULT		Ready_ProtoEffect(std::wstring EffectName, CEffect* pEffect);
	/**
	* @brief 미리 이펙트를 세팅하는 함수
	* @param EffectName - 등록된 이펙트 이름 "이펙트"
	* - Count - 갯수
	*/
	HRESULT		Reserve_Effect(std::wstring EffectName, size_t Count);
	/**
	* @brief 이펙트 재생 함수
	* @param EffectName - 등록된 이펙트 이름 "이펙트"
	* - Owner - 현재 이펙트를 재생할 오브젝트(위치값 사용하기 위함). 보통은 this 넘겨주면 됨
	*/
	_bool		Play_Effect(std::wstring EffectName, CGameObject* Owner);

	/**
	* @brief 이펙트 재생 함수 (위치 직접 지정)
	* @param EffectName - 등록된 이펙트 이름 "이펙트"
	* - vPos - 이펙트를 재생시킬 위치
	*/
	_bool		Play_Effect_Pos(std::wstring EffectName, _vec3 vPos);

	/**
	* @brief 현재 씬에 재생되고 있는 이펙트를 전부 끄는 함수
	*/
	void		AllStop_Effect();

public:
	_int		Update_Effect(const _float dt);
	void		LateUpdate_Effect(const _float dt);
	
private:
	unordered_map<std::wstring, CEffect*> m_mapProtoEffect;
	map<std::wstring, vector<CEffect*>> m_mapEffect;

public:
	virtual void	Free() override;
};

END