#pragma once
#include "CComponent.h"
#include "CGameObject.h"
#include "CState.h"

BEGIN(Engine)

class ENGINE_DLL CFSMComponent : public CComponent
{
private:
	unordered_map<const _tchar*, CState*> m_mapState;
	CState* m_eCurState = nullptr;
	const _tchar* strCurStateName = L"";
	CGameObject* m_pOwner = nullptr;

private:
	explicit CFSMComponent();
	explicit CFSMComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFSMComponent(const CFSMComponent& rhs);
	virtual ~CFSMComponent();

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override {}

	void	Add_State(const _tchar* stateName, CState* _state);
	void	Change_State(const _tchar* stateName);
	const _tchar* GerCurrStateName() const { return strCurStateName; }

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