/**
* @file		CCone.h
* @date		2025-07-19
* @author	권예지
* @brief	인게임 데코 오브젝트 클래스
* @details	Stage 1, 3
*			스케일 : 1
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CCone : public Engine::CGameObject
{
private:
	explicit CCone(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCone(const CGameObject& rhs);
	virtual ~CCone();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	void		Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	/**
	* @brief	콘 텍스쳐 지정
	* @param	_iID - 0(왼쪽), 1(오른쪽), 2(오른쪽)
	*/
	void		Set_Texture(const _uint _iID);

private:
	HRESULT		Add_Component();
	void		BillBoard();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_uint		m_iTextureNum;

public:
	static		CCone*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};