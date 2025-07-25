#include "pch.h"
#include "CUi_CookLoding.h"

//Engine
#include "CManagement.h"
#include "CTexture.h"
#include "CLayer.h"

// ������ �����Ͻ� ������� �ʾ��� ��Ƶ׽��ϴ�.
// ������ �����ܰ� ���� �״�� �����ǰ� ������ 
// ��ġ���� �����̳� �����߻��� ������ Ŭ������ �����ؼ� �����ϳ����ø� �˴ϴ�.
// �׸��� �������� �ö󰡴� �׽�Ʈ �ڵ�� RealPlayer 122 ~ 142��° ���� ���� ���ֽø� �˴ϴ�.
CUi_CookLoding::CUi_CookLoding(LPDIRECT3DDEVICE9 pGraphicDev): CUi_Gauge(pGraphicDev), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLoding::CUi_CookLoding(const CGameObject& rhs): CUi_Gauge(rhs), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLoding::~CUi_CookLoding()
{
	Free(); // �޸������� ���� �߿��մϴ�.
			// �ش� ��ü���� �����Ҵ�(Heap ������ �޸𸮸� �Ҵ�)�� �ߴٸ� �ݵ�� ������ ����� �޸� ������
			// ������ �ʽ��ϴ�.
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
	if (!m_pOwnerBox) 
		return 0;
	if (!m_bIsShow) // �������� ������ ������Ʈ�� ���� �̴ϴ�. �Ⱥ��̴� ��Ȳ������ ���൵�� ��� ������ �ʹٸ�
					// �� �б⹮ �������ֽø� �˴ϴ�.
		return 0;

	// �����̴��� �ѹ��� �� �Ĳ��� ���ּ���.
	// 1. ����� ��ġ�� �����ɴϴ�.
	CTransform* pOwnerTransform = dynamic_cast<CTransform*>(
		m_pOwnerBox->Get_Component(ID_DYNAMIC, L"Com_Transform")
		);

	_vec3 vBoxPos;
	pOwnerTransform->Get_Info(INFO_POS, &vBoxPos);

	_vec3 vBoxScale = pOwnerTransform->Get_Scale();

	// 2. ���� ��� �մϴ�.
	_float fPercent = m_fProgress / m_fMaxCookTime;
	fPercent = max(0.f, min(fPercent, 1.f)); // Ư�� �������� ���ڸ� ������Ű�� ����� Ŭ�����̶�� �մϴ�.
											 // ������ �ۼ��Ͻ�
											 // if (fPercent >= 1.f) fPercent = 1.f;
											 // if (fPercent <= 0.f) fPercent = 0.f;
											 // �� �ڵ带 ����ȭ �Ѱ̴ϴ�.
											 // min ��ũ�δ� �ΰ��� �Ű������� ���� ���� ��ȯ�� �ݴϴ�.
											 // ���� fPercent�� 1���� ������ fPercent�� ��ȯ�ǰ�
											 // 1���� Ŭ ��쿡�� 1�� ��ȯ�˴ϴ�. if (fPercent >= 1.f) fPercent = 1.f; �� ������ �ش�˴ϴ�.
											 // �׸��� ��ȯ�� ���� 0.f�� ���ؼ� �� ū���� ��ȯ ���ݴϴ�.
											 // ���� ��ȯ�� ���� 0.f�̰ų� 0.f���� ������ 0�� ��ȯ �ϴ°̴ϴ�.
											 // �ٸ� ���� ��ȯ�� ���� 0.f���� ū ��쿡�� min���� ��ȯ�� ���� �״�� �ִ°̴ϴ�.

	// 3. ������ ������, ��ġ ���� >> ���� �������� ���� �մϴ�.
	// ������ ������ ����� �ȵ� ������ ������Ʈ�� ���� �� �Լ����� ������ ������ ����ؼ� ȣ���ϱ� ������ ��������� �ȵȰ̴ϴ�.
	_vec3 vScale = { vBoxScale.x * fPercent, vBoxScale.y, vBoxScale.z }; //	�ڽ��� �������� �������� �ϱ� ������
																		 // �ڽ��� �������� �޶����� �ٷ� �������� �ڵ����� ���� �˴ϴ�.
																		 // ���� �ڽ� �������� 2��� 2�� fPercent�� ���� ��ŭ �������� �������ִϱ�
																		 // �ڿ������� ���������� ���� �� �ֽ��ϴ�.
	_vec3 vPos = vBoxPos;
	vPos.x -= (vBoxScale.x - vScale.x) * 0.5f; // ���� ���� ���� �մϴ�.
											   // ������ ���� ������ ������� �پ��� �þ��� �ϱ⶧���� �������� ��������
											   // �� �ʺ� - ������ �ʺ� / 2�� �Ѱ� ��ŭ �������� �̵� ���� �ִ°̴ϴ�.
											   // ���� ó�� ��������� ���� �������� �ڽ� ������ Ƣ��� ������
											   // �ڽ��� �������� ���������� �ʾұ� �����Դϴ�.
	// ������ �׻� �ڽ��� �������� �մϴ�.**

	m_pTransformCom2->Set_Scale(vScale);
	m_pTransformCom2->Set_Pos(vPos.x, vPos.y, vPos.z-0.0001);

	int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	
	return iExit;
}

