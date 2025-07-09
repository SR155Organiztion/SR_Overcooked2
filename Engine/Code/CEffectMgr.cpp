#include "CEffectMgr.h"

IMPLEMENT_SINGLETON(CEffectMgr)

CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
}

void CEffectMgr::Add_ProtoEffect(std::string EffectName, CEffect* pEffect)
{
	auto iter = m_mapProtoEffect.find(EffectName);
	if (iter != m_mapProtoEffect.end()) {
		iter->second = nullptr;  // ��ĥ ��, ���� Proto �ּ� ����
	}
	m_mapProtoEffect[EffectName] = pEffect;
}

HRESULT CEffectMgr::Reserve_Effect(std::string EffectName, size_t Count)
{
	auto iter = m_mapProtoEffect.find(EffectName);
	if (iter == m_mapProtoEffect.end()) {
		MSG_BOX("EffectMgr Reserve Failed");
		return E_FAIL;
	}
	for (size_t i = 0; i < Count; ++i) {
		m_mapEffect[EffectName].push_back(iter->second->Clone());
	}
}

void CEffectMgr::Play_Effect(std::string EffectName, CGameObject* Owner)
{
}

_int CEffectMgr::Update_Effect(const _float dt)
{
	for (auto& mapKey : m_mapEffect) { // ���� Ű�� ��ȸ
		for (auto& pEffect : mapKey.second) { // ���� ��ȸ
			if (pEffect->Get_EffectActive()) {
				_int Result = pEffect->Update_Effect(dt);
				

			}
		}
	}


	return S_OK;
}

void CEffectMgr::Render_Effect()
{
}

void CEffectMgr::Free()
{
}
