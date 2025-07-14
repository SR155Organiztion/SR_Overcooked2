#include "CHexPrismTex.h"

CHexPrismTex::CHexPrismTex()
{
}

CHexPrismTex::CHexPrismTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CHexPrismTex::CHexPrismTex(const CHexPrismTex& rhs)
	: CVIBuffer(rhs)
{
}

CHexPrismTex::~CHexPrismTex()
{
}

HRESULT CHexPrismTex::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwVtxCnt = 14;   // ���� 7�� + �Ʒ��� 7��
	m_dwTriCnt = 6 + 6 + (6 * 2);  // ���� + �Ʒ��� + ����
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	// ���� ����
	VTXTEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// �߽� ���� Y ������
	float topY = +0.25f;
	float botY = -0.25f;

	// ���� �߽�
	pVertex[0].vPosition = { 0.f, topY, 0.f };
	pVertex[0].vTexUV = { 0.25f, 0.25f };

	// ���� �ѷ�
	for (int i = 0; i < 6; ++i)
	{
		float angle = D3DXToRadian(60.f * i);
		float x = 0.5f * cosf(angle);
		float z = 0.5f * sinf(angle);

		pVertex[i + 1].vPosition = { x, topY, z };
	}

	// �Ʒ��� �߽�
	pVertex[7].vPosition = { 0.f, botY, 0.f };
	pVertex[7].vTexUV = { 0.5f, 0.5f };

	// �Ʒ��� �ѷ�
	for (int i = 0; i < 6; ++i)
	{
		float angle = +D3DXToRadian(60.f * i);
		float x = 0.5f * cosf(angle);
		float z = 0.5f * sinf(angle);

		pVertex[i + 8].vPosition = { x, botY, z };
	}


	pVertex[0].vTexUV = { 0.5f, 0.5f };

	for (int i = 0; i < 6; ++i)
	{
		float angle = D3DXToRadian(60.f * i);
		float u = 0.5f + 0.5f * cosf(angle);
		float v = 0.5f + 0.5f * sinf(angle);

		pVertex[i + 1].vTexUV = { u, v };
	}

	pVertex[7].vTexUV = { 0.5f, 0.5f };

	for (int i = 0; i < 6; ++i)
	{
		float angle = D3DXToRadian(60.f * i);
		float u = 0.5f + 0.5f * cosf(angle);
		float v = 0.5f + 0.5f * sinf(angle);

		pVertex[i + 8].vTexUV = { u, v };
	}

	m_pVB->Unlock();

	// �ε��� ����
	INDEX32* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	int idx = 0;

	// ���� (�ﰢ�� 6��)
	for (int i = 0; i < 6; ++i)
	{
		pIndex[idx]._0 = 0;
		pIndex[idx]._1 = (i + 1) % 6 + 1;
		pIndex[idx]._2 = i + 1;
		++idx;
	}

	// �Ʒ��� (�ﰢ�� 6��)
	for (int i = 0; i < 6; ++i)
	{
		pIndex[idx]._0 = 7;
		pIndex[idx]._1 = i + 8;
		pIndex[idx]._2 = (i + 1) % 6 + 8;
		++idx;
	}

	// ���� (�� ��� �ﰢ�� 2��)
	for (int i = 0; i < 6; ++i)
	{
		int top1 = i + 1;
		int top2 = (i + 1) % 6 + 1;
		int bot1 = i + 8;
		int bot2 = (i + 1) % 6 + 8;

		// �ﰢ�� 1 (CCW)
		pIndex[idx]._0 = top1;
		pIndex[idx]._1 = bot2;
		pIndex[idx]._2 = bot1;
		++idx;

		// �ﰢ�� 2 (CCW)
		pIndex[idx]._0 = top1;
		pIndex[idx]._1 = top2;
		pIndex[idx]._2 = bot2;
		++idx;
	}
	m_pIB->Unlock();

	return S_OK;
}

void CHexPrismTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CHexPrismTex::Clone()
{
	return new CHexPrismTex(*this);
}

CHexPrismTex* CHexPrismTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHexPrismTex* pHexTileTex = new CHexPrismTex(pGraphicDev);

	if (FAILED(pHexTileTex->Ready_Buffer()))
	{
		Safe_Release(pHexTileTex);
		MSG_BOX("pHexTileTex Create Failed");
		return nullptr;
	}

	return pHexTileTex;
}

void CHexPrismTex::Free()
{
	CVIBuffer::Free();
}
