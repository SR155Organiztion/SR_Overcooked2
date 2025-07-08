#pragma once
#include "CUi_Object.h"
#include "CTransform.h"

class CUi_Order :  public CUi
{

private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CSprite* m_pSpriteCom2;
	Engine::CSprite* m_pSpriteCom3;

	vector<CSprite*> m_vecSprite;

	vector<wstring> m_vecString;

	OBJECT_TYPE m_eObjectType;

	RECT* m_pSrcRect; //위치
	DWORD m_dwStartTime; //시작 시간
	DWORD m_dwLimitTime; //제한 시간
	DWORD m_dwTime; //남은 시간
	int m_iseconds;
	int m_iminute;
	int m_pGauge;

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

	DWORD Get_StartTIme() {
		return m_dwStartTime;
	}

	DWORD Get_LimitTIme() {
		return m_dwLimitTime;
	}

	DWORD Get_Time() {
		return m_dwTime;
	}

private:
	void Free();

};

