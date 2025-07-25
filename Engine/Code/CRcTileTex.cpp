#include "CRcTileTex.h"

CRcTileTex::CRcTileTex()
{
}

CRcTileTex::CRcTileTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRcTileTex::CRcTileTex(const CRcTileTex& rhs)
	: CVIBuffer(rhs)
{
}

CRcTileTex::~CRcTileTex()
{
}

HRESULT CRcTileTex::Ready_Buffer()
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;


	VTXTEX* pVertex = NULL;

	// 버텍스 버퍼에 보관된 정점 중 첫 번째 정점의 주소를 얻어오는 매개 변수
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -0.5f, 0.f, 0.5f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { 0.5f, 0.f, 0.5f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { 0.5f, 0.f, -0.5f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = { -0.5f, 0.f, -0.5f };
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	return S_OK;
}

void CRcTileTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CRcTileTex::Clone()
{
	return new CRcTileTex(*this);
}

CRcTileTex* CRcTileTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTileTex* pRcTileTex = new CRcTileTex(pGraphicDev);

	if (FAILED(pRcTileTex->Ready_Buffer()))
	{
		Safe_Release(pRcTileTex);
		MSG_BOX("pRcTileTex Create Failed");
		return nullptr;
	}

	return pRcTileTex;
}

void CRcTileTex::Free()
{
	CVIBuffer::Free();
}
