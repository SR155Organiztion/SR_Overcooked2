#pragma once

#include "CBase.h"
#include "CComponent.h"

#include "CRcTex.h"
#include "CRcCol.h"
#include "CTriCol.h"
#include "CTerrainTex.h"
#include "CCubeTex.h"

#include "CTransform.h"
#include "CTexture.h"
#include "CCalculator.h"

BEGIN(Engine)

class ENGINE_DLL CProtoMgr :   public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit CProtoMgr();
	virtual ~CProtoMgr();

public:
	HRESULT			Ready_Prototype(const _tchar* pComponentTag, CComponent* pComponent);
	CComponent*		Clone_Prototype(const _tchar* pComponentTag);

private:
	CComponent* Find_Prototype(const _tchar* pComponentTag);

private:
	map<const _tchar*, CComponent*>		m_mapPrototype;		// ���� ������Ʈ

private:
	virtual void		Free();


};

END
