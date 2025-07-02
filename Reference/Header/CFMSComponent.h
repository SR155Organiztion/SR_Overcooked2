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
	* @brief ���¸ӽ��� ��ü(����) ����
	* @param CGameObject* pOwner - ���� �ڱ� �ڽ� �־��ָ�� (this)
	*/
	void	Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

private:
	virtual void Free() override;
};

END