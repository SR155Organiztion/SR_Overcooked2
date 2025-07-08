#pragma once
#include "Engine_Define.h"

BEGIN(Engine)
class ENGINE_DLL InGameSystem
{
	DECLARE_SINGLETON(InGameSystem)

private:
	explicit InGameSystem();
	virtual ~InGameSystem();
};
END()

