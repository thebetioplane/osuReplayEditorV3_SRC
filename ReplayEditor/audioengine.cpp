// clang-format off
#include "stdafx.h"
// clang-format on
#include "audioengine.hpp"

#include <string.h>

#include <fstream>
#include <string>

#include "config.hpp"
#include "thirdparty/bass24/bass.h"
#include "thirdparty/bass24/bass_fx.h"

namespace audioengine
{
namespace
{

enum class StreamStatus { Stopped, Paused, Playing };

class BassAudioEngine : public AudioEngine
{
   public:
    bool load(const std::wstring &);
    void play() override;
    void pause() override;
    void stop() override;
    void toggle_pause() override;
    bool is_playing() override;
    bool is_paused() override;
    bool is_stopped() override;
    void jump_to(double ms) override;
    void rel_jump(double ms) override;
    void set_volume(float) override;
    float get_volume() override;
    void set_playback_speed(float) override;
    float get_playback_speed() override;
    double get_time() override;

   private:
    HSTREAM stream = 0;
    HSTREAM stream_without_fx = 0;
    StreamStatus status = StreamStatus::Stopped;
    float vol = 0.0f;
    float speed = 1.0f;
};

// Fake audio engine is used when the beatmap cannot be loaded. It doesn't support playing or pausing, but stores a
// current time, volume and speed.
class FakeAudioEngine : public AudioEngine
{
   public:
    bool load(SongTime_t start_time, SongTime_t end_time);
    void play() override;
    void pause() override;
    void stop() override;
    void toggle_pause() override;
    bool is_playing() override;
    bool is_paused() override;
    bool is_stopped() override;
    void jump_to(double ms) override;
    void rel_jump(double ms) override;
    void set_volume(float) override;
    float get_volume() override;
    void set_playback_speed(float) override;
    float get_playback_speed() override;
    double get_time() override;

   private:
    SongTime_t m_current_time = 0;
    SongTime_t m_start_time = 0;
    SongTime_t m_end_time = 1;
    const StreamStatus m_status = StreamStatus::Stopped;
    float m_volume = 0.f;
    float m_speed = 1.f;
};

BassAudioEngine local_bass_audio_engine;
FakeAudioEngine local_fake_audio_engine;

bool file_exists(const std::wstring &fname)
{
    std::ifstream f(fname);
    return f.good();
}

}  // namespace

AudioEngine *handle = &local_bass_audio_engine;

bool init()
{
    if (!BASS_Init(-1, 44100, BASS_DEVICE_STEREO, nullptr, nullptr)) {
        fatal("audio engine could not start");
        return false;
    }
    return true;
}

void load_with_fallback(const std::wstring &fname, SongTime_t start_time, SongTime_t end_time)
{
    local_bass_audio_engine.stop();
    if (!fname.empty() && local_bass_audio_engine.load(fname)) {
        handle = &local_bass_audio_engine;
    } else {
        local_fake_audio_engine.load(start_time, end_time);
        handle = &local_fake_audio_engine;
    }
}

bool BassAudioEngine::load(const std::wstring &fname)
{
    if (stream) stop();
    std::wstring full_path = config::song_path + fname;
    if (!file_exists(full_path)) {
        not_fatal("this audio file could not be found, perhaps you configured your paths incorrectly?");
        return false;
    }
    stream =
        BASS_StreamCreateFile(false, full_path.c_str(), 0, 0, BASS_STREAM_DECODE | BASS_UNICODE | BASS_STREAM_PRESCAN);
    if (stream == 0) {
        not_fatal("audio stream could not be made (BASS_StreamCreateFile)");
        return false;
    }
    // need to keep the original stream handle so we can free it later
    stream_without_fx = stream;
    stream = BASS_FX_TempoCreate(stream, BASS_FX_TEMPO_ALGO_LINEAR);
    if (stream == 0) {
        not_fatal("audio stream could not be made (BASS_FX_TempoCreate)");
        return false;
    }
    constexpr float BASS_TRUE = 1.f;
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO_OPTION_USE_QUICKALGO, BASS_TRUE);
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO_OPTION_OVERLAP_MS, 4.f);
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO_OPTION_SEQUENCE_MS, 30.f);
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, vol);
    speed = 1.0f;
    return true;
}