void CUi_CookLoding::LateUpdate_GameObject(const _float& _fTimeDelta)
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
	if (m_tData.m_bIconDown)
	{
		m_pTransformCom2->Set_Pos(m_tData.m_vPos.x, m_tData.m_vPos.y, m_tData.m_vPos.z);
	}
	if (!m_tData.m_bIconDown)
	{
		m_pTransformCom2->Set_Pos(m_tData.m_vPos.x, m_tData.m_vPos.y, m_tData.m_vPos.z);
	}
}

void CUi_CookLoding::Render_GameObject()
{
	/*if (!m_tData.m_bIsMgr)
	{*/
		if (m_tData.m_bProcess)
		{

			// �ϳ��� �Լ��ȿ��� �ʹ� ���� ���� �ϴ� �� ���� �ʽ��ϴ�.
			// �̷������δ� �Լ��� �ϳ��� ��ɸ��� ����ؾ��մϴ�.
			// ���� �ڵ带 ¥�ٺ��� ������ ��������
			// �ϳ��� �Լ����� �ʹ� ���� ����� ����ϰ� �Ǹ� ���� ���� ��Ȳó�� ��������� �� ����� ã��
			// ���Ѵٰų� �������� �ʹ� �������ٰų� �ڵ��� �帧�� ��ġ�� �����ϴ�.

			const _matrix* matWorld = m_pTransformCom2->Get_World();
			m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

			m_pTextureCom2->Set_Texture(1);
			m_pBufferCom->Render_Buffer();
		}
	/*}*/
}

CGameObject* CUi_CookLoding::Make_cookLoding( bool _m_bProcess, CGameObject* _pLoadingBox, _float _m_fProgress)
{
	CUi_CookLoding* pGameObject = new CUi_CookLoding(m_pGraphicDev); // ���� ������ִ� �� ���ӿ�����Ʈ�� ������Ʈ�� �� ������Ѵ�.
	pGameObject->m_pOwnerBox = dynamic_cast<CUi_CookLodingBox*>(_pLoadingBox);
	pGameObject->m_pOwnerBox->AddRef(); // �ٸ� ������Ʈ�� �����Ҷ��� ��!!!!!!�ش� ������Ʈ�� �������� �Ѱ� �÷��ݴϴ�.

	pGameObject->Add_Component();
	UIDATA* pData = pGameObject->Get_UiData();
	pGameObject->m_bIsMgr = false;
	pGameObject->m_tData.m_bProcess = _m_bProcess; //��� ����
	pGameObject->m_bProcess = _m_bProcess;
	// pGameObject->m_fProgress = _m_fProgress * 1000.f ; //��� �ð� >> �̹� �Ѿ���� ���� float���̰� �츮�� �ִ�
																		// 1Ȥ�� 2�� �������� �ϱ� ������ 1000�� ������ �ʿ䰡 �����ϴ�.
	pGameObject->m_fProgress = _m_fProgress;

	if (pGameObject->m_bProcess)
	{

		// ������Ʈ ����
		CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //���̾� �ҷ�����
		static _int iCookLodingCount = 0;
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, L"Object_CookLoding%d", iCookLodingCount++); // ������ ���̾� �߰� �� �̸� ����

		size_t len = wcslen(szFileName) + 1;
		wchar_t* pKey = new wchar_t[len];
		wcscpy_s(pKey, len, szFileName);

		if (FAILED(pLayer->Add_GameObject(pKey, pGameObject)))
			return nullptr;

		m_listData.push_back(*pData);
		return pGameObject;
	}
	return nullptr;

}

void CUi_CookLoding::UpdatePosition(const _vec3& _vPos)
{
	m_pTransformCom2->Set_Pos(m_tData.m_vPos.x = _vPos.x, m_tData.m_vPos.y = _vPos.y + CookLodingYOffset, m_tData.m_vPos.z = _vPos.z);
}

void CUi_CookLoding::Delete_Gauge(CGameObject* _pGameObject)
{
	CManagement::GetInstance()->Delete_GameObject(L"UI_Layer", _pGameObject->Get_SelfId(), _pGameObject);
}

HRESULT CUi_CookLoding::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// �������� ��Ȯ�ϰ� �ϴ°� �����ϴ�.
	// �ϰ��� ���� �� ���ڸ� ������ ���߿� ������ �ڵ带 �ٽ� ���Ƕ� �̰� ����? ��� ������ ��ǰ̴ϴ�.
	// ������ ���� �̰� � ������ �ϴ� �������� ������ ���� �� ������ �ִ��� ����� ���ּž��մϴ�.
	// �������̳� �Լ����� ������� ��������� �ִ��� �� ���� �ٷ� �˾����� �� �־���մϴ�.
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
	Safe_Release<CUi_CookLodingBox*>(m_pOwnerBox); // CBase�� �⺻���� �ϱ� ������ �������� 0�� �Ǹ� �ڵ����� 
												   // ������Ʈ�� �����ݴϴ�. ���� Safe_Delete�� �ƴ�
												   // �������� �ϳ� ���ҽ����ָ鼭 0�̸� �ڵ����� �޸𸮸� �����ϴ�
												   // Safe_Release�� ȣ�����ݴϴ�.
}
