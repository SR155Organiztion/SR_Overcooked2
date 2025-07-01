#include "CVIBuffer.h"

CVIBuffer::CVIBuffer() : m_pVB(nullptr), m_pIB(nullptr),
m_dwVtxSize(0), m_dwTriCnt(0), m_dwVtxCnt(0), m_dwFVF(0), m_dwIdxSize(0)
, m_fWidth(0), m_fHeight(0), m_fDepth(0)
{
}

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev), m_pVB(nullptr), m_pIB(nullptr),
    m_dwVtxSize(0), m_dwTriCnt(0), m_dwVtxCnt(0), m_dwFVF(0), m_dwIdxSize(0)
    , m_fWidth(0), m_fHeight(0), m_fDepth(0)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
    :CComponent(rhs), m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB),
    m_dwVtxSize(rhs.m_dwVtxSize), m_dwTriCnt(rhs.m_dwTriCnt),
    m_dwVtxCnt(rhs.m_dwVtxCnt), m_dwFVF(rhs.m_dwFVF),
    m_dwIdxSize(rhs.m_dwIdxSize), m_IdxFmt(rhs.m_IdxFmt)
    , m_fWidth(rhs.m_fWidth), m_fHeight(rhs.m_fHeight), m_fDepth(rhs.m_fWidth)

{
    m_pVB->AddRef();
    m_pIB->AddRef();
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready_Buffer()
{
    if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, // 버텍스 버퍼 크기
        0,      // 버퍼 속성(0인 경우 정적 버퍼)
        m_dwFVF,    // 버텍스 속성
        D3DPOOL_MANAGED, // 정적 버퍼일 경우 MANAGED
        &m_pVB, // 생성한 버텍스 버퍼를 정보를 저장할 객체 주소
        NULL)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize, // 인덱스 버퍼 크기
        0,      // 버퍼 속성(0인 경우 정적 버퍼)
        m_IdxFmt,    // 인덱스 속성
        D3DPOOL_MANAGED, // 정적 버퍼일 경우 MANAGED
        &m_pIB, // 생성한 인덱스 버퍼를 정보를 저장할 객체 주소
        NULL)))
    {
        return E_FAIL;
    }


    return S_OK;
}

void CVIBuffer::Render_Buffer()
{
    m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

    m_pGraphicDev->SetFVF(m_dwFVF);

    //m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);

    m_pGraphicDev->SetIndices(m_pIB);
    
    m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CVIBuffer::Calc_Size(VTXCUBE* _pVertex)
{

    float fMinX = _pVertex[0].vPosition.x;
    float fMaxX = _pVertex[0].vPosition.x;
    float fMinY = _pVertex[0].vPosition.y;
    float fMaxY = _pVertex[0].vPosition.y;
    float fMinZ = _pVertex[0].vPosition.z;
    float fMaxZ = _pVertex[0].vPosition.z;

    for (int i = 1; i < m_dwVtxCnt; ++i) {
        const auto& p = _pVertex[i].vPosition;

        if (p.x < fMinX) fMinX = p.x;
        if (p.x > fMaxX) fMaxX = p.x;

        if (p.y < fMinY) fMinY = p.y;
        if (p.y > fMaxY) fMaxY = p.y;

        if (p.z < fMinZ) fMinZ = p.z;
        if (p.z > fMaxZ) fMaxZ = p.z;
    }

    m_fWidth = fMaxX - fMinX;
    m_fHeight = fMaxY - fMinY;
    m_fDepth = fMaxZ - fMinZ;

    m_vMinBox = { fMinX, fMinY, fMinZ };
    m_vMaxBox = { fMaxX, fMaxY, fMaxZ };

}


void CVIBuffer::Free()
{
    Safe_Release(m_pVB);
    Safe_Release(m_pIB);

    CComponent::Free();
}
