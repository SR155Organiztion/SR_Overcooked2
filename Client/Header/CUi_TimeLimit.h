#pragma once
#include "CUi_Object.h"
class CUi_TimeLimit : public CUi_Object
{
private:
	DWORD m_dwLimitTime; //���� �ð�, �ʱⰪ�� �ִ� �ð����� ����
	DWORD m_dwTime; //���� �ð�
	DWORD m_dwStartTime; //���� �ð�
	RECT* m_pSrcRect;
	int m_iseconds;
	int m_iminute;
	
	LPD3DXFONT m_pFont;
	LPD3DXSPRITE m_pSprite;

public:
	CUi_TimeLimit();
	CUi_TimeLimit(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_TimeLimit(const CGameObject& rhs);
	~CUi_TimeLimit();

public: 
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, GAUGE_TYPE _type);
	int Update_GameObject(const _float& _fTimeDelta);
	void Render_GameObject();

private:
	void Free();
};

