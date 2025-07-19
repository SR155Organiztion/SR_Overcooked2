/**
* @file		CTrafficLight.h
* @date		2025-07-19
* @author	�ǿ���
* @brief	�ΰ��� ���� ������Ʈ Ŭ����
* @details	Stage 4
*			������ : 1.5 ~ 2
*			Set_Texture() �Լ� ȣ���ؼ� ��ġ ��ġ�� �ٶ󺸴� ���⿡ �´� �ؽ��� ���������
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
	 * @brief	��ȣ�� ��ġ ��ġ�� �ٶ󺸴� ����
	 */
	enum TLPOSDIR 
	{ 
		L_FWD,		///< ���ʿ� ��ġ, ���� �ٶ�
		R_FWD,		///< �����ʿ� ��ġ, ���� �ٶ�
		R_LEFT		///< �����ʿ� ��ġ, ���� �ٶ�
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
	 * @brief	��ȣ�� ��ġ ��ġ�� �ٶ󺸴� ���⿡ ���� �ؽ��ĸ� ����
	 * @param	TRAFFICLIGHTDIR : LEFTFRONT(ȭ�� ���� ��ġ, ���� �ٶ�) / RIGHTFRONT(ȭ�� ������ ��ġ, ���� �ٶ�) / RIGHTSIDE(ȭ�� ������ ��ġ, ���ʹٶ�)
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