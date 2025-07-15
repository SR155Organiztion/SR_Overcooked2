#pragma once
#include "CUi.h"
#include "CIngredient.h"

class CUi_WarningBox :  public CUi
{
	private:
		Engine::CTransform* m_pTransformCom;
		Engine::CTexture* m_pTextureCom;
		Engine::CVIBuffer* m_pBufferCom;

		list<UIDATA> m_listData;
		UIDATA m_tData;
		float CookLodingYOffset = 1.f;

		int m_iFrameCount;
		int m_iGaugeFrame;
		float fElapsed = 0;
		bool m_bIsShow = false;

	public:
		CUi_WarningBox();
		CUi_WarningBox(LPDIRECT3DDEVICE9 pGraphicDev);
		CUi_WarningBox(const CGameObject& rhs);
		~CUi_WarningBox();

	public:
		HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 m_pGraphicDev);
		_int Update_GameObject(const _float& _fTimeDelta);
		void LateUpdate_GameObject();
		void Render_GameObject();
		HRESULT Add_Component();

		CGameObject* Make_WarningBox(bool _m_bVisible);
		void UpdatePosition(const _vec3& _vPos);


		void On_Off(bool _false)
		{
			m_tData.m_bEnd = _false;

		}

		void Free();

};

