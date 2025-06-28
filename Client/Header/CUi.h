#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"

class CUi :public CGameObject
{
	enum Ui_STATE { ACTIVATY, DEAD };

public:
	CUi(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi(const CGameObject& rhs);
	~CUi();

	void Render_GameObject();
};

