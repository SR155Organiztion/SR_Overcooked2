/**
* @file		CBrickWall.h
* @date		2025-07-18
* @author	�ǿ���
* @brief	�ΰ��� �� ������Ʈ Ŭ����
* @details	Stage 1, 3
*			������ : x = 3  y = 2  z = 0.5 �Ǵ� 1
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
	 * @brief	���� ������ ���� �ؽ��� ����
	 * @param	BRICKTYPE : REDBROWN(�������� 1, 3) / PINKBROWN(�������� 2)
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