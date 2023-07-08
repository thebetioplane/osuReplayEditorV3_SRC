#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

namespace audioengine
{

class AudioEngine
{
   public:
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void toggle_pause() = 0;
    virtual bool is_playing() = 0;
    virtual bool is_paused() = 0;
    virtual bool is_stopped() = 0;
    virtual void jump_to(SongTime_t ms) = 0;
    virtual void rel_jump(SongTime_t ms) = 0;
    virtual void set_volume(float) = 0;
    virtual float get_volume() = 0;
    virtual void set_playback_speed(float) = 0;
    virtual float get_playback_speed() = 0;
    virtual SongTime_t get_time() = 0;
};

bool init();
void load_with_fallback(const std::wstring &fname, SongTime_t start_time, SongTime_t end_time);

extern AudioEngine *handle;

}  // namespace audioengine
