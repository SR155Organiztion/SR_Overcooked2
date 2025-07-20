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
	* @brief ProtoEffect�� ����ϴ� �Լ�. ó�� 1ȸ�� ����� ��
	* @param EffectName - ����� ����Ʈ �̸� "����Ʈ"
	* - pEffect - ����� ProtoEffect 
	*/
	HRESULT		Ready_ProtoEffect(std::wstring EffectName, CEffect* pEffect);
	/**
	* @brief �̸� ����Ʈ�� �����ϴ� �Լ�
	* @param EffectName - ��ϵ� ����Ʈ �̸� "����Ʈ"
	* - Count - ����
	*/
	HRESULT		Reserve_Effect(std::wstring EffectName, size_t Count);
	/**
	* @brief ����Ʈ ��� �Լ�
	* @param EffectName - ��ϵ� ����Ʈ �̸� "����Ʈ"
	* - Owner - ���� ����Ʈ�� ����� ������Ʈ(��ġ�� ����ϱ� ����). ������ this �Ѱ��ָ� ��
	*/
	_bool		Play_Effect(std::wstring EffectName, CGameObject* Owner);

	/**
	* @brief ����Ʈ ��� �Լ� (��ġ ���� ����)
	* @param EffectName - ��ϵ� ����Ʈ �̸� "����Ʈ"
	* - vPos - ����Ʈ�� �����ų ��ġ
	*/
	_bool		Play_Effect_Pos(std::wstring EffectName, _vec3 vPos);

	/**
	* @brief ���� ���� ����ǰ� �ִ� ����Ʈ�� ���� ���� �Լ�
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