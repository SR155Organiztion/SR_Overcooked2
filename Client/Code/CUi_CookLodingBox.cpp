#include "pch.h"
#include "CUi_CookLodingBox.h"

//Engine
#include "CManagement.h"
#include "CTexture.h"
#include "CLayer.h"


CUi_CookLodingBox::CUi_CookLodingBox(LPDIRECT3DDEVICE9 pGraphicDev) : CUi_Gauge(pGraphicDev), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLodingBox::CUi_CookLodingBox(const CGameObject& rhs) : CUi_Gauge(rhs), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLodingBox::~CUi_CookLodingBox()
{
	Free();
}


HRESULT CUi_CookLodingBox::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CUi_CookLodingBox::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_tData.m_bProcess)
		return 0;
	m_tData.m_dwTime += _fTimeDelta;

	m_tData.m_bVisible = m_bProcess;

	if (m_tData.m_bIconDown)
	{
		m_pTransformCom->Set_Pos(m_tData.m_vPos.x, m_tData.m_vPos.y , m_tData.m_vPos.z-2);
	}
	else
	{
		m_pTransformCom->Set_Pos(m_tData.m_vPos.x, m_tData.m_vPos.y, m_tData.m_vPos.z);
	}
	
	int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CUi_CookLodingBox::LateUpdate_GameObject(const _float& _fTimeDelta)
{
	for (auto it = m_listData.begin(); it != m_listData.end(); )
	{
		if (!it->m_bVisible || !it->m_bProcess)
		{
			it = m_listData.erase(it);
	
		}
		else
		{
			++it;
		}

	}
}

void CUi_CookLodingBox::Render_GameObject()
{
	if (m_tData.m_bIsMgr)
	{
		if (m_tData.m_bProcess)
		{

			//_matrix matView;
			//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

			//_matrix matBillboard;
			//D3DXMatrixIdentity(&matBillboard);
			//matBillboard._11 = matView._11;
			//matBillboard._12 = matView._21;
			//matBillboard._13 = matView._31;
			//matBillboard._21 = matView._12;
			//matBillboard._22 = matView._22;
			//matBillboard._23 = matView._32;
			//matBillboard._31 = matView._13;
			//matBillboard._32 = matView._23;
			//matBillboard._33 = matView._33;

			//_vec3 vPos;
			//m_pTransformCom->Get_Info(INFO_POS, &vPos);
			//_matrix matTrans;
			//D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

			//_matrix matScale;
			//D3DXMatrixScaling(&matScale, m_tData.m_vScale.x, m_tData.m_vScale.y, m_tData.m_vScale.z);

			//_matrix matWorld = matScale * matBillboard * matTrans; // ���� = ������ * ������ * �巣��
			//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);


			const _matrix* matWorld = m_pTransformCom->Get_World();
			m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);
			m_pTextureCom->Set_Texture(0); //BOX
			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		}
	}
}

CGameObject* CUi_CookLodingBox::Make_cookLodingBox(bool _m_bProcess)
{

	CUi_CookLodingBox* pGameObject = new CUi_CookLodingBox(m_pGraphicDev);
	pGameObject->Add_Component();
	//UIDATA* pData = pGameObject->Get_UiData();

	pGameObject->m_tData.m_bProcess = _m_bProcess; //��� ����
	pGameObject->m_tData.m_bIsMgr = true;

	if (pGameObject->m_tData.m_bProcess)
	{
		pGameObject->m_tData.m_dwLimitTime = (DWORD)pGameObject->m_fProgress;
		pGameObject->m_tData.m_dwStartTime = GetTickCount64();
		pGameObject->m_tData.m_fAnimDuration = pGameObject->m_fProgress;

		_vec3 vScale = { 0.8f,0.5f, 1.f };
		pGameObject->m_tData.m_vScale = vScale;
		
		pGameObject->m_pTransformCom->Set_Scale(vScale);
		pGameObject->m_tData.m_fAnimTime = GetTickCount64();

		CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //���̾� �ҷ�����
		static _int iCookLodingBoxCount = 0;
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, L"Object_CookLodingBox%d", iCookLodingBoxCount++); // ������ ���̾� �߰� �� �̸� ����

		if (FAILED(pLayer->Add_GameObject(szFileName, pGameObject)))
			return nullptr;

		m_listData.push_back(m_tData);
		return pGameObject;
	}

	else
	{
		return nullptr;
	}

}

void CUi_CookLodingBox::UpdatePosition(const _vec3& _vPos)
{
	m_pTransformCom->Set_Pos(m_tData.m_vPos.x = _vPos.x, m_tData.m_vPos.y = _vPos.y + CookLodingYOffset, m_tData.m_vPos.z = _vPos.z + 0.0001f);
}

HRESULT CUi_CookLodingBox::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Cook"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_CTexture", pComponent });


	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_CBuffer", pComponent });


	return S_OK;
}


void CUi_CookLodingBox::Free()
{

}
