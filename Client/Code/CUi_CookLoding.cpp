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
}


HRESULT CUi_CookLoding::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tData.m_vPos = D3DXVECTOR3(100, 100, 0);
	SetRect(m_tData.SrcRect, 0, 0, 300, 300);
	m_tData.m_fXScale = 0.1f;
	m_tData.m_fYScale = 0.1f;

	Make_cookLoding(1.f, 1.f, m_tData.m_vPos);
	
	return S_OK;
}

_int CUi_CookLoding::Update_GameObject(const _float& _fTimeDelta)
{
	m_dwTime += _fTimeDelta;

	m_iFrameCount++;
	if (m_iFrameCount % 100 == 0)
	{
		m_iGaugeFrame++;
		if (m_iGaugeFrame >= 100)
		{
			// 끝까지 갔으면 멈추기
		}
	}
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	
	return iExit;
}

void CUi_CookLoding::LateUpdate_GameObject(const _float& _fTimeDelta)
{
}

void CUi_CookLoding::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_dwTime = GetTickCount64();
	float remaining = (m_dwLimitTime > (m_dwTime - m_dwStartTime)) ? (m_dwLimitTime - (m_dwTime - m_dwStartTime)) : 0;
	m_iminute = (int)(remaining / 1000) / 60;
	m_iseconds = (int)(remaining / 1000) % 60;

	//이미지
	float percent = (float)remaining / (float)m_dwLimitTime;
	if (percent < 0) percent = 0;
	m_pGauge = (int)(percent * 300.0f);
	SetRect(m_tData.SrcRect, 0, 0, 300, 120);
	SetRect(m_tData.SrcRect2, 0, 0, m_pGauge, 120);
	m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, m_tData.SrcRect, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Cook_Loding0.png");
	m_pSpriteCom2->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, m_tData.SrcRect2, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/Cook_Loding1.png");

	if (m_iminute <= 0 && m_iseconds <= 15)
	{
		//종료
	}
}

void CUi_CookLoding::Make_cookLoding(bool _m_bProcess, _float _m_fProgress, _vec3 _m_vPos)
{
	m_bProcess = _m_bProcess; //사용 여부
	m_fProgress = _m_fProgress; //사용 시간

	if (m_bProcess)
	{
		m_tData.m_iWidth = 270; 
		m_tData.m_iGap = 10; 
		m_tData.m_fXScale = 0.003f;
		m_tData.m_fYScale = 0.003f;
		m_tData.m_vPos = _m_vPos;
		m_tData.m_dwStartTime = GetTickCount64();
		m_tData.m_dwLimitTime = _m_fProgress;
		m_tData.m_bVisible = true;
		m_tData.m_bAnimating = true;
		m_tData.m_fAnimTime = GetTickCount64();
		m_tData.m_fAnimDuration=_m_fProgress;
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
