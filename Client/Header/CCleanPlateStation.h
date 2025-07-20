/**
* @file		CCleanPlateStation.h
* @date		2025-07-02
* @author	권예지
* @brief	깨끗한 접시 제공 (아마도 싱크대에서 세척한)
* @details	(이하 내용은 수정 예정)
 *			- 플레이어가 접시(더러운 접시 등)를 가져갈 수 있는 공간을 제공하는 스테이션 클래스입니다.
 *			- 게임 시작 시 여러 개의 빈 접시가 이 스테이션에 준비되어 있을 수 있습니다. (게임 확인 필요)
 *			- 플레이어가 음식을 내놓으면, 게임의 평가(채점) 과정 후 사용된 더러운 접시가 이 스테이션 위에 올라옵니다.
 *			- 플레이어는 어떤 물건을 들고 있든지 간에, 이 스테이션에 물건을 직접 내려놓을 수 없습니다.
 *			- 따라서, IPlace 인터페이스의 Get_CanPlace() 함수에서 항상 false를 반환하여, 플레이어가 직접 올리는 동작을 차단합니다.
 */
#pragma once
#include "CInteract.h"
#include "IPlace.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CCleanPlateStation : public CInteract, public IPlace
{
protected:
	explicit CCleanPlateStation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCleanPlateStation(const CGameObject& rhs);
	virtual ~CCleanPlateStation();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	// CInteract을(를) 통해 상속됨
	INTERACTTYPE	Get_InteractType() const override { return CInteract::STATION; }

	// IPlace을(를) 통해 상속됨
	_bool			Set_Place(CGameObject* pItem, CGameObject* pPlace);
	_bool			Get_CanPlace(CGameObject* pItem) override;
	CGameObject*	Get_PlacedItem();

private:
	HRESULT			Add_Component();
	void			Update_PlatePosition();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	vector<Engine::CTexture*> m_vecTextureCom;

	vector<CGameObject*>	m_vecItem;

public:
	static CCleanPlateStation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void			Free(); 
};