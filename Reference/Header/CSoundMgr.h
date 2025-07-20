#pragma once

#include <fmod.hpp>

#define SOUND_RESOURCE_PATH "../Bin/Resource/Sound"
using namespace FMOD;

enum ASOUND_ID {
    BGM,
    BGM2
};

enum ASOUND_CHANNEL_ID {
    BGM_CHANNEL,
    BGM2_CHANNEL
};

struct VolumeFadeInfo {
    Channel* pChannel = nullptr;
    float startVolume = 0.f;
    float targetVolume = 1.f;
    float duration = 1.f;
    float elapsed = 0.f;
    bool fadeOut = false;
    ASOUND_CHANNEL_ID channelId;
};

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

private:
	explicit CSoundMgr();
	virtual ~CSoundMgr();

private:
    System* m_pSystem;
    map<ASOUND_ID, Sound*> m_mapSound;
    map<ASOUND_CHANNEL_ID, Channel*> m_mapChannel;
    list<VolumeFadeInfo> m_FadeList;
    list<ASOUND_CHANNEL_ID> m_StoppedChannels;

public:
    void Init();
    void Update(const _float& fTimeDelta);
    void Load_Sound(const ASOUND_ID key, const string& filepath, bool isLoop = false, ASOUND_CHANNEL_ID channelId = ASOUND_CHANNEL_ID::BGM_CHANNEL);
    void Play_Sound(const ASOUND_ID soundId, const ASOUND_CHANNEL_ID channelId, float _fFadeTime = 0.5f);
    bool Stop_Sound(const ASOUND_CHANNEL_ID key);
    void Stop_All();
};

