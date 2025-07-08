#pragma once
#include "Engine_Define.h"
class IShadow {
public:
	IShadow() {};
	IShadow(LPDIRECT3DDEVICE9 _pGraphicDev): m_pShadowGraphicDev(_pGraphicDev) {}

protected:
	LPDIRECT3DDEVICE9 m_pShadowGraphicDev = nullptr;

public:
	const LPDIRECT3DDEVICE9 Get_GraphicDev() {
		return m_pShadowGraphicDev;
	}
};