/**
* @file		CTrafficLight.h
* @date		2025-07-19
* @author	권예지
* @brief	인게임 데코 오브젝트 클래스
* @details	Stage 4
*			스케일 : 1.5 ~ 2
*			Set_Texture() 함수 호출해서 설치 위치와 바라보는 방향에 맞는 텍스쳐 설정해줘야
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CTrafficLight : public Engine::CGameObject
{
public:
	/**
	 * @enum	TRAFFICLIGHTDIR
	 * @brief	신호등 배치 위치와 바라보는 방향
	 */
	enum TLPOSDIR 
	{ 
		L_FWD,		///< 왼쪽에 배치, 정면 바라봄
		R_FWD,		///< 오른쪽에 배치, 정면 바라봄
		R_LEFT		///< 오른쪽에 배치, 왼쪽 바라봄
	};

private:
	explicit CTrafficLight(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrafficLight(const CGameObject& rhs);
	virtual ~CTrafficLight();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	void		Set_Scale(const _float& fX, const _float& fY, const _float& fZ);

	/**
	 * @brief	신호등 배치 위치와 바라보는 방향에 따라 텍스쳐를 설정
	 * @param	TRAFFICLIGHTDIR : LEFTFRONT(화면 왼쪽 배치, 정면 바라봄) / RIGHTFRONT(화면 오른쪽 배치, 정면 바라봄) / RIGHTSIDE(화면 오른쪽 배치, 왼쪽바라봄)
	 */
	void		Set_Texture(TLPOSDIR eDir);

private:
	HRESULT		Add_Component();
	void		BillBoard();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	int			m_iFrame;

public:
	static	CTrafficLight*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};