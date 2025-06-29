#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"
/**
* @mainpage
* - 소개: UI 부모 클래스
* @date 2025-06-29
* @section CREATINFO 작성정보
* - 작성자: 김서영
* - 작성일: 2025.06.29
* @section MODIFYINFO 수정 정보
* - 수정일/ 수정자: 수정내역
*/

class CUi :public CGameObject
{
	enum Ui_STATE { ACTIVATY, DEAD };

public:
	CUi(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi(const CGameObject& rhs);
	~CUi();

	void Render_GameObject();
};

