/**
* @mainpage FSM컴포넌트
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
	* @brief Update타임
	* @detail 현재 State가 있는지 확인(예외처리), State_Update와 TestForExit를 순환
	*/
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override {}

	HRESULT	Ready_FSMComponent();
	/**
	* @brief State 추가
	* @detail 키값 string, 
	* @param stateName - string으로 이루어진 "key" , _state - 여기에서 State 동적할당하는것을 추천 new CStateMove
	*/
	void	Add_State(std::string stateName, CState* _state);
	/**
	* @brief State 전환
	* @detail 보통 TestForExit 시점에 넣는걸 강력 "권장"
	* @param stateName - 등록된 State의 string key 넣을 것 "key"
	*/
	void	Change_State(std::string stateName);
	/**
	* @brief 현재 가리키고 있는 State 탐색
	*/
	std::string GerCurrStateName() const { return strCurStateName; }

public:
	virtual CComponent* Clone() override;
	/**
	* @brief 컴포넌트 생성
	* @detail 생성 후 컴포넌트의 Set_Owner 반드시 호출할 것
	*/
	static	CFSMComponent* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	/**
	* @brief 상태머신의 주체(주인) 설정
	* @param CGameObject* pOwner - 보통 자기 자신 넣어주면됨 (this)
	*/
	/**
	* @brief State의 소유자 설정
	* @detail 컴포넌트 생성 후 바로 넣는걸 강력 "권장". Set_Owner(this)로 넣는걸 추천
	*/
	void	Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

private:
	virtual void Free() override;
};

END