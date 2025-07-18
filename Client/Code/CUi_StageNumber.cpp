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

	_vec3 vPos{ 100,100,1 };  //★실험용
	Make_StageNumber(true, 3);  //★실험용
	UpdatePosition(vPos);  //★실험용

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

void CUi_StageNumber::Render_GameObject()
{
	if (m_tData.m_bIsMgr)
	{
		if (m_tData.m_bProcess)
		{

			const _matrix* matWorld = m_pTransformCom->Get_World();
			m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

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
			case 6:
				m_pTextureCom->Set_Texture(5);
				m_pBufferCom->Render_Buffer();
				break;
			}

			m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
		}
	}
}

CGameObject* CUi_StageNumber::Make_StageNumber(bool _m_bVisible, int _Number)
{
	CUi_StageNumber* pGameObject = new CUi_StageNumber(m_pGraphicDev);
	pGameObject->Add_Component();
	UIDATA* pData = pGameObject->Get_UiData();

	pGameObject->m_tData.m_bVisible = _m_bVisible;
	pGameObject->m_tData.m_bIsMgr = true;
	pGameObject->m_tData.m_bProcess = true;
	pGameObject->m_tData.m_iNumber = _Number;

	if (pGameObject->m_tData.m_bVisible)
	{
		pGameObject->m_tData.m_vScale = { 1.0f, 1.0f, 0.f };
		pGameObject->m_pTransformCom->Set_Scale(pData->m_vScale);

		CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
		static _int iStageNumberCount = 0;
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, L"Object_StageNumber%d", iStageNumberCount++); // 레이어 추가 및 이름 변경
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