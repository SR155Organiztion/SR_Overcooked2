#pragma once
#include "CUi_Gauge.h"
class CUi_GameLoding :  public CUi_Gauge
{

private:

	Engine::CSprite* m_pSpriteCom; //��׶��� �̹���
	Engine::CSprite* m_pSpriteCom2; //������ 
	Engine::CSprite* m_pSpriteCom3; //���� �����̴� ������
	LPD3DXFONT m_pFont;
	LPD3DXSPRITE m_pSprite;

	_float m_dwTime = 0.f;

public:
	CUi_GameLoding();
	CUi_GameLoding(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_GameLoding(const CGameObject& rhs);
	~CUi_GameLoding();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	HRESULT Add_Component();
	void Make_GameLoding(bool _bVisible); 

	/// ��� ���� �ε��� ����Ǿ������� ��Ÿ���� ���� �и��ʷ� �Ѱ��ּ���. 
	void Set_Loding(float _dwTime) { m_tData.m_dwTime = _dwTime;}

protected:
	void Free();
};

