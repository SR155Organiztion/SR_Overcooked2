#include "pch.h"
#include "CUi_Button.h"
#include "CUi_Icon.h"
#include "CUi_MainMenu.h"



CUi_Button::CUi_Button(LPDIRECT3DDEVICE9 pGraphicDev):CUi(pGraphicDev)
{
}

CUi_Button::CUi_Button(const CGameObject& rhs) : CUi(rhs)
{
}

CUi_Button::~CUi_Button()
{
}

void CUi_Button::Ready_GameObject(LPDIRECT3DDEVICE9 pGraphicDev, BUTTON_TYPE _cValue)
{
	

}

void CUi_Button::Update_GameObject()
{
}

void CUi_Button::LateUpdate_GameObject()
{
}

void CUi_Button::Render_GameObject()
{
}


void CUi_Button::Mouse_Pickig(int _m_iKeyPress)
{
}


void CUi_Button::Key_Input(int m_iMousPicking)
{
}

void CUi_Button::Free()
{
}