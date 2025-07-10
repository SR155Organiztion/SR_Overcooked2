#include "pch.h"
#include "CUi_Icon.h"
#include "CManagement.h"


/// <summary>
///  사용법: 외부에서 월드좌표를 매개변수로 받아와서 따라다니기 
/// </summary>
CUi_Icon::CUi_Icon()
{

}
CUi_Icon::CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev)
{
}

CUi_Icon::CUi_Icon(const CGameObject& rhs):CUi(rhs)
{
}

CUi_Icon::~CUi_Icon()
{
}

HRESULT CUi_Icon::Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	///Add_Component 호출, 기본 위치/크기/타입 초기화
	m_vPos = D3DXVECTOR3(0, 0, 0);   // 임시 위치 월드객체의 스크린좌표로 바꾸기
	m_tXScale = 1.0f;
	m_tYScale = 1.0f;
	Make_Icon(CIngredient::INGREDIENT_TYPE::FISH, m_vPos); //★실험용
	if (FAILED(Add_Component()))
		return E_FAIL;


	D3DXMATRIX matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMATRIX matBillboard;
	D3DXMatrixIdentity(&matBillboard);

	// 회전 부분만 카메라에서 복사
	matBillboard._11 = matView._11;
	matBillboard._12 = matView._21;
	matBillboard._13 = matView._31;
	matBillboard._21 = matView._12;
	matBillboard._22 = matView._22;
	matBillboard._23 = matView._32;
	matBillboard._31 = matView._13;
	matBillboard._32 = matView._23;
	matBillboard._33 = matView._33;

	D3DXMATRIX matTrans;
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());


	return S_OK;
}

int CUi_Icon::Update_GameObject(const _float& _fTimeDelta)
{
	
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	Engine::CTransform* pIconTransformCom = dynamic_cast<Engine::CTransform*>
		(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameObject_Layer", L"Player", L"Com_Transform"));

	if (nullptr == pIconTransformCom)
		return 0;

	_vec3   vPlayerPos{};
	pIconTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	float iconYOffset = 2.f;
	m_pTransformCom->Set_Pos(vPlayerPos.x, vPlayerPos.y + iconYOffset, vPlayerPos.z);

	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	Engine::CManagement::GetInstance()->Get_GameObject(L"Player", L"Com_Transform");




	return iExit;
}

void CUi_Icon::LateUpdate_GameObject()
{

}

void CUi_Icon::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	switch (m_eType)
	{
	case CIngredient::INGREDIENT_TYPE::SEAWEED:
	{
		m_pTextureCom->Set_Texture(6);
	}
	break;
	case CIngredient::INGREDIENT_TYPE::LETTUCE:
	{
		m_pTextureCom->Set_Texture(2);
	}
	break;
	case CIngredient::INGREDIENT_TYPE::TOMATO:
	{
		m_pTextureCom->Set_Texture(7);
	}
	break;
	case CIngredient::INGREDIENT_TYPE::CUCUMBER:
	{
		m_pTextureCom->Set_Texture(0);
	}
	break;
	case CIngredient::INGREDIENT_TYPE::FISH:
	{
		m_pTextureCom->Set_Texture(1);
	}
	break;
	case CIngredient::INGREDIENT_TYPE::SHRIMP:
	{
		m_pTextureCom->Set_Texture(4);
	}
	break;
	case CIngredient::INGREDIENT_TYPE::RICE:
	{
		m_pTextureCom->Set_Texture(5);
	}
	case CIngredient::INGREDIENT_TYPE::PASTA :
	{
		m_pTextureCom->Set_Texture(3);
	}
	break;
	}
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

HRESULT CUi_Icon::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Icon"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Object", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Icon2"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CUi_Icon::Make_Icon(CIngredient::INGREDIENT_TYPE _m_eType, _vec3 _pos)
{
	float iconYOffset = 20.f;
	m_eType = _m_eType;
	m_vObjectPos = _pos;
	m_vObjectPos.y += iconYOffset;
}

void CUi_Icon::Free()
{
}