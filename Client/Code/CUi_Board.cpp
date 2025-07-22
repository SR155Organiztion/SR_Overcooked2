#include "pch.h"
#include "CUi_Board.h"

CUi_Board::CUi_Board() :CUi()
{
}

CUi_Board::CUi_Board(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev)
{
}

CUi_Board::CUi_Board(const CUi_Board& rhs): CUi(rhs)
{
}

CUi_Board::~CUi_Board()
{
}

HRESULT CUi_Board::Ready_GameObject(LPDIRECT3DDEVICE9 _m_pGraphicDev)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	
	m_tData.m_vPos = {50,50,0};
	m_tData.m_fXScale = 0.372f;
	m_tData.m_fYScale = 0.536f;

	return S_OK;
}
int CUi_Board::Update_GameObject(const _float& _fTimeDelta)
{
	_uint iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CUi_Board::LateUpdate_GameObject(const _float& _fTimeDelta)
{

}

void CUi_Board::Render_GameObject()
{
	
	
	if (!m_tData.m_bVisible)
		return;

	switch (m_tData.m_iNumber)
	{
		case 1:
		{
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/RecipeTutorial0.png");
			
		}
		break;
		case 2:
		{
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/RecipeTutorial1.png");
		}
		break;
		case 3:
		{
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/RecipeTutorial2.png");
		}
		break;
		case 4:
		{
			m_pSpriteCom->Render_Sprite(m_tData.m_fXScale, m_tData.m_fYScale, nullptr, m_pCenter, m_tData.m_vPos, L"../Bin/Resource/Texture/UI/in_game/RecipeTutorial3.png");
		}
		break;
	}

}

HRESULT CUi_Board::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pSpriteCom = dynamic_cast<Engine::CSprite*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Board"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Sprite", pComponent });


	return S_OK;
}


void CUi_Board::Make_Board(bool _bVisible,int _Number)
{
	m_tData.m_bVisible = _bVisible;
	m_tData.m_iNumber = _Number;
}

void CUi_Board::Free()
{

}