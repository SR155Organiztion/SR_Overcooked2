#include "pch.h"
#include "CUi_CookLoding.h"

//Engine
#include "CManagement.h"
#include "CTexture.h"
#include "CLayer.h"

// 누나가 설계하신 구조대로 초안을 잡아뒀습니다.
// 구조는 아이콘과 같이 그대로 유지되고 있으니 
// 원치않은 동작이나 에러발생시 아이콘 클래스를 참고해서 수정하나가시면 됩니다.
// 그리고 게이지가 올라가는 테스트 코드는 RealPlayer 122 ~ 142번째 줄을 참고 해주시면 됩니다.
CUi_CookLoding::CUi_CookLoding(LPDIRECT3DDEVICE9 pGraphicDev): CUi_Gauge(pGraphicDev), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLoding::CUi_CookLoding(const CGameObject& rhs): CUi_Gauge(rhs), m_fProgress(0.f), m_bProcess(false)
{
}

CUi_CookLoding::~CUi_CookLoding()
{
	Free(); // 메모리해제는 정말 중요합니다.
			// 해당 객체에서 동적할당(Heap 영역에 메모리를 할당)을 했다면 반드시 해제를 해줘야 메모리 누수가
			// 생기지 않습니다.
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
	if (!m_bIsShow) // 보여주지 않으면 업데이트도 안할 겁니다. 안보이는 상황에서도 진행도는 계속 돌리고 싶다면
					// 이 분기문 제거해주시면 됩니다.
		return 0;

	// 서영이누나 한번은 꼭 꼼꼼히 봐주세요.
	// 1. 배경의 위치값 가져옵니다.
	CTransform* pOwnerTransform = dynamic_cast<CTransform*>(
		m_pOwnerBox->Get_Component(ID_DYNAMIC, L"Com_Transform")
		);

	_vec3 vBoxPos;
	pOwnerTransform->Get_Info(INFO_POS, &vBoxPos);

	_vec3 vBoxScale = pOwnerTransform->Get_Scale();

	// 2. 비율 계산 합니다.
	_float fPercent = m_fProgress / m_fMaxCookTime;
	fPercent = max(0.f, min(fPercent, 1.f)); // 특정 범위내로 숫자를 고정시키는 기법을 클램핑이라고 합니다.
											 // 누님이 작성하신
											 // if (fPercent >= 1.f) fPercent = 1.f;
											 // if (fPercent <= 0.f) fPercent = 0.f;
											 // 이 코드를 간소화 한겁니다.
											 // min 메크로는 두개의 매개변수중 작은 수를 반환해 줍니다.
											 // 따라서 fPercent가 1보다 작으면 fPercent가 반환되고
											 // 1보다 클 경우에는 1이 반환됩니다. if (fPercent >= 1.f) fPercent = 1.f; 이 로직에 해당됩니다.
											 // 그리고 반환된 값과 0.f와 비교해서 더 큰수를 반환 해줍니다.
											 // 만약 반환된 값이 0.f이거나 0.f보다 작으면 0을 반환 하는겁니다.
											 // 다른 경우로 반환된 값이 0.f보다 큰 경우에는 min에서 반환된 값을 그대로 주는겁니다.

	// 3. 게이지 스케일, 위치 설정 >> 왼쪽 기준으로 조정 합니다.
	// 스케일 적용이 제대로 안된 이유는 업데이트와 지금 이 함수에서 스케일 값들을 계속해서 호출하기 때문에 덮어써져서 안된겁니다.
	_vec3 vScale = { vBoxScale.x * fPercent, vBoxScale.y, vBoxScale.z }; //	박스의 스케일을 기준으로 하기 때문에
																		 // 박스의 스케일이 달라져도 바로 게이지도 자동으로 적용 됩니다.
																		 // 만약 박스 스케일이 2라면 2에 fPercent를 곱한 만큼 스케일을 조정해주니까
																		 // 자연스럽게 반응형으로 만들 수 있습니다.
	_vec3 vPos = vBoxPos;
	vPos.x -= (vBoxScale.x - vScale.x) * 0.5f; // 왼쪽 기준 보정 합니다.
											   // 보정을 하지 않으면 가운데에서 줄었다 늘었다 하기때문에 스케일을 지정한후
											   // 총 너비 - 게이지 너비 / 2를 한거 만큼 왼쪽으로 이동 시켜 주는겁니다.
											   // 제가 처음 만들었을때 빨간 게이지가 박스 밖으로 튀어나간 이유는
											   // 박스를 기준으로 오프셋하지 않았기 때문입니다.
	// 기준을 항상 박스를 기준으로 합니다.**

	m_pTransformCom2->Set_Scale(vScale);
	m_pTransformCom2->Set_Pos(vPos.x, vPos.y, vPos.z);

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
	if (m_tData.m_bIsMgr)
	{
		// 하나의 함수안에서 너무 많은 일을 하는 건 좋지 않습니다.
		// 이론적으로는 함수는 하나의 기능만을 담당해야합니다.
		// 물론 코드를 짜다보면 지켜지 힘들지만
		// 하나의 함수에서 너무 많은 기능을 담당하게 되면 이전 저희 상황처럼 덮어써지는 걸 제대로 찾지
		// 못한다거나 가독성이 너무 떨어진다거나 코드의 흐름을 놓치기 쉽습니다.

		if (!m_tData.m_bProcess)
			return;

		//_matrix matView;
		//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		//_matrix matBillboard;
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

		//_vec3 vPos;
		//m_pTransformCom2->Get_Info(INFO_POS, &vPos);
		//_matrix matTrans;
		//D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

		//_matrix matScale;
		//D3DXMatrixScaling(&matScale, m_tData.m_vScale.x, m_tData.m_vScale.y, m_tData.m_vScale.z);

		//_matrix matWorld = matScale * matBillboard * matTrans; // 월드 = 스케일 * 빌보드 * 드랜스
		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom2->Get_World());

		m_pBufferCom->Render_Buffer();
		m_pTextureCom2->Set_Texture(1);

	}
}

