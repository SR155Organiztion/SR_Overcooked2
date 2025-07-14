#include "pch.h"
#include "CUi_WarningBox.h"

#include "CManagement.h"

CUi_WarningBox::CUi_WarningBox():CUi()
{
}

CUi_WarningBox::CUi_WarningBox(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev)
{
}

CUi_WarningBox::CUi_WarningBox(const CGameObject& rhs):CUi(rhs)
{
}

CUi_WarningBox::~CUi_WarningBox()
{
}

HRESULT CUi_WarningBox::Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	m_tData.m_bVisible = false;
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUi_WarningBox::Update_GameObject(const _float& _fTimeDelta)
{
	//if (!m_tData.m_bVisible)
		//return 0;


	int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CUi_WarningBox::LateUpdate_GameObject()
{
	/*for (auto it = m_listData.begin(); it != m_listData.end(); )
	{
		if (it->m_bVisible == false)
		{
			it = m_listData.erase(it);
		}
		else
		{
			++it;
		}
	}*/
}

void CUi_WarningBox::Render_GameObject()
{
	if (!m_tData.m_bVisible)
	{
		/*wchar_t buf[128];
		swprintf_s(buf, 128, L"Render: visible = %d", m_tData.m_bVisible);
		MessageBox(0, buf, L"WarningBox", 0);
	*/
			return;
	}
	if (!m_pTransformCom) 
	{
		//MessageBox(0, L"TransformCom is nullptr!", L"Warning", 0);
	}

		/*D3DXMATRIX matView;
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
		matBillboard._33 = matView._33;*/

		const _matrix* matWorld = m_pTransformCom->Get_World();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

		m_pTextureCom->Set_Texture(0); 
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	
}

CGameObject* CUi_WarningBox::Make_WarningBox(bool _m_bVisible)
{
	CUi_WarningBox* pGameObject = new CUi_WarningBox(m_pGraphicDev); // 지금 만들어주는 이 게임오브젝트에 컴포넌트를 셋 해줘야한다.
	if (!pGameObject) {
		MessageBox(0, L"new로 WarningBox 생성 실패", 0, 0);
		return nullptr;
	}
	
	pGameObject->Add_Component();
	UIDATA* pData = pGameObject->Get_UiData();
	if (!pData) {
		MessageBox(0, L"pData가 nullptr!", 0, 0);
		return nullptr;
	}

	pGameObject->m_tData.m_bVisible = TRUE;

	if (pGameObject->m_tData.m_bVisible)
	{
		
		pData->m_vScale = { 1.f, 1.f, m_tData.m_vScale.z };
		pGameObject->m_pTransformCom->Set_Scale(pData->m_vScale);
		
		CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
		static _int iWarningCount = 0;
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, L"Object_Warning%d", iWarningCount++); // 레이어 추가 및 이름 변경
		if (FAILED(pLayer->Add_GameObject(szFileName, pGameObject)))
			return nullptr;

		m_listData.push_back(*pData);
		return pGameObject;
	}

	else
	{
		return pGameObject;
	}
}

HRESULT CUi_WarningBox::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Warning"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_CTexture", pComponent });


	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_CBuffer", pComponent });


	return S_OK;
}
void CUi_WarningBox::UpdatePosition(const _vec3& _vPos)
{
	
	m_pTransformCom->Set_Pos(m_tData.m_vPos.x = _vPos.x, m_tData.m_vPos.y = _vPos.y + CookLodingYOffset, m_tData.m_vPos.z = _vPos.z);
}
 
void CUi_WarningBox::Free()
{

}