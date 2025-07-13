#pragma once
#include "CUi_Gauge.h"
#include "CIngredient.h"
#include "CUi_CookLodingBox.h"

class CUi_CookLoding : public CUi_Gauge
{
private:
	Engine::CTransform* m_pTransformCom2;
	Engine::CTexture* m_pTextureCom2;
	Engine::CVIBuffer* m_pBufferCom;

	// 코드컨벤션은 정말 잘 지켜주셔야합니다.
	// float형 변수 앞에는 꼭f를 붙혀주시고 해당 변수는 멤버변수이기 때문에 m_도 같이 붙혀주셔야합니다.
	// ex) m_fCookLoadingYOffset
	// 오타가 난 부분들은 발견한 즉시 바로 고쳐야 나중에 고생하지 않습니다.
	// ex) Loding > Loading
	_float CookLodingYOffset = 2.f;
	_float m_fMaxCookTime = 1.f; /// << 최대 조리시간입니다. 누나가 쓰셨던 limitTime역할을 하는겁니다.*******
	_float	m_fProgress = 0.f;	///< 현재 가공 진행도 (0.0f ~ 1.0f) (가열 조리는 0.0f ~2.0f)

	_bool	m_bProcess = false;	///< 현재 가공 중인지 여부
	_bool	m_bIsShow = true; // 현재 보여줄건지 여부 입니다.

	CUi_CookLodingBox* m_pOwnerBox = nullptr; // 이 게이지를 가지는 박스의 포인터 입니다.

public:
	CUi_CookLoding(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_CookLoding(const CGameObject& rhs);
	~CUi_CookLoding();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	// m_는 멤버변수 앞에 붙히는 prefix(접두어)입니다. 매개변수에는 통상적으로 그냥 _만 붙혀줍니다.
	CGameObject* Make_cookLoding(bool _m_bProcess, CGameObject* _pLoadingBox, _float _m_fProgress = 0.f);
	HRESULT Add_Component();
	void UpdatePosition( const _vec3& _vPos);
	void Delete_Gauge(CGameObject* _pGameObject); // 레이어에서 게이지 UI를 지워주는 함수를 제공해 줄겁니다.
												  // 그래야 사용하는 입장에서 좀 더 편하게 사용 할 수 있습니다.
												  // 항상 제가 만든 클래스 혹은 함수를 사용하는 사람 입장을 생각하면서 만드셔야합니다.
												  // 그래야 어떤 매개변수를 받을지 어떤건 내가 자동으로 처리해줄지 명확히 기준이 세워집니다.
												  // 또 다른 관점으로 보자면 사용하는 사람이 오타나 내가 원치않는 행동을 하지 않게 강제하는 효과도
												  // 줄 수 있습니다.

	// 프로그레스를 설정해주면 업데이트 돌 때 알아서 올라갑니다.
	void Set_Progress(float _fProgress) {
		m_fProgress = _fProgress;
	}

	void Show() {
		m_bIsShow = TRUE;
	}

	void Hide() {
		m_bIsShow = FALSE;
	}

private:
	void Free();
};

