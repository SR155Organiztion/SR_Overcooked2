#pragma once
#include "CUi.h"
#include "CIngredient.h"

class CUi_Icon : public CUi
{

private:
	Engine::CSprite* m_pSpriteCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CVIBuffer* m_pBufferCom;
	

	CIngredient::INGREDIENT_TYPE m_eType;
	list<ICON> m_listIcon;
	ICON m_tData;

public:
	CUi_Icon();
	CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Icon(const CGameObject& rhs);
	~CUi_Icon();

public:

	/// - 컴포넌트, 크기 등을 모아둔 함수
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	/// 대상의 위치를 받아와서 아이콘 위치에 반영
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	
	/// 아이콘 종류별 이미지 그리기
	void Render_GameObject();
	HRESULT Add_Component();

	/// - 외부에서 사용할 떄 호출하는 함수 (Make 추가 / Delete 삭제) 
	void Make_Icon(CIngredient::INGREDIENT_TYPE _m_eType, _vec3 _pos); ///아이콘 추가 시 사용
	void Delete_Icon(CIngredient::INGREDIENT_TYPE _m_eType); ///아이콘 삭제 시 사용

	CGameObject* Add_Icon(CIngredient::INGREDIENT_TYPE _m_eType);
	void UpdatePosition(CGameObject* _pGameObject, const _vec3& _vPos);
	void Set_Icon(CIngredient::INGREDIENT_TYPE _eType) { m_eType = _eType; }

private:
	void Free();

};

