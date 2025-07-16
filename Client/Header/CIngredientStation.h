/**
* @file    CIngredientStation.h
* @date    2025-06-29
* @author  권예지
* @brief   재료 제공 오브젝트 클래스
* @details 플레이어가 재료를 내려놓거나 꺼낼 수 있는 공간을 제공하는 작업대 오브젝트.
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

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE			Get_InteractType() const override { return CInteract::STATION; }

	// IPlace을(를) 통해 상속됨
	_bool					Get_CanPlace(CGameObject* pItem) override;

public:
	// 재료를 꺼낼 트리거 함수
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
	_int	m_iLidTexNum;									///< 뚜껑 텍스쳐 index
	_bool	m_bReadyLid = false;
	
	CIngredient::INGREDIENT_TYPE	m_eTypeIngredient;		///< station이 어떤 타입의 재료를 관리하는지 구분용
	const _tchar*					m_szIngredientName;			///< ObjectPoolMgr 호출용 문자열
	

public:
	static CIngredientStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	// create후 어떤상자인지 세팅
	void	Set_TypeIngredientStation(const _tchar* create_name);

private:
	virtual		void		Free(); 
};