#include "CFSMComponent.h"
#include "CState.h"

CFSMComponent::CFSMComponent()
	:m_eCurState(nullptr)
{
}

CFSMComponent::CFSMComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CFSMComponent::CFSMComponent(const CFSMComponent& rhs)
	:CComponent(rhs)
{
}

CFSMComponent::~CFSMComponent()
{
}

_int CFSMComponent::Update_Component(const _float& fTimeDelta)
{
	if (!m_eCurState) { MSG_BOX("State Update Failed"); return E_FAIL; }

	m_eCurState->Update_State(m_pOwner, fTimeDelta);
	m_eCurState->TestForExit_State(m_pOwner);

	return S_OK;
}

HRESULT CFSMComponent::Ready_FSMComponent()
{
	return S_OK;
}

void CFSMComponent::Add_State(string stateName, CState* _state)
{
	m_mapState.insert({ stateName, _state });
}

void CFSMComponent::Change_State(string stateName)
{
	auto iter = m_mapState.find(stateName);
	if (iter != m_mapState.end())
	{
		m_eCurState = (*iter).second;
		m_eCurState->Enter_State(m_pOwner);
		strCurStateName = stateName;
	}
}
CFSMComponent* CFSMComponent::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFSMComponent* pFSMComponent = new CFSMComponent(pGraphicDev);

	if (FAILED(pFSMComponent->Ready_FSMComponent()))
	{
		Safe_Release(pFSMComponent);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pFSMComponent;
}

CComponent* CFSMComponent::Clone()
{
	return new CFSMComponent(*this);
}

void CFSMComponent::Free()
{
	for (auto& state : m_mapState)
	{
		delete state.second;
	}
	m_mapState.clear();
}
