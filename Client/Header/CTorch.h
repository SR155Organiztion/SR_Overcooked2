/**
* @file		CTorch.h
* @date		2025-07-19
* @author	�ǿ���
* @brief	�ΰ��� ���� ������Ʈ Ŭ����
* @details	Stage 5, 6
*			������ : 1.5
*			(��ġ�� �� ����) �ǵ����̸� x ������ ȭ�� ����� �� �� (�����̳� ���������� ġ��ġ�� ���ٰ��� �̻�����)
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