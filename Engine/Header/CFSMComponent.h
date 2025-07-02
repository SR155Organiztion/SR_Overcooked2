#pragma once
#include "CComponent.h"
#include "CGameObject.h"
#include "CState.h"

BEGIN(Engine)

class ENGINE_DLL CFSMComponent : public CComponent
{
private:
	unordered_map<string, CState*> m_mapState;
	CState* m_eCurState = nullptr;
	std::string strCurStateName = "";
	CGameObject* m_pOwner = nullptr;

private:
	explicit CFSMComponent();
	explicit CFSMComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFSMComponent(const CFSMComponent& rhs);
	virtual ~CFSMComponent();

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override {}

	HRESULT	Ready_FSMComponent();
	void	Add_State(std::string stateName, CState* _state);
	void	Change_State(std::string stateName);
	std::string GerCurrStateName() const { return strCurStateName; }

public:
	virtual CComponent* Clone() override;
	static	CFSMComponent* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	/**
	* @brief ���¸ӽ��� ��ü(����) ����
	* @param CGameObject* pOwner - ���� �ڱ� �ڽ� �־��ָ�� (this)
	*/
	void	Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

private:
	virtual void Free() override;
};

END