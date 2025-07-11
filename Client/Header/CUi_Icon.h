#pragma once
#include "CUi.h"
#include "CIngredient.h"

class CUi_Icon : public CUi
{
private:
	typedef struct IconData
	{
		CIngredient::INGREDIENT_TYPE m_eType;
		int m_iWidth;
		int m_iGap;
		_vec3 m_vScale {0.5f, 0.5f, 0.f }; /// 크기
		D3DXVECTOR3 m_vStartPos; ///시작 위치
		D3DXVECTOR3 m_vTargetPos; ///이동할 위치
		D3DXVECTOR3 m_vPos; ///현재 위치
		DWORD m_dwStartTime; ///시작 시간
		DWORD m_dwLimitTime; ///제한 시간
		DWORD m_dwTime; //남은 시간
		DWORD m_dwHideTime; //사라지는 시간
		bool m_bVisible; //보이는 기능 
		bool m_bAnimating; /// 애니메이션 중 여부
		float m_fAnimTime; ///현재 애니메이션 시간
		float m_fAnimDuration; /// 애니메이션 총 소요 시간(초)
	}ICON;

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CVIBuffer* m_pBufferCom;
	Engine::CSprite* m_pSpriteCom;

	CIngredient::INGREDIENT_TYPE m_eType;
	list<ICON> m_listIcon;
	ICON m_tData;

public:
	CUi_Icon();
	CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Icon(const CGameObject& rhs);
	~CUi_Icon();

public:
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	void Render_GameObject();
	HRESULT Add_Component();
	void Make_Icon(CIngredient::INGREDIENT_TYPE _m_eType, _vec3 _pos); ///아이콘 추가 시 사용
	void Delete_Icon(CIngredient::INGREDIENT_TYPE _m_eType); ///아이콘 삭제 시 사용

private:
	void Free();

};

