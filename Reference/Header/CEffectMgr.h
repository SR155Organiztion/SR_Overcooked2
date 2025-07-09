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
	// ����Ʈ�� ��� �ְ� ��� ȣ���Ұ���?
	// ������ ����Ʈ�� �غ��س���
	// ȣ��� ��, Add_Effect�� �־���.
	// �̶�, Ű���� �־� �̸� ������ ����Ʈ�� Ž�� �� ��������ϰ�
	// Owner(��ġ�� ������� ����.)���� ��ġ�� �� ������
	// �����̳ʿ� ���� ��, ��ġ�� �־����.
	// ������Ʈ ����?
	// �׶��׶� �Ҹ������� ������?
	// ������Ʈ Ǯ������ Stage�� �̸� �����ΰ�
	// �׶��׶� ȣ��ɶ����� ��Ȱ���ؼ� ����?
	// �� ȣ�� �� �θ���.

	_int		Update_Effect(const _float dt);
	void		LateUpdate_Effect(const _float dt);
	void		Render_Effect();


	HRESULT		Ready_ProtoEffect(std::wstring EffectName, CEffect* pEffect);
	HRESULT		Reserve_Effect(std::wstring EffectName, size_t Count);
	
	// ����Ʈ �߻� �� 1ȸ ȣ��, Owner�� ȣ���� ��ü. ���⼭ ��ġ���� �޾Ƽ� ������
	_bool		Play_Effect(std::wstring EffectName, CGameObject* Owner);



private:
	unordered_map<std::wstring, CEffect*> m_mapProtoEffect;
	map<std::wstring, vector<CEffect*>> m_mapEffect;

public:
	virtual void	Free() override;
};

END