void BassAudioEngine::play()
{
    if (stream) {
        status = StreamStatus::Playing;
        BASS_ChannelPlay(stream, false);
    }
}

void BassAudioEngine::pause()
{
    if (stream) {
        status = StreamStatus::Paused;
        BASS_ChannelPause(stream);
    }
}

void BassAudioEngine::toggle_pause()
{
    if (status == StreamStatus::Paused)
        play();
    else if (status == StreamStatus::Playing)
        pause();
}

void BassAudioEngine::stop()
{
    if (stream) {
        status = StreamStatus::Stopped;
        BASS_StreamFree(stream);
        BASS_StreamFree(stream_without_fx);
        stream = 0;
        stream_without_fx = 0;
    }
}

bool BassAudioEngine::is_playing()
{
    return status == StreamStatus::Playing;
}

bool BassAudioEngine::is_paused()
{
    return status == StreamStatus::Paused;
}

bool BassAudioEngine::is_stopped()
{
    return status == StreamStatus::Stopped;
}

void BassAudioEngine::jump_to(double ms)
{
    if (stream) {
        if (ms < 0) ms = 0;
        const QWORD byte_pos = BASS_ChannelSeconds2Bytes(stream, ms / 1000.0);
        BASS_ChannelSetPosition(stream, byte_pos, BASS_POS_BYTE);
    }
}

void BassAudioEngine::rel_jump(double ms)
{
    jump_to(get_time() + ms);
}


void BassAudioEngine::set_volume(float value)
{
    vol = value;
    if (stream) {
        BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, value);
    }
}

float BassAudioEngine::get_volume()
{
    return vol;
}

void BassAudioEngine::set_playback_speed(float value)
{
    if (stream) {
        speed = value;
        value = 100 * (value - 1);
        BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO, value);
    }
}

float BassAudioEngine::get_playback_speed()
{
    return speed;
}

double BassAudioEngine::get_time()
{
    if (stream) {
        const QWORD byte_pos = BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
        const double ms = BASS_ChannelBytes2Seconds(stream, byte_pos);
        return ms * 1000.0;
    } else {
        return 0;
    }
}

bool FakeAudioEngine::load(SongTime_t start_time, SongTime_t end_time)
{
    m_current_time = start_time;
    m_start_time = start_time;
    m_end_time = end_time;
    return true;
}

void FakeAudioEngine::play()
{
}

void FakeAudioEngine::pause()
{
}

void FakeAudioEngine::stop()
{
}

void FakeAudioEngine::toggle_pause()
{
}

bool FakeAudioEngine::is_playing()
{
    return m_status == StreamStatus::Playing;
}

bool FakeAudioEngine::is_paused()
{
    return m_status == StreamStatus::Paused;
}

bool FakeAudioEngine::is_stopped()
{
    return m_status == StreamStatus::Stopped;
}

void FakeAudioEngine::jump_to(double ms)
{
    if (ms < m_start_time) {
        m_current_time = m_start_time;
    } else if (ms > m_end_time) {
        m_current_time = m_end_time;
    } else {
        m_current_time = ms;
    }
}

void FakeAudioEngine::rel_jump(double ms)
{
    jump_to(get_time() + ms);
}

void FakeAudioEngine::set_volume(float value)
{
    m_volume = value;
}

float FakeAudioEngine::get_volume()
{
    return m_volume;
}

void FakeAudioEngine::set_playback_speed(float value)
{
    m_speed = value;
}

float FakeAudioEngine::get_playback_speed()
{
    return m_speed;
}

double FakeAudioEngine::get_time()
{
    return m_current_time;
}

}  // namespace audioengine
