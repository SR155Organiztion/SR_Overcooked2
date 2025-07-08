#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CEffectMgr : public CBase
{
	DECLARE_SINGLETON(CEffectMgr)
private:
	explicit CEffectMgr();
	virtual ~CEffectMgr();

public:
	void	Ready_Effect(); // ���⿡ �ε��Ҷ� �̸� ����Ʈ �����س�?
	void	Add_Effect(std::string EffectName, CGameObject* Owner);

	

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

private:
	unordered_map<std::string, CGameObject*> m_mapEffect;

public:
	virtual void	Free() override;
};

END