/**
* @file    CIngredientStation.h
* @date    2025-06-29
* @author  �ǿ���
* @brief   ��� ���� ������Ʈ Ŭ����
* @details �÷��̾ ��Ḧ �������ų� ���� �� �ִ� ������ �����ϴ� �۾��� ������Ʈ.
*/
#pragma once
#include "CInteract.h"
#include "IPlace.h"
#include "CIngredient.h"

namespace Engine
{
	class CCubeTex;
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CIngredientStation : public CInteract, public IPlace
{
protected:
	explicit CIngredientStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CIngredientStation(const CGameObject& rhs);
	virtual ~CIngredientStation();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	// CInteract��(��) ���� ��ӵ�
	INTERACTTYPE			Get_InteractType() const override { return CInteract::STATION; }

	// IPlace��(��) ���� ��ӵ�
	_bool					Get_CanPlace(CGameObject* pItem) override;

public:
	// ��Ḧ ���� Ʈ���� �Լ�
	CGameObject*					TakeOut_Ingredient();
	const CIngredient::INGREDIENT_TYPE	Get_StationIngredientType() { return m_eTypeIngredient; }
	virtual _bool			On_Snap(CGameObject* _pGameObject) override;
	const _tchar*			Get_IngredientName() { return m_szIngredientName; }
private:
	HRESULT		Add_Component();
	void		Ready_Lid();
	void		Render_Lid();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

	Engine::CRcTex* m_pLidBufferCom;
	Engine::CTransform* m_pLidTransformCom;
	Engine::CTexture* m_pLidTextureCom;
	_int	m_iLidTexNum;									///< �Ѳ� �ؽ��� index
	_bool	m_bReadyLid = false;
	
	CIngredient::INGREDIENT_TYPE	m_eTypeIngredient;		///< station�� � Ÿ���� ��Ḧ �����ϴ��� ���п�
	const _tchar*					m_szIngredientName;			///< ObjectPoolMgr ȣ��� ���ڿ�
	

public:
	static CIngredientStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	// create�� ��������� ����
	void	Set_TypeIngredientStation(const _tchar* create_name);

private:
	virtual		void		Free(); 
};