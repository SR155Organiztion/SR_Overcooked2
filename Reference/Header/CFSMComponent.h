/**
* @mainpage FSM������Ʈ
*/

#pragma once
#include "CComponent.h"
#include "CGameObject.h"
#include "CState.h"
#include "CTransform.h"

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
	/**
	* @brief UpdateŸ��
	* @detail ���� State�� �ִ��� Ȯ��(����ó��), State_Update�� TestForExit�� ��ȯ
	*/
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override {}

	HRESULT	Ready_FSMComponent();
	/**
	* @brief State �߰�
	* @detail Ű�� string, 
	* @param stateName - string���� �̷���� "key" , _state - ���⿡�� State �����Ҵ��ϴ°��� ��õ new CStateMove
	*/
	void	Add_State(std::string stateName, CState* _state);
	/**
	* @brief State ��ȯ
	* @detail ���� TestForExit ������ �ִ°� ���� "����"
	* @param stateName - ��ϵ� State�� string key ���� �� "key"
	*/
	void	Change_State(std::string stateName);
	/**
	* @brief ���� ����Ű�� �ִ� State Ž��
	*/
	std::string GerCurrStateName() const { return strCurStateName; }

public:
	virtual CComponent* Clone() override;
	/**
	* @brief ������Ʈ ����
	* @detail ���� �� ������Ʈ�� Set_Owner �ݵ�� ȣ���� ��
	*/
	static	CFSMComponent* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	/**
	* @brief ���¸ӽ��� ��ü(����) ����
	* @param CGameObject* pOwner - ���� �ڱ� �ڽ� �־��ָ�� (this)
	*/
	/**
	* @brief State�� ������ ����
	* @detail ������Ʈ ���� �� �ٷ� �ִ°� ���� "����". Set_Owner(this)�� �ִ°� ��õ
	*/
	void	Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

private:
	virtual void Free() override;
};

END