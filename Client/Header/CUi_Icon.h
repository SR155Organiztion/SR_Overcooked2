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
		_vec3 m_vScale {0.5f, 0.5f, 0.f }; /// ũ��
		D3DXVECTOR3 m_vStartPos; ///���� ��ġ
		D3DXVECTOR3 m_vTargetPos; ///�̵��� ��ġ
		D3DXVECTOR3 m_vPos; ///���� ��ġ
		DWORD m_dwStartTime; ///���� �ð�
		DWORD m_dwLimitTime; ///���� �ð�
		DWORD m_dwTime; //���� �ð�
		DWORD m_dwHideTime; //������� �ð�
		bool m_bVisible; //���̴� ��� 
		bool m_bAnimating; /// �ִϸ��̼� �� ����
		float m_fAnimTime; ///���� �ִϸ��̼� �ð�
		float m_fAnimDuration; /// �ִϸ��̼� �� �ҿ� �ð�(��)
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
	void Make_Icon(CIngredient::INGREDIENT_TYPE _m_eType, _vec3 _pos); ///������ �߰� �� ���
	void Delete_Icon(CIngredient::INGREDIENT_TYPE _m_eType); ///������ ���� �� ���

private:
	void Free();

};

