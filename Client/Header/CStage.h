#pragma once

#include "CScene.h"
#include "CMapTool.h"
#include "CTransform.h"
#include <CSoundMgr.h>
#include "CUi_Board.h"

class CStage : public Engine::CScene
{
private:
	enum INGAME_SHOW_UI {
		GAME_READY,
		GAME_START,
		GAME_PLAY,
		GAME_END,
		UI_PHASE_MAX
	};

private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev, string _szCurrStage);
	virtual ~CStage();

private:
	string		m_szCurrStage	= "";
	_bool		m_bIsEnter		= TRUE;
	_bool		m_InGameUIVec[GAME_END];
	_bool		m_bDoPattern	= FALSE;
	_bool		m_bIsBoardOpen	= TRUE;
	CUi_Board*	m_pBoardUI		= nullptr;

	INGAME_SHOW_UI m_eCurrUI = GAME_READY;

	_float		m_fEnterStopTimeElapsed = 0.f;
	_float		m_fEnterStopLogoInterval = 1.f;

	_float		m_fEndGameUITimeElapsed = 0.f;
	_float		m_fEndGameUITimeInterval = 2.f;

	_float		m_fPatternTimeElapsed = 0.f;
	_float		m_fPatternTime = 5.f;

	D3DVIEWPORT9		m_matStoreViewPort;
	_matrix				m_matStoreProjection;
	_matrix				m_matStoreView;

	Channel* m_pBGMChannel = nullptr;
	Channel* m_pReadyChannel = nullptr;
	Channel* m_pStartChannel = nullptr;
	Channel* m_pTimeUpChannel = nullptr;

	_int m_iPlayReadyCnt = 0;
	_int m_iPlayStartCnt = 0;
	_int m_iResultStartCnt = 0;

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_GameObject_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Ingredient();
	_int		Get_NumberEndOfString(string _szKey)
	{
		_tchar chNumber = _szKey.at(_szKey.size() - 1);
		_int iNumber = chNumber - '0';

		return iNumber;
	}

private:
	HRESULT		Ready_Prototype();
	HRESULT		Ready_Light();

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CStage* Create(LPDIRECT3DDEVICE9 pGraphicDev, string _szStageKey);
private:
	virtual void	Free();


};

