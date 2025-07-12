#include "pch.h"
#include "CUi_CookLoding.h"


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

	m_bProcess = true;
	Make_cookLoding(true, 5.0f, D3DXVECTOR3(500, 500, 0));
	SetRect(&SrcRect, 0, 0, 300, 300);
	
	return S_OK;
}

_int CUi_CookLoding::Update_GameObject(const _float& _fTimeDelta)
{
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

	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
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
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

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

void CUi_CookLoding::Make_cookLoding(bool _m_bProcess, float _m_fProgress, _vec3 _m_vPos)
{
	m_bProcess = _m_bProcess; //사용 여부
	m_fProgress = _m_fProgress * 1000.f ; //사용 시간

	if (m_bProcess)
	{
		m_tData.m_dwLimitTime = (DWORD)m_fProgress;
		m_tData.m_dwStartTime = GetTickCount64();    
		m_tData.m_fAnimDuration = m_fProgress;
		m_tData.m_fXScale = 0.025f;
		m_tData.m_fYScale = 0.015f;
		m_tData.m_fAnimTime = GetTickCount64();

		D3DXVECTOR3 worldPos = _m_vPos;
		D3DXVECTOR3 screenPos;
		D3DVIEWPORT9 viewport;
		D3DXMATRIX matWorld, matView, matProj;
		m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		m_pGraphicDev->GetViewport(&viewport);
		D3DXMatrixIdentity(&matWorld);
		D3DXVec3Project(
			&screenPos,
			&worldPos,
			&viewport,
			&matProj,   // 투영 행렬
			&matView,   // 뷰 행렬
			&matWorld   // 월드 행렬
		);
		m_tData.m_vPos = D3DXVECTOR3(screenPos.x, screenPos.y, 0);
		m_listData.push_back(m_tData);
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

	return S_OK;
}

HRESULT CUi_CookLoding::Delete_Component()
{
	return S_OK;
}

void CUi_CookLoding::Free()
{
	
}
