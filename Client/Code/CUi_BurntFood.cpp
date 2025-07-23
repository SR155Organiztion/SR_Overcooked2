#include "pch.h"
#include "CUi_BurntFood.h"
#include "CManagement.h"

CUi_BurntFood::CUi_BurntFood()
{
}

CUi_BurntFood::CUi_BurntFood(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

CUi_BurntFood::CUi_BurntFood(const CGameObject& rhs)
{
}

CUi_BurntFood::~CUi_BurntFood()
{
}

HRESULT CUi_BurntFood::Ready_GameObject(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	m_pGraphicDev = _pGraphicDev;
	if (FAILED(Add_Component()))
		return E_FAIL;
	
	return S_OK;
}

_int CUi_BurntFood::Update_GameObject(const _float& _fTimeDelta)
{

	if (m_tData.m_bProcess)
		return 0;
	
	fElapsed += _fTimeDelta;

	if (fElapsed > 1)
	{
		m_tData.m_bVisible = !m_tData.m_bVisible;
		fElapsed = 0;
	}
			
	int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CUi_BurntFood::LateUpdate_GameObject()
{

}

void CUi_BurntFood::Render_GameObject()
{
	if (!m_tData.m_bIsMgr || m_tData.m_bProcess)
	{
			
		_matrix matView;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		_matrix matBillboard;
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
		_matrix matTrans;
		D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z -= 1.5);

		_matrix matScale;
		D3DXMatrixScaling(&matScale, m_tData.m_vScale.x, m_tData.m_vScale.y, m_tData.m_vScale.z);

		_matrix matWorld = matScale * matBillboard * matTrans; // 월드 = 스케일 * 빌보드 * 드랜스
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		
	}

}

CGameObject* CUi_BurntFood::Make_BurntFood(bool _m_bVisible)
{
	CUi_BurntFood* pGameObject = new CUi_BurntFood(m_pGraphicDev);
	pGameObject->Add_Component();
	UIDATA* pData = pGameObject->Get_UiData();

	pGameObject->m_tData.m_bVisible = _m_bVisible;
	pGameObject->m_tData.m_bIsMgr = false;
	pGameObject->m_tData.m_bProcess = true;

	if (pGameObject->m_tData.m_bVisible)
	{
		pData->m_vScale = { 1.2f, 1.2f, 0.f };
		m_pTransformCom->Set_Scale(pGameObject->m_tData.m_vScale);
		CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
		static _int iBurntFoodCount = 0;
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, L"Object_BurntFood%d", iBurntFoodCount++); // 레이어 추가 및 이름 변경
		if (FAILED(pLayer->Add_GameObject(szFileName, pGameObject)))
			return nullptr;

		m_listData.push_back(*pData);
		return pGameObject;
	}
}

HRESULT CUi_BurntFood::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_BurntFood"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_CTexture", pComponent });


	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_CBuffer", pComponent });


	return S_OK;
}
void CUi_BurntFood::UpdatePosition(const _vec3& _vPos)
{
	m_pTransformCom->Set_Pos(m_tData.m_vPos.x = _vPos.x, m_tData.m_vPos.y = _vPos.y + CookLodingYOffset, m_tData.m_vPos.z = _vPos.z);
}

void CUi_BurntFood::Free()
{

}