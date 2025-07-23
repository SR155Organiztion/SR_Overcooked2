#include "pch.h"
#include "CUi_StageInfo.h"
#include "CManagement.h"

CUi_StageInfo::CUi_StageInfo(): CUi()
{
}

CUi_StageInfo::CUi_StageInfo(LPDIRECT3DDEVICE9 _pGraphicDev):CUi(_pGraphicDev)
{
}

CUi_StageInfo::CUi_StageInfo(const CGameObject& _rhs) :CUi(_rhs)
{
}

CUi_StageInfo::~CUi_StageInfo()
{
}

HRESULT CUi_StageInfo::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	m_pGraphicDev = _m_pGraphicDev;
	if (FAILED(Add_Component()))
		return E_FAIL;
	return S_OK;
}

int CUi_StageInfo::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CUi_StageInfo::LateUpdate_GameObject()
{
}

void CUi_StageInfo::Render_GameObject()
{
	const _matrix* matWorld = m_pTransformCom->Get_World();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	if (m_tData.m_bProcess && !m_tData.m_bIsMgr)
	{

		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


		switch (m_tData.m_iNumber)
		{
		case 1:
			m_pTextureCom->Set_Texture(0);
			m_pBufferCom->Render_Buffer();
			break;
		case 2:
			m_pTextureCom->Set_Texture(1);
			m_pBufferCom->Render_Buffer();
			break;
		case 3:
			m_pTextureCom->Set_Texture(2);
			m_pBufferCom->Render_Buffer();
			break;
		case 4:
			m_pTextureCom->Set_Texture(3);
			m_pBufferCom->Render_Buffer();
			break;
		case 5:
			m_pTextureCom->Set_Texture(4);
			m_pBufferCom->Render_Buffer();
			break;

		}

		switch (m_tData.m_iStarNumber)
		{
		case 1:
			m_pTextureCom2->Set_Texture(0);
			m_pBufferCom->Render_Buffer();
			break;
		case 2:
			m_pTextureCom2->Set_Texture(1);
			m_pBufferCom->Render_Buffer();
			break;
		case 3:
			m_pTextureCom2->Set_Texture(2);
			m_pBufferCom->Render_Buffer();
			break;
		}


		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	}

}

HRESULT CUi_StageInfo::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;


	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StageInfo"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_CTexture", pComponent });

	pComponent = m_pTextureCom2 = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StageInfo2"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_CTexture2", pComponent });


	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_CBuffer", pComponent });

	return S_OK;
}

CUi_StageInfo* CUi_StageInfo::Make_StageInfo(int _Number, int _StarNumber, _vec3 _vPos)
{
	CUi_StageInfo* pGameObject = new CUi_StageInfo(m_pGraphicDev);
	pGameObject->Add_Component();
	pGameObject->m_tData.m_bIsMgr = false;
	pGameObject->m_tData.m_bProcess = true;
	pGameObject->m_tData.m_iNumber = _Number;
	pGameObject->m_tData.m_iStarNumber = _StarNumber;

	pGameObject->m_tData.m_vScale = { 1.0f, 1.0f, 1.0f }; 
	pGameObject->m_pTransformCom->Set_Scale(pGameObject->m_tData.m_vScale);
	pGameObject->m_pTransformCom->Set_Pos(_vPos.x, _vPos.y, _vPos.z);
	CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
	static _int iStageInfoCount = 0; 
	TCHAR		szFileName[128] = L""; 
	wsprintf(szFileName, L"Object_StageInfo%d", iStageInfoCount++); // 레이어 추가 및 이름 변경 
	 
	size_t len = wcslen(szFileName) + 1;
	wchar_t* pKey = new wchar_t[len];
	wcscpy_s(pKey, len, szFileName);

	if (FAILED(pLayer->Add_GameObject(pKey, pGameObject)))
		return nullptr;

	m_listData.push_back(pGameObject->m_tData);
	return pGameObject;
}

void CUi_StageInfo::Free()
{
}