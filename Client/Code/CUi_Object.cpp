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
HRESULT CUi_Object::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev, OBJECT_TYPE _type)
{
	return S_OK;
}
int CUi_Object::Update_GameObject(const _float& _fTimeDelta)
{
	return 0;
}
void CUi_Object::LateUpdate_GameObject(const _float& _fTimeDelta)
{ }
void CUi_Object::Render_GameObject()
{ }
HRESULT CUi_Object::Add_Order(LPDIRECT3DDEVICE9 pGraphicDev, OBJECT_TYPE _type)
{ }
void CUi_Object::Remove_Order(wstring& key)
{ }
void CUi_Object::Free()
{
}