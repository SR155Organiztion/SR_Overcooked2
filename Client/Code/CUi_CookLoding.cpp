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
	int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	m_dwTime += _fTimeDelta;

	//if (m_bProcess)
	//{

	//	DWORD dwCurTime = GetTickCount64();
	//	float elapsed = (float)(dwCurTime - m_tData.m_dwStartTime); //경과 시간
	//	float percent = (dwCurTime - m_tData.m_dwStartTime) / m_fProgress;
	//	if (percent > 1.0f) percent = 1.0f;
	//	if (percent < 0.0f) percent = 0.0f;

	//	
	//	/*if (percent > m_tData.m_dwLimitTime )
	//	{
	//		m_bProcess = false;
	//		m_tData.m_bVisible = false;
	//	}
	//	else if (percent < 0)
	//	{
	//		percent = 0;
	//	}*/
	//}

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	
	return iExit;
}

void CUi_CookLoding::LateUpdate_GameObject(const _float& _fTimeDelta)
{
	for (auto it = m_listData.begin(); it != m_listData.end(); )
	{
		if (it->m_bVisible == false)
		{
			it = m_listData.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void CUi_CookLoding::Render_GameObject()
{
	if (!m_bProcess) 
		return;

	/*if (_tcscmp(this->m_szSelfId, L"Object_CookLoding0") == 0)
		m_bProcess = m_bProcess; */

	CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer");

	m_vBasePos = { m_tData.m_vPos.x, m_tData.m_vPos.y, m_tData.m_vPos.z };

	DWORD dwCurTime = GetTickCount64();
	float elapsed = (float)(dwCurTime - m_tData.m_dwStartTime);
	float percent = elapsed / m_fProgress;
	if (percent > 1.0f) percent = 1.0f;
	if (percent < 0.0f) percent = 0.0f;

	float width = m_tData.m_vScale.x; // 버퍼 가로
	_vec3 vScale = { percent, m_tData.m_vScale.y, m_tData.m_vScale.z };
	m_pTransformCom2->Set_Scale(vScale);

	float offset = -width * (0.05f - percent) * 0.5f;
	m_pTransformCom2->Set_Pos(m_vBasePos.x + offset, m_vBasePos.y, m_vBasePos.z);

	m_pTextureCom2->Set_Texture(1);
	m_pBufferCom->Render_Buffer();

	//float width = 1;
	//_vec3 vScale = { 0.1f, m_tData.m_vScale.y, m_tData.m_vScale.z };
	//m_pTransformCom2->Set_Scale(vScale);
	//_vec3 vPos;
	//m_pTransformCom2->Get_Info(INFO::INFO_POS, &vPos);
	//
	//float offset = -width * (1.0f - vScale.x) * 0.5f;
	//vPos.x = vPos.x - offset;
	//m_pTransformCom2->Set_Pos(vPos.x, vPos.y, vPos.z);


	//D3DXMATRIX matView;
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//D3DXMATRIX matBillboard;
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


	//const _matrix* matWorld = m_pTransformCom2->Get_World();
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);
	//m_pTextureCom2->Set_Texture(1); //게이지
	//m_pBufferCom->Render_Buffer();

	//
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	
}

CGameObject* CUi_CookLoding::Make_cookLoding( bool _m_bProcess, float _m_fProgress)
{
	
	CUi_CookLoding* pGameObject = new CUi_CookLoding(m_pGraphicDev); // 지금 만들어주는 이 게임오브젝트에 컴포넌트를 셋 해줘야한다.
	pGameObject->Add_Component();
	UIDATA* pData = pGameObject->Get_UiData();

	pGameObject->m_bProcess = _m_bProcess; //사용 여부
	pGameObject->m_fProgress = _m_fProgress * 1000.f ; //사용 시간

	if (pGameObject->m_bProcess)
	{
		
		pData->m_dwLimitTime = (DWORD)pGameObject->m_fProgress;
		pData->m_dwStartTime = GetTickCount64();
		pData->m_fAnimDuration = pGameObject->m_fProgress;

		_vec3 vScale = { 0.3f,0.2f, m_tData.m_vScale.z };
		pData->m_vScale =  vScale;
		m_pTransformCom2->Set_Scale(vScale);
		pData->m_fAnimTime = GetTickCount64();

		CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
		static _int iCookLodingCount = 0;
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, L"Object_CookLoding%d", iCookLodingCount++); // 아이콘 레이어 추가 및 이름 변경

		if (FAILED(pLayer->Add_GameObject(szFileName, pGameObject)))
			return nullptr;

		m_listData.push_back(*pData);
		return pGameObject;
	}

	else
	{
		return nullptr;
	}

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
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_CTexture2", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_CBuffer", pComponent });


	return S_OK;
}


void CUi_CookLoding::Free()
{
	
}
