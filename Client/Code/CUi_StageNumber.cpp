#include "pch.h"
#include "CUi_StageNumber.h"

#include "CManagement.h"

CUi_StageNumber::CUi_StageNumber() :CUi()
{
}

CUi_StageNumber::CUi_StageNumber(LPDIRECT3DDEVICE9 pGraphicDev): CUi(pGraphicDev)
{
}

CUi_StageNumber::CUi_StageNumber(const CUi_StageNumber& rhs):CUi(rhs)
{
}

CUi_StageNumber::~CUi_StageNumber()
{
}

HRESULT CUi_StageNumber::Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//_vec3 vPos{ 10, 5,1 };  //★실험용
	//Make_StageNumber(true, 3, vPos);  //★실험용

	return S_OK;
}

_int CUi_StageNumber::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_tData.m_bProcess)
		return 0;

	int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CUi_StageNumber::LateUpdate_GameObject()
{

	for (auto it = m_listData.begin(); it != m_listData.end(); )
	{
		if (!it->m_bProcess)
		{
			it = m_listData.erase(it);

		}
		else
		{
			++it;
		}

	}
}

void CUi_StageNumber::Render_GameObject()
{
	if (!m_tData.m_bIsMgr && m_tData.m_bProcess)
	{
		D3DXMATRIX matScale, matRotX, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.6f, 0.3f, 1.0f);
		float fAngle = D3DXToRadian(90.0f);
		D3DXMatrixRotationX(&matRotX, fAngle);
		D3DXMatrixTranslation(&matTrans, m_tData.m_vPos.x, m_tData.m_vPos.y + 0.1, m_tData.m_vPos.z);
		matWorld = matScale * matRotX * matTrans;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pTextureCom->Set_Texture(m_tData.m_iNumber);
		m_pBufferCom->Render_Buffer();
				break;
			}

			m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
	
}

CGameObject* CUi_StageNumber::Make_StageNumber(int _Number, _vec3 _vPos)
{
	CUi_StageNumber* pGameObject = new CUi_StageNumber(m_pGraphicDev);
	pGameObject->Add_Component();
	UIDATA* pData = pGameObject->Get_UiData();

	pGameObject->m_tData.m_bIsMgr = false;
	pGameObject->m_tData.m_bProcess = true;
	pGameObject->m_tData.m_iNumber = _Number;
	pGameObject->m_tData.m_vPos = _vPos;
	m_pTransformCom->Set_Pos(_vPos.x, _vPos.y, _vPos.z);
	CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
	static _int iStageNumberCount = 0; 
	TCHAR		szFileName[128] = L""; 

	wsprintf(szFileName, L"Object_StageNumber%d", iStageNumberCount++); // 레이어 추가 및 이름 변경

	size_t len = wcslen(szFileName) + 1;
	wchar_t* pKey = new wchar_t[len];
	wcscpy_s(pKey, len, szFileName);

	if (FAILED(pLayer->Add_GameObject(pKey, pGameObject)))
		return nullptr;

	m_listData.push_back(pGameObject->m_tData); 
	return pGameObject;
	}
	
	else
	{
		return pGameObject;
	}
}

HRESULT CUi_StageNumber::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_SelectNumber"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_CTexture", pComponent });


	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_CBuffer", pComponent });


	return S_OK;
}
void CUi_StageNumber::UpdatePosition(const _vec3& _vPos)
{

	m_pTransformCom->Set_Pos(m_tData.m_vPos.x = _vPos.x, m_tData.m_vPos.y = _vPos.y, m_tData.m_vPos.z = _vPos.z);
}

void CUi_StageNumber::Free()
{

}