CGameObject* CUi_CookLoding::Make_cookLoding( bool _m_bProcess, CGameObject* _pLoadingBox, _float _m_fProgress)
{
	CUi_CookLoding* pGameObject = new CUi_CookLoding(m_pGraphicDev); // 지금 만들어주는 이 게임오브젝트에 컴포넌트를 셋 해줘야한다.
	pGameObject->m_pOwnerBox = dynamic_cast<CUi_CookLodingBox*>(_pLoadingBox);
	pGameObject->m_pOwnerBox->AddRef(); // 다른 오브젝트를 참조할때는 꼭!!!!!!해당 오브젝트의 참조값을 한개 늘려줍니다.

	pGameObject->Add_Component();
	UIDATA* pData = pGameObject->Get_UiData();
	pGameObject->m_bIsMgr = true;
	pGameObject->m_bProcess = _m_bProcess; //사용 여부
	// pGameObject->m_fProgress = _m_fProgress * 1000.f ; //사용 시간 >> 이미 넘어오는 값이 float형이고 우리는 최대
																		// 1혹은 2를 기준으로 하기 때문에 1000을 곱해줄 필요가 없습니다.
	pGameObject->m_fProgress = _m_fProgress;

	if (pGameObject->m_bProcess)
	{

		// 오브젝트 생성
		CLayer* pLayer = CManagement::GetInstance()->Get_Layer(L"UI_Layer"); //레이어 불러오기
		static _int iCookLodingCount = 0;
		TCHAR		szFileName[128] = L"";
		wsprintf(szFileName, L"Object_CookLoding%d", iCookLodingCount++); // 아이콘 레이어 추가 및 이름 변경

		if (FAILED(pLayer->Add_GameObject(szFileName, pGameObject)))
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

	// 변수명은 명확하게 하는게 좋습니다.
	// 일관된 변수 명에 숫자만 붙히면 나중에 누나도 코드를 다시 보실때 이게 뭐지? 라는 생각이 드실겁니다.
	// 변수명만 보고도 이게 어떤 역할을 하는 변수인지 구분이 가능 할 정도로 최대한 노력을 해주셔야합니다.
	// 변수명이나 함수명은 길어져도 상관없으니 최대한 명만 보고도 바로 알아차릴 수 있어야합니다.
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
	Safe_Release<CUi_CookLodingBox*>(m_pOwnerBox); // CBase를 기본으로 하기 때문에 참조값이 0이 되면 자동으로 
												   // 오브젝트를 지워줍니다. 따라서 Safe_Delete가 아닌
												   // 참조값을 하나 감소시켜주면서 0이면 자동으로 메모리를 해제하는
												   // Safe_Release를 호출해줍니다.
}
