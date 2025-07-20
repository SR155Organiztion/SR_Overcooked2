/**
* @file		CTorch.h
* @date		2025-07-19
* @author	권예지
* @brief	인게임 데코 오브젝트 클래스
* @details	Stage 5, 6
*			스케일 : 1.5
*			(배치할 때 참고) 되도록이면 x 축으로 화면 가운데에 둘 것 (왼쪽이나 오른쪽으로 치우치면 원근감이 이상해짐)
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
}

class CTorch : public Engine::CGameObject
{
private:
	explicit CTorch(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTorch(const CGameObject& rhs);
	virtual ~CTorch();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	void		Set_Scale(const _float& fX, const _float& fY, const _float& fZ);

private:
	HRESULT		Add_Component();
	void		BillBoard();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static		CTorch*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};