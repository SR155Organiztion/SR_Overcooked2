#include "pch.h"
#include "CUi_Icon.h"
#include "CManagement.h"
#include "CUi_Factory.h"

/// <summary>
///  사용법: 외부에서 월드좌표를 매개변수로 받아와서 따라다니기 
/// </summary>
CUi_Icon::CUi_Icon(): CUi(nullptr)
, m_pTransformCom(nullptr)
, m_pTextureCom(nullptr)
, m_pBufferCom(nullptr)
{

}
CUi_Icon::CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev)
, m_pTransformCom(nullptr)
, m_pTextureCom(nullptr)
, m_pBufferCom(nullptr)
{
}

CUi_Icon::CUi_Icon(const CUi_Icon& rhs):CUi(rhs)
, m_pTransformCom(rhs.m_pTransformCom)
, m_pTextureCom(rhs.m_pTextureCom)
, m_pBufferCom(rhs.m_pBufferCom)
{
}

CUi_Icon::~CUi_Icon()
{
}

HRESULT CUi_Icon::Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	
	if (FAILED(Add_Component()))
		return E_FAIL;



	return S_OK;
}

int CUi_Icon::Update_GameObject(const _float& _fTimeDelta)
{
	
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
		
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	Engine::CManagement::GetInstance()->Get_GameObject(L"Player", L"Com_Transform");


		int xPos = 30;
		if (!m_listIcon.empty())
		{
			const auto& lastOrder = m_listIcon.back();
			xPos = (int)lastOrder.m_vTargetPos.x + lastOrder.m_iWidth * lastOrder.m_vScale.x + lastOrder.m_iGap;
			m_tData.m_vTargetPos = D3DXVECTOR3(xPos, 20, 0);
			m_listIcon.push_back(m_tData);
		}
		else if (m_listIcon.empty())
		{
			m_tData.m_vTargetPos = D3DXVECTOR3(xPos, 20, 0);
			m_listIcon.push_back(m_tData);
		}
	


	return iExit;
}

void CUi_Icon::LateUpdate_GameObject()
{
}

void CUi_Icon::Render_GameObject()
{
	
	D3DXMATRIX matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matBillboard;
	D3DXMatrixIdentity(&matBillboard);
	matBillboard._11 = matView._11;
	matBillboard._12 = matView._21;
	matBillboard._13 = matView._31;
	matBillboard._21 = matView._12;
	matBillboard._22 = matView._22;
	matBillboard._23 = matView._32;
	matBillboard._31 = matView._13;
	matBillboard._32 = matView._23;
	matBillboard._33 = matView._33;

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, m_tData.m_vScale.x, m_tData.m_vScale.y, m_tData.m_vScale.z);
	D3DXMATRIX matWorld = matScale * matBillboard * matTrans; // 월드 = 스케일 * 빌보드 * 드랜스
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	

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
	break;
	case CIngredient::INGREDIENT_TYPE::PASTA :
	{
		m_pTextureCom->Set_Texture(3);
	}
	break;
	case CIngredient::INGREDIENT_TYPE::TOMATOSOUP:
	{
		m_pTextureCom->Set_Texture(7);
	}
	break;
	}
	
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

HRESULT CUi_Icon::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

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



CGameObject* CUi_Icon::Make_Icon(CIngredient::INGREDIENT_TYPE _eType)
{
	CUi_Icon* pGameObject = new CUi_Icon(m_pGraphicDev);
	pGameObject->Add_Component();
	UIDATA* pData = pGameObject->Get_UiData();
	pGameObject->Set_Icon(_eType);
	pData->m_vScale = { 1.f, 1.f, 1.f };
	pGameObject->m_pTransformCom->Set_Scale(m_tData.m_vScale);
	pData->m_vPos.y += iconYOffset;
	pData->m_iWidth = 119.f;
	pData->m_iGap = 5.f;
	pData->m_dwStartTime = GetTickCount64();
	pData->m_bAnimating = true;
	pData->m_fAnimTime = 0.0f;
	pData->m_fAnimDuration = 0.5f;

	CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
	
	int xPos = 30;
	if (!m_listIcon.empty())
	{
		const auto& lastIcon = m_listIcon.back();
		xPos = (int)lastIcon.m_vTargetPos.x + lastIcon.m_iWidth * lastIcon.m_vScale.x + lastIcon.m_iGap;
		m_tData.m_vTargetPos = D3DXVECTOR3(xPos, 20, 0);
		m_listIcon.push_back(m_tData);
	}
	else if (m_listIcon.empty())
	{
		m_tData.m_vTargetPos = D3DXVECTOR3(xPos, 20, 0);
		m_listIcon.push_back(m_tData);
	}

	if (nullptr == pGameObject)
		return nullptr;

	static _int iCount = 0;
	TCHAR		szFileName[128] = L"";
	wsprintf(szFileName, L"Object_Icon%d", iCount++); // 아이콘 레이어 추가 및 이름 변경

	if (FAILED(pLayer->Add_GameObject(szFileName, pGameObject)))
		return nullptr;

	return pGameObject;
}

void CUi_Icon::UpdatePosition(const _vec3& _vPos)
{
	m_pTransformCom->Set_Pos(_vPos.x, _vPos.y + iconYOffset, _vPos.z);

}

void CUi_Icon::Free()
{
	for (auto& pair : m_mapComponent[ID_STATIC])
		if (pair.second) 
		{ 
			pair.second->Release(); 
		}	

	for (auto& pair : m_mapComponent[ID_DYNAMIC])
		if (pair.second)
		{
			pair.second->Release();
		}
}