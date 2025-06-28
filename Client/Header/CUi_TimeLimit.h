#pragma once
#include "CUi_Object.h"
class CUi_TimeLimit : public CUi_Object
{
private:
	float m_fmainTime; //제한 시간, 초기값은 최대 시간으로 설정
	float m_fMaxTime; //최대 시간
	_tchar* m_szTime[5]; // 표시할 시간 (숫자)

private:
	CUi_TimeLimit(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_TimeLimit(const CGameObject& rhs);
	~CUi_TimeLimit();

public: 
	void Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Update_GameObject();
	void Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
};

