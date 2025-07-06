#pragma once
#include "CUi.h"
#include "CUi_Factory.h"
#include "CGraphicDev.h"

class CUi_Button :   public CUi
{
	/**
	* @file CUi_Button
	* @date 2025.06.29
	* @author �輭��
	* @brief ��ư ������ �����ϴ� Ŭ����
	*/
	
private: 
	LPDIRECT3DTEXTURE9 m_pTexBtn; /// �⺻ �ϴû� ��ư
	LPDIRECT3DTEXTURE9 m_pTexLockImage;///�ڹ��� �׸�
	LPDIRECT3DTEXTURE9 m_pTexScroll; ///��ư ������ ������ ��ũ�� 
	LPDIRECT3DTEXTURE9 m_pTexScrollStart; // ������
	LPDIRECT3DTEXTURE9 m_pTexOkBox; //���� ��ư
	LPDIRECT3DTEXTURE9 m_pTexNoBox;  //���� ��ư
	LPD3DXSPRITE m_pSprite;
	static DWORD64 m_dwlastInputTime; //Ű�Է� ���
	float m_fscaleX;
	float m_fscaleY;
	static int m_iKeyPress;

public:
	CUi_Button(LPDIRECT3DDEVICE9 pGraphicDev);
	CUi_Button(const CGameObject& rhs);
	~CUi_Button();

public:
	/**
	* @brief BUTTON_TYPE�� ���� �̹��� ������ �ҷ����� �Լ��� �Ű������� ���� BUTTON_TYPE�� m_eType�� �����Ѵ�.
	*/
	HRESULT Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev, BUTTON_TYPE _cValue);
	/**
	* @brief m_etype�� ����� BUTTON_TYPE�� ���� �ٸ� ����� ����. (���콺 ��ŷ, Ű���� �Է� ��)
	*/
	_int Update_GameObject(const _float& fTimeDelta);
	void LateUpdate_GameObject(const _float& fTimeDelta);
	void Render_GameObject();

	/**
	* @brief Ű�Է� ��, �Է°��� ���� �̹��� ������ �� ��ȯ���� ������Ʈ �Ѵ�.
	*/
	void Select_Button(float _fTimeDelta);
	void KeyInput();




private:
	void Free();

};

