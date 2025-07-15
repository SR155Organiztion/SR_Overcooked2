#include "pch.h"
#include "CInteract.h"
#include "CManagement.h"
#include "CCamera.h"
#include "CTransform.h"

CInteract::CInteract(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bGround(false), m_bHighlight(false)
{
	ZeroMemory(m_szTemp, sizeof(m_szTemp));
}

CInteract::CInteract(const CGameObject& rhs)
	: Engine::CGameObject(rhs), m_bGround(false), m_bHighlight(false)
{
	ZeroMemory(m_szTemp, sizeof(m_szTemp));
}

CInteract::~CInteract()
{
}

void CInteract::Billboard(_matrix& matWorld)
{
	_matrix matView, matBill;

	CGameObject* pCamera = CManagement::GetInstance()->Get_GameObject(L"Environment_Layer", L"DynamicCamera");
	matView = *dynamic_cast<CCamera*>(pCamera)->Get_View();
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._12 = matView._12;
	matBill._13 = matView._13;
	matBill._21 = matView._21;
	matBill._22 = matView._22;
	matBill._23 = matView._23;
	matBill._31 = matView._31;
	matBill._32 = matView._32;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);
	matWorld = matBill * matWorld;
}

void CInteract::Free()
{
	Engine::CGameObject::Free();
}