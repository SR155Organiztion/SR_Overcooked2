#pragma once
#include "CBase.h"
#include "Engine_Define.h"
class CInteract;

BEGIN(Engine)

class ENGINE_DLL CInteractMgr : public CBase
{
	DECLARE_SINGLETON(CInteractMgr)

public:
	enum INTERACT { CARRY, STATION, INTEND };

private:
	explicit CInteractMgr();
	virtual ~CInteractMgr();

public:
	list<CInteract*>*	Get_List(INTERACT eType) const;
	void				Clear_List();
	void				Add_List(INTERACT eType, CInteract* pInteract);

private:
	list<CInteract*>	m_list[INTEND];

public:
	virtual void	Free(void);
};

END