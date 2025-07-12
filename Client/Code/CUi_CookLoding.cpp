#include "pch.h"
#include "CUi_CookLoding.h"

//Engine
#include "CManagement.h"
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

	//m_bProcess = true;
	Make_cookLoding(m_bProcess, 5.0f, D3DXVECTOR3(500, 500, 0));
	SetRect(&SrcRect, 0, 0, 300, 300);
	
	return S_OK;
}

_int CUi_CookLoding::Update_GameObject(const _float& _fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	m_dwTime += _fTimeDelta;

	if (m_bProcess)
	{
		DWORD dwCurTime = GetTickCount64();
		float elapsed = (float)(dwCurTime - m_tData.m_dwStartTime);
		float percent = (m_tData.m_dwLimitTime > 0) ? (elapsed / (float)m_tData.m_dwLimitTime) : 1.0f;
		if (percent >= 1.0f)
		{
			m_bProcess = false;
			m_tData.m_bVisible = false;
		}
	}

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	
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
	if (!m_bProcess) return;
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	DWORD dwCurTime = GetTickCount64();
	float elapsed = (float)(dwCurTime - m_tData.m_dwStartTime);
	float percent = (m_tData.m_dwLimitTime > 0) ? (elapsed / (float)m_tData.m_dwLimitTime) : 1.0f;
	if (percent < 0.f)
	{
		percent = 0.f;
	}
		m_pGauge = (int)(percent * 1920.0f);

	SetRect(&SrcRect, 0, 0, 1920, 524);
	SetRect(&SrcRect2, 0, 0, m_pGauge, 524);
	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Cook_Loding0.png");
	m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, &SrcRect2, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Cook_Loding1.png");
	
}

void CUi_CookLoding::Make_cookLoding( bool _m_bProcess, float _m_fProgress, _vec3 _m_vPos)
{

	CUi_CookLoding* pGameObject = new CUi_CookLoding(m_pGraphicDev); // 지금 만들어주는 이 게임오브젝트에 컴포넌트를 셋 해줘야한다.
	pGameObject->Add_Component();
	pGameObject->m_bProcess = true;
	UIDATA* pData = pGameObject->Get_UiData();

	m_bProcess = _m_bProcess; //사용 여부
	m_fProgress = _m_fProgress * 1000.f ; //사용 시간

	if (m_bProcess)
	{
		pData->m_dwLimitTime = (DWORD)m_fProgress;
		pData->m_dwStartTime = GetTickCount64();
		pData->m_fAnimDuration = m_fProgress;
		pData->m_fXScale = 0.025f;
		pData->m_fYScale = 0.015f;
		pData->m_fAnimTime = GetTickCount64();
		pData->m_vPos = _m_vPos;
		_vec3 Scale = { pData->m_fXScale, pData->m_fYScale, 1 };
		pGameObject->m_pTransformCom->Set_Scale(Scale);


		m_listData.push_back(m_tData);

		CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
		static _int iCookLodingCount = 0;
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, L"Object_CookLoding%d", iCookLodingCount++); // 아이콘 레이어 추가 및 이름 변경

		if (FAILED(pLayer->Add_GameObject(szFileName, pGameObject)))
			return;
	}

	if (!m_bProcess)
	{
		return;
	}

}

HRESULT CUi_CookLoding::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Cook"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });

	pComponent = m_pSpriteCom2 = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Cook"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite2", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}


void CUi_CookLoding::Free()
{
	
}
