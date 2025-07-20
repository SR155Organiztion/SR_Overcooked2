#include "Engine_Define.h"
#include "CSoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
}

void CSoundMgr::Init()
{
    System_Create(&m_pSystem);
    m_pSystem->init(512, FMOD_INIT_NORMAL, nullptr);

    Load_Sound(BGM, "/bgm.mp3", true, BGM_CHANNEL);
    Load_Sound(BGM2, "/rbgm.mp3", true, BGM2_CHANNEL);
}

void CSoundMgr::Update(const _float& fTimeDelta)
{
    m_pSystem->update();

    for (auto it = m_FadeList.begin(); it != m_FadeList.end(); )
    {
        it->elapsed += fTimeDelta;
        float t = min(it->elapsed / it->duration, 1.f);
        float volume = it->startVolume + (it->targetVolume - it->startVolume) * t;

        if (it->pChannel)
            it->pChannel->setVolume(volume);

        if (t >= 1.f) {
            if (it->fadeOut && it->pChannel) {
                it->pChannel->stop();
                m_StoppedChannels.push_back(it->channelId);
            }

            it = m_FadeList.erase(it);
        }
        else {
            ++it;
        }
    }
}

void CSoundMgr::Load_Sound(const ASOUND_ID key, const string& filepath, bool isLoop, ASOUND_CHANNEL_ID channelId)
{
    string fullPath = SOUND_RESOURCE_PATH + filepath;

    FMOD_MODE mode = FMOD_DEFAULT;
    if (isLoop)
        mode |= FMOD_LOOP_NORMAL;
    else
        mode |= FMOD_LOOP_OFF;

    Sound* pSound = nullptr;
    m_pSystem->createSound(fullPath.c_str(), mode, nullptr, &pSound);
    m_mapSound[key] = pSound;
}

void CSoundMgr::Play_Sound(const ASOUND_ID soundId, const ASOUND_CHANNEL_ID channelId, float _fFadeTime)
{
    auto soundIt = m_mapSound.find(soundId);
    if (soundIt == m_mapSound.end())
        return;

    Channel* pCurrentChannel = nullptr;
    auto chIt = m_mapChannel.find(channelId);
    if (chIt != m_mapChannel.end())
        pCurrentChannel = chIt->second;

    if (pCurrentChannel)
    {
        bool isPlaying = false;
        Sound* currentSound = nullptr;

        pCurrentChannel->isPlaying(&isPlaying);
        pCurrentChannel->getCurrentSound(&currentSound);

        if (isPlaying && currentSound == soundIt->second)
        {
            return;
        }
        pCurrentChannel->stop();
    }

    Channel* pChannel = nullptr;
    m_pSystem->playSound(soundIt->second, nullptr, false, &pChannel);

    // BGM º¼·ý ³·Ãã
    if (channelId == BGM && pChannel)
    {
        pChannel->setVolume(0.3f);
    }
    else if (channelId == BGM2 && pChannel)
    {
        pChannel->setVolume(0.3f);
    }

    m_mapChannel[channelId] = pChannel;
}

bool CSoundMgr::Stop_Sound(ASOUND_CHANNEL_ID key)
{
    auto it = m_mapChannel.find(key);
    if (it != m_mapChannel.end() && it->second)
    {
        if (find(m_StoppedChannels.begin(), m_StoppedChannels.end(), key) != m_StoppedChannels.end())
        {
            m_StoppedChannels.remove(key);
            return true;
        }

        Channel* pChannel = it->second;

        float currentVolume = 1.f;
        pChannel->getVolume(&currentVolume);

        VolumeFadeInfo fade;
        fade.pChannel = pChannel;
        fade.startVolume = currentVolume;
        fade.targetVolume = 0.f;
        fade.duration = 5.f;
        fade.elapsed = 0.f;
        fade.fadeOut = true;
        fade.channelId = key;

        bool alreadyFading = any_of(m_FadeList.begin(), m_FadeList.end(),
            [pChannel](const VolumeFadeInfo& fade) {
                return fade.pChannel == pChannel;
            });

        if (!alreadyFading) {
            m_FadeList.push_back(fade);
        }
    }
    return false;
}



void CSoundMgr::Stop_All()
{
    for (auto& pair : m_mapChannel)
    {
        Channel* pChannel = pair.second;
        if (pChannel)
        {
            float currentVolume = 1.f;
            pChannel->getVolume(&currentVolume);

            VolumeFadeInfo fade;
            fade.pChannel = pChannel;
            fade.startVolume = currentVolume;
            fade.targetVolume = 0.f;
            fade.duration = 5.f;
            fade.elapsed = 0.f;
            fade.fadeOut = true;

            m_FadeList.push_back(fade);
        }
    }
}


