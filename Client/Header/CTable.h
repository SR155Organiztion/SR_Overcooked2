/**
* @file		CTable.h
* @date		2025-07-20
* @author	권예지
* @brief	인게임 데코 오브젝트 클래스
* @details	Stage 1, 3
*			스케일 : x = 1 y = 0.5  z = 1
*			Set_Texture() 함수 호출해서 테이블 디자인 정해줘야
*/
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CTable : public CGameObject
{
public:
	/**
	 * @enum	TABLETYPE
	 * @brief	테이블 디자인 종류
	 */
	enum TABLETYPE
	{
		BLACK,
		WHITE,
		GREEN,
		CHECK
	};

protected:
	explicit CTable(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTable(const CGameObject& rhs);
	virtual ~CTable();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual		void		Render_GameObject();

	void		Set_Scale(const _float& fX, const _float& fY, const _float& fZ);

	/**
	 * @brief	테이블 디자인에 맞는 텍스쳐로 설정
	 * @param	TABLETYPE : BLACK, WHITE, GREEN, CHECK
	 */
	void		Set_Texture(TABLETYPE eType);

private:
	HRESULT		Add_Component();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	int			m_iFrame;

public:
	static		CTable*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();
};