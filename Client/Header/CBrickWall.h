/**
* @file		CBrickWall.h
* @date		2025-07-18
* @author	권예지
* @brief	인게임 벽 오브젝트 클래스
* @details	Stage 1, 3
*			스케일 : x = 3  y = 2  z = 0.5 또는 1
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CBrickWall : public CGameObject
{
public:
	enum BRICKTYPE
	{
		REDBROWN,
		PINKBROWN
	};

protected:
	explicit CBrickWall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBrickWall(const CGameObject& rhs);
	virtual ~CBrickWall();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	void		Set_Scale(const _float& fX, const _float& fY, const _float& fZ);

	/**
	 * @brief	돌벽 종류에 따라 텍스쳐 설정
	 * @param	BRICKTYPE : REDBROWN(스테이지 1, 3) / PINKBROWN(스테이지 2)
	 */
	void		Set_Texture(BRICKTYPE eType);

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	int			m_iFrame;

public:
	static	CBrickWall*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};