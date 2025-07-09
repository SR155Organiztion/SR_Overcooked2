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



	void		Add_ProtoEffect(std::string EffectName, CEffect* pEffect);
	HRESULT		Reserve_Effect(std::string EffectName, size_t Count);
	
	// ����Ʈ �߻� �� 1ȸ ȣ��, Owner�� ȣ���� ��ü. ���⼭ ��ġ���� �޾Ƽ� ������
	void		Play_Effect(std::string EffectName, CGameObject* Owner);

private:
	
	_int		Update_Effect(const _float dt);
	void		Render_Effect();


private:
	unordered_map<std::string, CEffect*> m_mapProtoEffect;
	map<std::string, vector<CEffect*>> m_mapEffect;

public:
	virtual void	Free() override;
};

END