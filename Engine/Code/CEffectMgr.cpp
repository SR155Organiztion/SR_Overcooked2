#include "CEffectMgr.h"
#include "CTransform.h"

IMPLEMENT_SINGLETON(CEffectMgr)

CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
	Free();
}

HRESULT CEffectMgr::Ready_ProtoEffect(std::wstring EffectName, CEffect* pEffect)
{
	auto iter = m_mapProtoEffect.find(EffectName);
	if (iter != m_mapProtoEffect.end()) {
		//Safe_Release(iter->second); // ��ĥ ��, ���� Proto ����
		//m_mapProtoEffect.erase(iter);
		return S_OK;
	}
	m_mapProtoEffect[EffectName] = pEffect;
	return S_OK;
}

HRESULT CEffectMgr::Reserve_Effect(std::wstring EffectName, size_t Count)
{
	std::wstring SetEffectName = L"Proto_" + EffectName;
	auto iter = m_mapProtoEffect.find(SetEffectName);
	if (iter == m_mapProtoEffect.end()) {
		MSG_BOX("EffectMgr Reserve Failed");
		return E_FAIL;
	}
	for (size_t i = 0; i < Count; ++i) {
		CEffect* CloneEffect = iter->second->Clone();
		m_mapEffect[EffectName].push_back(CloneEffect);
	}
	return S_OK;
}

_bool CEffectMgr::Play_Effect(std::wstring EffectName, CGameObject* Owner)
{
	auto iter = m_mapEffect.find(EffectName);
	if (iter == m_mapEffect.end()) {
		return false;
	}
	_vec3 vOwnerPos;
	dynamic_cast<CTransform*>(Owner->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vOwnerPos);
	
	auto& vecEffect = iter->second; // Ű������ �����ִ� CEffect*�� ������ �ִ� ���� �����̳�
	for (auto& pEffect : vecEffect) {
		if (!pEffect->Get_EffectActive()) { //Ȱ��ȭ�Ǿ����� ���� ����Ʈ ���
			pEffect->Play_Effect(vOwnerPos);
			return true;
		}
	 }
	return false;
}

_int CEffectMgr::Update_Effect(const _float dt)
{
	for (auto& mapKey : m_mapEffect) { // ���� Ű�� ��ȸ
		for (auto& pEffect : mapKey.second) { // ���� ��ȸ
			if (pEffect->Get_EffectActive()) {
				pEffect->Update_Effect(dt);
			}
		}
	}
	
	return S_OK;
}

void CEffectMgr::LateUpdate_Effect(const _float dt)
{
	for (auto& mapKey : m_mapEffect) { // ���� Ű�� ��ȸ
		for (auto& pEffect : mapKey.second) { // ���� ��ȸ
			if (pEffect->Get_EffectActive()) {
				pEffect->LateUpdate_Effect(dt);
			}
		}
	}
}

void CEffectMgr::Free()
{
	for_each(m_mapProtoEffect.begin(), m_mapProtoEffect.end(), CDeleteMap());
	m_mapProtoEffect.clear();

	for (auto& vecEffect : m_mapEffect)       
	{
		for_each(vecEffect.second.begin(), vecEffect.second.end(), CDeleteObj());
		vecEffect.second.clear();
	}

	m_mapEffect.clear();
	
}
