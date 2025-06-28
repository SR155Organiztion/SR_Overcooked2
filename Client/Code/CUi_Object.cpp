#include "pch.h"
#include "CUi_Object.h"

CUi_Object::CUi_Object(LPDIRECT3DDEVICE9 pGraphicDev): CUi(pGraphicDev)
{
}

CUi_Object::CUi_Object(const CGameObject& rhs): CUi(rhs)
{
}

CUi_Object::~CUi_Object()
{
}
