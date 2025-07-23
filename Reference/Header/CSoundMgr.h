#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include <fmod.hpp>

BEGIN(Engine)

#define SOUND_RESOURCE_PATH "../Bin/Resource/Sound"
#define MAX_CHANNELS_PER_GROUP 10

using namespace FMOD;

enum SOUND_ID {
    BGM,
    BGM2,
    LEVEL_READY1,
    LEVEL_READY2,
    LEVEL_READY3,
    LEVEL_READY4,

    INGAME_ITEM_PICKUP,
    INGAME_ITEM_PUTDOWN,
    INGAME_TRASHCAN,
    INGAME_HOTPOT_START,
    INGAME_HOTPOT_BUBBLE,
    INGAME_COOKING_COOKED,
    INGAME_COOKING_WARNING,
    INGAME_FIRE_IGNITE,
    INGAME_FIRE_LOOPSTART,
    INGAME_FIRE_LOOP,
    INGAME_EXTINGUISHER_LOOP,

};

enum SOUND_CHANNEL_ID {
    BGM_CHANNEL,
    BGM2_CHANNEL,
    LEVEL_READY_CHANNEL,

    INGAME_SFX
};

struct VolumeFadeInfo {
    Channel* pChannel = nullptr;
    float startVolume = 0.f;
    float targetVolume = 1.f;
    float duration = 1.f;
    float elapsed = 0.f;
    bool fadeOut = false;
    SOUND_CHANNEL_ID channelId;
};

class ENGINE_DLL CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

private:
	explicit CSoundMgr();
	virtual ~CSoundMgr();

private:
    System* m_pSystem;
    map<SOUND_ID, Sound*> m_mapSound;
    map<SOUND_CHANNEL_ID, vector<Channel*>> m_mapChannels;
    list<VolumeFadeInfo> m_FadeList;
    list<SOUND_CHANNEL_ID> m_StoppedChannels;

public:
    void Init();
    void Update(const _float& fTimeDelta);
    void Load_Sound(const SOUND_ID key, const string& filepath, bool isLoop = false, SOUND_CHANNEL_ID channelId = SOUND_CHANNEL_ID::BGM_CHANNEL);
    Channel* Play_Sound(
        SOUND_ID soundId
        , const SOUND_CHANNEL_ID channelId
        , _bool _bPlayAlone = FALSE
        , float _fFadeTime = 0.5f);
    bool Stop_Sound(const SOUND_CHANNEL_ID key);
    void Stop_All();
    _bool Get_IsPlaying(Channel* _pChannel);

    // CBase을(를) 통해 상속됨
    void Free() override;
};

END