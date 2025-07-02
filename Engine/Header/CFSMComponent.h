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
	* @brief 상태머신의 주체(주인) 설정
	* @param CGameObject* pOwner - 보통 자기 자신 넣어주면됨 (this)
	*/
	void	Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

private:
	virtual void Free() override;
};

END