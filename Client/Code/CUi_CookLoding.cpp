#include "pch.h"
#include "CUi_CookLoding.h"

//Engine
#include "CManagement.h"
#include "CTexture.h"
#include "CLayer.h"


CUi_CookLoding::CUi_CookLoding(LPDIRECT3DDEVICE9 pGraphicDev): CUi_Gauge(pGraphicDev), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLoding::CUi_CookLoding(const CGameObject& rhs): CUi_Gauge(rhs), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLoding::~CUi_CookLoding()
{
	Free();
}


HRESULT CUi_CookLoding::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{

	if (FAILED(Add_Component()))
		return E_FAIL;
	
	return S_OK;
}

_int CUi_CookLoding::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_bProcess)
		return 0;
	float percent = m_fProgress / m_fMaxCookTime;
	percent = max(0.f, min(percent, 1.f));  // Clamp 0~1

	// ���� �߽� ��ġ (UpdatePosition���� ������ �� ����)
	_vec3 vBasePos = m_tData.m_vPos;  // UI ���� �� ���� ���� ��ġ

	// ���� ���� ����
	float offsetX = (1.f - percent) * 0.5f; // ������ �پ�� ���ݸ�ŭ ��������
	_vec3 vFinalPos = vBasePos;
	vFinalPos.x -= offsetX;  // ���� ����

	// ����
	m_pTransformCom2->Set_Scale({ percent, 1.f, 1.f });
	m_pTransformCom2->Set_Pos(vFinalPos.x, vFinalPos.y, vFinalPos.z);



	int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	
	return iExit;
}

void CUi_CookLoding::LateUpdate_GameObject(const _float& _fTimeDelta)
{
	
}

void CUi_CookLoding::Render_GameObject()
{
	if (!m_bProcess) 
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom2->Get_World());

	m_pTextureCom2->Set_Texture(1);
	m_pBufferCom->Render_Buffer();
}

CGameObject* CUi_CookLoding::Make_cookLoding( bool _m_bProcess, float _m_fProgress)
{
	
	CUi_CookLoding* pGameObject = new CUi_CookLoding(m_pGraphicDev); // ���� ������ִ� �� ���ӿ�����Ʈ�� ������Ʈ�� �� ������Ѵ�.
	pGameObject->Add_Component();
	UIDATA* pData = pGameObject->Get_UiData();

	pGameObject->m_bProcess = _m_bProcess; //��� ����
	// pGameObject->m_fProgress = _m_fProgress * 1000.f ; //��� �ð�
	pGameObject->m_fProgress = _m_fProgress;

	if (pGameObject->m_bProcess)
	{

		// ������Ʈ ����
		CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //���̾� �ҷ�����
		static _int iCookLodingCount = 0;
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, L"Object_CookLoding%d", iCookLodingCount++); // ������ ���̾� �߰� �� �̸� ����

		if (FAILED(pLayer->Add_GameObject(szFileName, pGameObject)))
			return nullptr;

		//m_listData.push_back(*pData);
		return pGameObject;
	}
	return nullptr;

}

void CUi_CookLoding::UpdatePosition(const _vec3& _vPos)
{
	m_pTransformCom2->Set_Pos(m_tData.m_vPos.x = _vPos.x, m_tData.m_vPos.y = _vPos.y + CookLodingYOffset, m_tData.m_vPos.z = _vPos.z);
}

HRESULT CUi_CookLoding::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom2 = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform2", pComponent });

	pComponent = m_pTextureCom2 = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Cook"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_CTexture2", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_CBuffer", pComponent });


	return S_OK;
}


void CUi_CookLoding::Free()
{
	
}
