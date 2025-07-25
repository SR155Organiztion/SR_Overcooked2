#pragma once
#include "CUi.h"
#include "CUi_Factory.h"
#include "CGraphicDev.h"

class CUi_Button :   public CUi
{
	/**
	* @file CUi_Button
	* @date 2025.06.29
	* @author 김서영
	* @brief 버튼 정보를 저장하는 클래스
	*/
	

	/**
	* - 메인 메뉴 사용법
	* 1. Get 함수로 m_iKeyPress의 값을 가져온다.
	* 2. 받아온 m_iKeyPress값을 KeyInput 함수의 매개변수로 넣는다.
	* 3. 이후 Select_Button 함수를 불러오고 사용하고 싶은 BUTTON_TYPE(enum)을 매개변수로 넣는다.
	*/


private: 

	Engine::CSprite* m_pSpriteCom; //Button

	LPDIRECT3DTEXTURE9 m_pTexBtn; /// 기본 하늘색 버튼
	LPDIRECT3DTEXTURE9 m_pTexLockImage;///자물쇠 그림
	LPDIRECT3DTEXTURE9 m_pTexScroll; ///버튼 누르면 나오는 스크롤 
	LPDIRECT3DTEXTURE9 m_pTexScrollStart; // 선택지
	LPDIRECT3DTEXTURE9 m_pTexOkBox; //수락 버튼
	LPDIRECT3DTEXTURE9 m_pTexNoBox;  //거절 버튼
	LPD3DXSPRITE m_pSprite;
	static DWORD64 m_dwlastInputTime; //키입력 계산
	float m_fscaleX;
	float m_fscaleY;
	static int m_iKeyPress;

	UIDATA m_tData;

public:
	CUi_Button(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Button(const CGameObject& rhs);
	~CUi_Button();

public:
	/**
	* @brief BUTTON_TYPE에 따라 이미지 정보를 불러오는 함수로 매개변수로 받은 BUTTON_TYPE을 m_eType에 저장한다.
	*/
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev, BUTTON_TYPE _cValue);
	
	/**
	* @brief m_etype에 저장된 BUTTON_TYPE에 따라 다른 기능이 동작. (마우스 피킹, 키보드 입력 등)
	*/
	_int Update_GameObject(const _float& fTimeDelta);
	void LateUpdate_GameObject(const _float& fTimeDelta);
	void Render_GameObject();

	/**
	* @brief 키입력 시, 입력값에 따라 이미지 정보나 씬 전환등을 업데이트 한다.
	*/
	void Select_Button(float _fTimeDelta, int _m_iKeyPress, BUTTON_TYPE _type);
	void KeyInput(int _m_iKeyPress);
	int Get_KeyPress() { return m_iKeyPress; }
	void Set_KeyPress(int _m_iKeyPress){ m_iKeyPress = _m_iKeyPress; }
	HRESULT Add_Component();


private:
	void Free();

};

