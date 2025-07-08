#pragma once
#include "CUi_Object.h"
#include "CTransform.h"

class CUi_Order :  public CUi
{

private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CSprite* m_pSpriteCom2;
	Engine::CSprite* m_pSpriteCom3;

	/*vector<CSprite*> m_vecSprite;*/
	vector<wstring> m_vecType; //이름 저장
	vector<CUi_Order*> m_vecOrder; //m_bVisible 상태 저장
	vector<float> m_vecTime; //시간 저장

	OBJECT_TYPE m_eObjectType;

	RECT* m_pSrcRect; //위치
	DWORD m_dwStartTime; //시작 시간
	DWORD m_dwLimitTime; //제한 시간
	DWORD m_dwTime; //남은 시간
	DWORD m_dwHideTime; //사라지는 시간

	bool m_bAnimating;    // 애니메이션 중 여부
	float m_fAnimTime;     // 현재 애니메이션 시간
	float m_fAnimDuration; // 애니메이션 총 소요 시간(초)
	D3DXVECTOR3 m_vStartPos;      // 시작 위치
	D3DXVECTOR3 m_vTargetPos;     // 최종 위치(도달해야할 자리)
	D3DXVECTOR3 m_vPos;           // 현재 위치(렌더링에 사용)

	int m_iseconds;
	int m_iminute;
	int m_pGauge;

	bool m_bVisible;

public:
	CUi_Order();
	CUi_Order(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Order(const CUi_Order& rhs);
	~CUi_Order();

	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();
	HRESULT Add_Component();

	void Get_Order(wstring _Name, float _time);

	DWORD Get_StartTIme() 
	{
		return m_dwStartTime;
	}

	DWORD Get_LimitTIme() 
	{
		return m_dwLimitTime;
	}

	DWORD Get_Time() 
	{
		return m_dwTime;
	}



private:
	void Free();

};

