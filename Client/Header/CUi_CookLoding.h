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

	// �ڵ��������� ���� �� �����ּž��մϴ�.
	// float�� ���� �տ��� ��f�� �����ֽð� �ش� ������ ��������̱� ������ m_�� ���� �����ּž��մϴ�.
	// ex) m_fCookLoadingYOffset
	// ��Ÿ�� �� �κе��� �߰��� ��� �ٷ� ���ľ� ���߿� ������� �ʽ��ϴ�.
	// ex) Loding > Loading
	_float CookLodingYOffset = 2.f;
	_float m_fMaxCookTime = 1.f; /// << �ִ� �����ð��Դϴ�. ������ ���̴� limitTime������ �ϴ°̴ϴ�.*******
	_float	m_fProgress = 0.f;	///< ���� ���� ���൵ (0.0f ~ 1.0f) (���� ������ 0.0f ~2.0f)

	_bool	m_bProcess = false;	///< ���� ���� ������ ����
	_bool	m_bIsShow = true; // ���� �����ٰ��� ���� �Դϴ�.

	CUi_CookLodingBox* m_pOwnerBox = nullptr; // �� �������� ������ �ڽ��� ������ �Դϴ�.

public:
	CUi_CookLoding(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_CookLoding(const CGameObject& rhs);
	~CUi_CookLoding();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	_int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	// m_�� ������� �տ� ������ prefix(���ξ�)�Դϴ�. �Ű��������� ��������� �׳� _�� �����ݴϴ�.
	CGameObject* Make_cookLoding(bool _m_bProcess, CGameObject* _pLoadingBox, _float _m_fProgress = 0.f);
	HRESULT Add_Component();
	void UpdatePosition( const _vec3& _vPos);
	void Delete_Gauge(CGameObject* _pGameObject); // ���̾�� ������ UI�� �����ִ� �Լ��� ������ �̴ٰϴ�.
												  // �׷��� ����ϴ� ���忡�� �� �� ���ϰ� ��� �� �� �ֽ��ϴ�.
												  // �׻� ���� ���� Ŭ���� Ȥ�� �Լ��� ����ϴ� ��� ������ �����ϸ鼭 ����ž��մϴ�.
												  // �׷��� � �Ű������� ������ ��� ���� �ڵ����� ó�������� ��Ȯ�� ������ �������ϴ�.
												  // �� �ٸ� �������� ���ڸ� ����ϴ� ����� ��Ÿ�� ���� ��ġ�ʴ� �ൿ�� ���� �ʰ� �����ϴ� ȿ����
												  // �� �� �ֽ��ϴ�.

	// ���α׷����� �������ָ� ������Ʈ �� �� �˾Ƽ� �ö󰩴ϴ�.
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

