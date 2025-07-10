#include "pch.h"
#include "CUi_Icon.h"

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

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

int CUi_Icon::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CUi_Icon::LateUpdate_GameObject()
{
}

void CUi_Icon::Render_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	switch (m_eType)
	{
	case CIngredient::INGREDIENT_TYPE::SEAWEED:
	{
		/*m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon6.png");*/
	}
	break;
	case CIngredient::INGREDIENT_TYPE::LETTUCE:
	{
		/*m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon2.png");*/
	}
	break;
	case CIngredient::INGREDIENT_TYPE::TOMATO:
	{
		/*m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon7.png");*/
	}
	break;
	case CIngredient::INGREDIENT_TYPE::CUCUMBER:
	{
		//m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon0.png");
	}
	break;
	case CIngredient::INGREDIENT_TYPE::FISH:
	{
		/*m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon1.png");*/
	}
	break;
	case CIngredient::INGREDIENT_TYPE::SHRIMP:
	{
		/*m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon4.png");*/
	}
	break;
	case CIngredient::INGREDIENT_TYPE::RICE:
	{
		/*m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon5.png");*/
	}
	case CIngredient::INGREDIENT_TYPE::PASTA :
	{
		/*m_pSpriteCom->Render_Sprite(m_tXScale, m_tYScale, nullptr, m_pCenter, m_vPos, L"../Bin/Resource/Texture/UI/in_game/Icon3.png");*/
	}
	break;
	}

}

HRESULT CUi_Icon::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcCol*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Icon"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });


	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
}

void CUi_Icon::Make_Icon(CIngredient::INGREDIENT_TYPE _m_eType)
{


}

void CUi_Icon::Free()
{
}