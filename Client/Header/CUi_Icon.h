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
	float iconYOffset= 2.f;

public:
	CUi_Icon();
	CUi_Icon(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Icon(const CGameObject& rhs);
	~CUi_Icon();

public:

	/// - ������Ʈ, ũ�� ���� ��Ƶ� �Լ�
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
	/// ����� ��ġ�� �޾ƿͼ� ������ ��ġ�� �ݿ�
	int Update_GameObject(const _float& _fTimeDelta);
	void LateUpdate_GameObject();
	
	/// ������ ������ �̹��� �׸���
	void Render_GameObject();
	HRESULT Add_Component();

	CGameObject* Add_Icon(CIngredient::INGREDIENT_TYPE _m_eType);
	void UpdatePosition(CGameObject* _pGameObject, const _vec3& _vPos);
	void Set_Icon(CIngredient::INGREDIENT_TYPE _eType) { m_eType = _eType; }

private:
	void Free();

};

