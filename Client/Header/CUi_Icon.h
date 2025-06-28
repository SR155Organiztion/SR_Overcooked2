#pragma once
#include "CUi.h"
class CUi_Icon : public CUi
{
private:
	map<_tchar*,LPDIRECT3DTEXTURE9*> m_mapIcon;
	_tchar* m_szName;
	_vec3 m_pPosition;
	LPDIRECT3DTEXTURE9 m_pTexIcon; //잠시 담아뒀다가 맵에 값을 옮기는 용도

private:
	CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Icon(const CGameObject& rhs);
	~CUi_Icon();

public:
	void Ready_GameObject(_tchar* m_szName, _vec3 m_pPosition, LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Update_GameObject();
	void LateUpdate_GameObject();
	void Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);

private:
	void Free();

};

