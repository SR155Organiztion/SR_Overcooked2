#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer :   public CComponent
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

protected:
	template<typename T>
	void Calc_Size(const T* _pVertex) {
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

		if (fabsf(fMaxZ - fMinZ) < 0.01f) {
			float centerZ = (fMinZ + fMaxZ) * 0.5f;
			fMinZ = centerZ - 0.05f;
			fMaxZ = centerZ + 0.05f;
		}

		m_fWidth = fMaxX - fMinX;
		m_fHeight = fMaxY - fMinY;
		m_fDepth = fMaxZ - fMinZ;

		m_vMinBox = { fMinX, fMinY, fMinZ };
		m_vMaxBox = { fMaxX, fMaxY, fMaxZ };
	}


protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong						m_dwVtxSize;
	_ulong						m_dwVtxCnt;
	_ulong						m_dwTriCnt;
	_ulong						m_dwFVF;

	_ulong						m_dwIdxSize;
	D3DFORMAT					m_IdxFmt;

	_float						m_fWidth;
	_float						m_fHeight;
	_float						m_fDepth;
	_vec3						m_vMinBox;
	_vec3						m_vMaxBox;

public:
	virtual void Free();
	_float Get_Width()		{ return m_fWidth; }
	_float Get_Height()		{ return m_fHeight; }
	_float Get_Depth()		{ return m_fDepth; }
	_ulong Get_VtxCnt()		{ return m_dwVtxCnt; }
	_ulong Get_VtxSize()	{ return m_dwVtxSize; }
	_vec3  Get_MinBox()		{ return m_vMinBox; }
	_vec3  Get_MaxBox()		{ return m_vMaxBox; }
};

END
