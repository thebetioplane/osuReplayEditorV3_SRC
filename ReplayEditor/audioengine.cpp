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

enum class StreamStatus { Stopped, Paused, Playing };

static HSTREAM stream = 0;
static HSTREAM stream_without_fx = 0;
static StreamStatus status = StreamStatus::Stopped;

bool audioengine::init()
{
    if (!BASS_Init(-1, 44100, BASS_DEVICE_STEREO, nullptr, nullptr)) {
        fatal("audio engine could not start");
        return false;
    }
    return true;
}

static bool file_exists(const std::wstring &fname)
{
    std::ifstream f(fname);
    return f.good();
}

static float vol = 0.0f;
static float speed = 1.0f;

bool audioengine::load(const std::wstring &fname)
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

void audioengine::play()
{
    if (stream) {
        status = StreamStatus::Playing;
        BASS_ChannelPlay(stream, false);
    }
}

void audioengine::pause()
{
    if (stream) {
        status = StreamStatus::Paused;
        BASS_ChannelPause(stream);
    }
}

void audioengine::toggle_pause()
{
    if (status == StreamStatus::Paused)
        play();
    else if (status == StreamStatus::Playing)
        pause();
}

void audioengine::stop()
{
    if (stream) {
        status = StreamStatus::Stopped;
        BASS_StreamFree(stream);
        BASS_StreamFree(stream_without_fx);
        stream = 0;
        stream_without_fx = 0;
    }
}

bool audioengine::is_playing()
{
    return status == StreamStatus::Playing;
}

bool audioengine::is_paused()
{
    return status == StreamStatus::Paused;
}

bool audioengine::is_stopped()
{
    return status == StreamStatus::Stopped;
}

void audioengine::jump_to(SongTime_t ms)
{
    if (stream) {
        if (ms < 0) ms = 0;
        const QWORD byte_pos = BASS_ChannelSeconds2Bytes(stream, static_cast<double>(ms / 1000.0));
        BASS_ChannelSetPosition(stream, byte_pos, BASS_POS_BYTE);
    }
}

void audioengine::rel_jump(SongTime_t ms)
{
    jump_to(get_time() + ms);
}

void audioengine::set_volume(float value)
{
    vol = value;
    if (stream) {
        BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, value);
    }
}

float audioengine::get_volume()
{
    return vol;
}

void audioengine::set_playback_speed(float value)
{
    if (stream) {
        speed = value;
        value = 100 * (value - 1);
        BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO, value);
    }
}

float audioengine::get_playback_speed()
{
    return speed;
}

SongTime_t audioengine::get_time()
{
    if (stream) {
        const QWORD byte_pos = BASS_ChannelGetPosition(stream, BASS_POS_BYTE);
        const double ms = BASS_ChannelBytes2Seconds(stream, byte_pos);
        return static_cast<SongTime_t>(ms * 1000. + 0.5);
    } else {
        return 0;
    }
}
