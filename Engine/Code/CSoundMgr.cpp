#include "Engine_Define.h"
#include "CSoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr() {}

CSoundMgr::~CSoundMgr() {
    Free();
}

void CSoundMgr::Init()
{
    System_Create(&m_pSystem);
    m_pSystem->init(512, FMOD_INIT_NORMAL, nullptr);

    //Load_Sound(BGM, "/bgm.mp3", true, BGM_CHANNEL);
    //Load_Sound(BGM2, "/rbgm.mp3", true, BGM2_CHANNEL);
}

void CSoundMgr::Update(const _float& fTimeDelta)
{
    m_pSystem->update();

    // 볼륨 페이드
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

                auto& chVec = m_mapChannels[it->channelId];
                chVec.erase(remove(chVec.begin(), chVec.end(), it->pChannel), chVec.end());
            }

            it = m_FadeList.erase(it);
        }
        else {
            ++it;
        }
    }

    // 재생 끝난 채널 제거
    for (auto& pair : m_mapChannels)
    {
        auto& chList = pair.second;
        chList.erase(remove_if(chList.begin(), chList.end(),
            [](Channel* ch) {
                bool playing = false;
                if (ch && ch->isPlaying(&playing) == FMOD_OK)
                    return !playing;
                return true;
            }),
            chList.end());
    }
}

void CSoundMgr::Load_Sound(const SOUND_ID key, const string& filepath, bool isLoop, SOUND_CHANNEL_ID channelId)
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

Channel* CSoundMgr::Play_Sound(
    const SOUND_ID soundId
    , const SOUND_CHANNEL_ID channelId
    , _bool _bPlayAlone
    , float _fFadeTime)
{
    auto soundIt = m_mapSound.find(soundId);
    if (soundIt == m_mapSound.end())
        return nullptr;

    auto& chVec = m_mapChannels[channelId];

    // 최대 채널 수 초과 시 가장 오래된 채널 제거
    if (chVec.size() >= MAX_CHANNELS_PER_GROUP)
    {
        Channel* oldest = chVec.front();
        if (oldest)
            oldest->stop();
        chVec.erase(chVec.begin());
    }

    if (_bPlayAlone && chVec.size() >= 1) {
        return chVec.front();
    }

    Channel* pChannel = nullptr;
    m_pSystem->playSound(soundIt->second, nullptr, false, &pChannel);

    if (pChannel)
    {
        /*if (channelId == BGM || channelId == BGM2)
            pChannel->setVolume(0.3f);*/

        chVec.push_back(pChannel);
    }

    return pChannel;
}

bool CSoundMgr::Stop_Sound(const SOUND_CHANNEL_ID key, _bool _bIsFade)
{
    auto it = m_mapChannels.find(key);
    if (it == m_mapChannels.end())
        return false;

    for (Channel* pChannel : it->second)
    {
        if (!pChannel)
            continue;

        float currentVolume = 1.f;
        pChannel->getVolume(&currentVolume);

        if (!_bIsFade) {
            pChannel->stop();
            return true;
        }

        VolumeFadeInfo fade;
        fade.pChannel = pChannel;
        fade.startVolume = currentVolume;
        fade.targetVolume = 0.f;
        fade.duration = 5.f;
        fade.elapsed = 0.f;
        fade.fadeOut = true;
        fade.channelId = key;

        bool alreadyFading = any_of(m_FadeList.begin(), m_FadeList.end(),
            [pChannel](const VolumeFadeInfo& f) {
                return f.pChannel == pChannel;
            });

        if (!alreadyFading)
            m_FadeList.push_back(fade);
    }

    return true;
}

bool CSoundMgr::Stop_Sound(const SOUND_CHANNEL_ID key, const Channel* _pChannel, _bool _bIsFade)
{
    auto it = m_mapChannels.find(key);
    if (it == m_mapChannels.end())
        return false;

    for (Channel* pChannel : it->second)
    {
        if (!pChannel)
            continue;

        if (pChannel != _pChannel)
            continue;

        float currentVolume = 1.f;
        pChannel->getVolume(&currentVolume);

        if (!_bIsFade) {
            pChannel->stop();
            return true;
        }

        VolumeFadeInfo fade;
        fade.pChannel = pChannel;
        fade.startVolume = currentVolume;
        fade.targetVolume = 0.f;
        fade.duration = 5.f;
        fade.elapsed = 0.f;
        fade.fadeOut = _bIsFade;
        fade.channelId = key;

        bool alreadyFading = any_of(m_FadeList.begin(), m_FadeList.end(),
            [pChannel](const VolumeFadeInfo& f) {
                return f.pChannel == pChannel;
            });

        if (!alreadyFading)
            m_FadeList.push_back(fade);
    }

    return true;
}

void CSoundMgr::Stop_All()
{
    for (auto& pair : m_mapChannels)
    {
        for (Channel* pChannel : pair.second)
        {
            if (!pChannel)
                continue;

            float currentVolume = 1.f;
            pChannel->getVolume(&currentVolume);

            pChannel->stop();
            return;

            /*VolumeFadeInfo fade;
            fade.pChannel = pChannel;
            fade.startVolume = currentVolume;
            fade.targetVolume = 0.f;
            fade.duration = 5.f;
            fade.elapsed = 0.f;
            fade.fadeOut = true;
            fade.channelId = pair.first;

            m_FadeList.push_back(fade);*/
        }
    }
}

_bool CSoundMgr::Get_IsPlaying(Channel* _pChannel)
{
    _bool bIsPlaying = FALSE;

    if (_pChannel == nullptr)
        return FALSE;

    FMOD_RESULT result = _pChannel->isPlaying(&bIsPlaying);
    if (result != FMOD_OK)
    {
        return FALSE;
    }

    return bIsPlaying;
}

void CSoundMgr::Free()
{
    for (auto& pair : m_mapSound)
    {
        if (pair.second)
        {
            pair.second->release();
            pair.second = nullptr;
        }
    }
    m_mapSound.clear();

    for (auto& pair : m_mapChannels)
    {
        for (Channel* pChannel : pair.second)
        {
            if (pChannel)
            {
                pChannel->stop();
            }
        }
    }
    m_mapChannels.clear();

    if (m_pSystem)
    {
        m_pSystem->close();
        m_pSystem->release();
        m_pSystem = nullptr;
    }

    m_FadeList.clear();
    m_StoppedChannels.clear();
}

