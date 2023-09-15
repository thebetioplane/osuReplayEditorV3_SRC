// clang-format off
#include "stdafx.h"
// clang-format on
#include <string_view>
#include <utility>
#include <vector>

#include "accuracy_analyzer.hpp"
#include "audioengine.hpp"
#include "beatmapengine.hpp"
#include "config.hpp"
#include "osudb.hpp"
#include "replayengine.hpp"
#include "texture.hpp"
#include "thirdparty/glm/glm.hpp"
#include "tools.hpp"
#include "ui.hpp"
#include "zoom_pan.hpp"

#ifndef BUILD_LABEL
#ifdef _DEBUG
#define BUILD_LABEL ("Debug " __DATE__ " " __TIME__)
#else
#error Non-debug build without a build label
#define BUILD_LABEL ("None")
#endif
#endif

namespace
{

class GraphicsHandle
{
   public:
    HWND hWnd;
    HDC hdc;
    HGLRC hrc;
};

GraphicsHandle graphics_handle;

void setup_graphics(HWND hWnd)
{
    graphics_handle.hWnd = hWnd;
    graphics_handle.hdc = GetDC(hWnd);
    if (graphics_handle.hdc == nullptr) {
        fatal("Get device context (GetDC) failed");
        return;
    }
    PIXELFORMATDESCRIPTOR pfd;
    std::memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int iFormat = ChoosePixelFormat(graphics_handle.hdc, &pfd);
    if (iFormat == 0) fatal("ChoosePixelFormat failed");
    if (!SetPixelFormat(graphics_handle.hdc, iFormat, &pfd)) fatal("SetPixelFormat failed");
    graphics_handle.hrc = wglCreateContext(graphics_handle.hdc);
    if (graphics_handle.hrc == nullptr) fatal("wglCreateContext failed");
    if (!wglMakeCurrent(graphics_handle.hdc, graphics_handle.hrc)) fatal("wglMakeCurrent failed");
}

// If out_str is nullptr, then assigns out_len to the length of the string. Otherwise this call is expecting out_len to
// contain the length of out_str. It will fill out_str up to the length of s or the given length, whichever is smaller.
// Afterwards, the value out_len is set to the the number of bytes written.
void return_a_string(BYTE *out_str, INT *out_len, std::string_view s)
{
    if (out_str == nullptr) {
        *out_len = static_cast<int>(s.size());
        return;
    }
    *out_len = std::min(*out_len, static_cast<int>(s.size()));
    for (int i = 0; i < *out_len; ++i) {
        out_str[i] = static_cast<BYTE>(s[i]);
    }
}

void return_a_string(BYTE *out_str, INT *out_len, std::wstring_view s)
{

    if (out_str == nullptr) {
        *out_len = static_cast<int>(s.size() * 2);
        return;
    }
    *out_len = std::min(*out_len, static_cast<int>(s.size() * 2));
    int wchar_count = *out_len / 2;
    for (int i = 0; i < wchar_count; ++i) {
        wchar_t curr_wchar = s[i];
        char* byte_array = (char*)&curr_wchar;  // transform wchar_t into byte array of two bytes
        out_str[2 * i] = static_cast<BYTE>(byte_array[0]);
        out_str[2 * i + 1] = static_cast<BYTE>(byte_array[1]);
    }
}

void set_a_string(const wchar_t *in_str, INT in_len, std::string &s)
{
    s.resize(in_len);
    for (int i = 0; i < in_len; ++i) {
        if (in_str[i] < 0x20 || in_str[i] >= 127)
            s[i] = '?';
        else
            s[i] = static_cast<char>(in_str[i]);
    }
}

void pixel_to_virtual(glm::vec2 &v)
{
    const float vpw = static_cast<float>(zoom_pan.playfield().w);
    const float vph = static_cast<float>(zoom_pan.playfield().h);
    // change to GL coordinates
    v.x = (v.x - zoom_pan.playfield().x) / vpw * 2.f - 1.f;
    v.y = (v.y - zoom_pan.playfield().y) / vph * 2.f - 1.f;
    v.y = -v.y;
    // inverse of the projection matrix (to osu coords)
    zoom_pan.gl_to_osu_pixel(v);
}

}  // namespace

DLLFUN(void) GetDllBuildLabel(BYTE *out_str, INT *len)
{
    return_a_string(out_str, len, BUILD_LABEL);
}

DLLFUN(INT) Init(HWND hWnd, const wchar_t *osu_db_path, const wchar_t *song_path)
{
    constexpr int SUCCESS = 0;
    constexpr int NULL_HWND = 1;
    constexpr int AUDIO_FAILURE = 2;
    constexpr int OSUDB_FAILURE = 3;
    constexpr int REPLAYENGINE_FAILURE = 4;
    constexpr int BEATMAPENGINE_FAILURE = 5;
    constexpr int TEXTURE_FAILURE = 6;
    if (hWnd == nullptr) return NULL_HWND;
    error_message_owner = hWnd;
    setup_graphics(hWnd);
    config::osu_db_path = osu_db_path;
    config::song_path = song_path;
    config::init();
    if (!audioengine::init()) return AUDIO_FAILURE;
    if (!osudb::init()) return OSUDB_FAILURE;
    if (!replayengine::init()) return REPLAYENGINE_FAILURE;
    if (!beatmapengine::init(L"")) return BEATMAPENGINE_FAILURE;
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glClearColor(.0f, .0f, .0f, 1.0f);
    zoom_pan.reset();
    zoom_pan.set_projection();
    if (!textures::init()) return TEXTURE_FAILURE;
    glColor3f(1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    return SUCCESS;
}

DLLFUN(BOOL) Cleanup()
{
    config::cleanup();
    return TRUE;
}

DLLFUN(void) GetMapPath(BYTE *out_str, INT *len)
{
    std::wstring result = config::song_path + beatmapengine::path;

    std::size_t pos = result.find(L"/");
    if (pos == std::string::npos) return;
    result.replace(pos, 1, L"\\");

    return_a_string(out_str, len, result);
}

DLLFUN(BOOL) LoadReplay(const wchar_t *fname)
{
    using ::replayengine::Replay;
    using ::replayengine::ReplayFrame;
    if (!replayengine::MutateCurrentView([&fname](const Replay &, Replay &next) { return next.read_from(fname); })) {
        return FALSE;
    }
    std::wstring osu_path;
    std::wstring song_path;
    if (!osudb::get_entry(replayengine::CurrentView()->metadata().beatmap_hash, osu_path, song_path)) {
        not_fatal("You do not have this beatmap. Information about the hit objects cannot be loaded.");
    }
    {
        beatmapengine::path = osu_path;
        const std::vector<ReplayFrame> &frames = replayengine::CurrentView()->frames();
        SongTime_t start = 0;
        SongTime_t end = 1;
        if (!frames.empty()) {
            start = frames.front().ms;
            end = frames.back().ms;
        }
        beatmapengine::first_hitobject_ms = start;
        beatmapengine::last_hitobject_ms = end;
        audioengine::load_with_fallback(song_path, start - 500, end + 500);
    }
    if (!beatmapengine::init(osu_path)) {
        return FALSE;
    }
    audioengine::handle->set_playback_speed(1.0f);
    return TRUE;
}

DLLFUN(BOOL) ChangeHashOfBeatmap(const wchar_t *fname)
{
    using ::replayengine::Replay;
    std::wstring filename(fname);
    filename.push_back('\0');
    std::string hash;
    bool result = osudb::get_hash(filename, hash);
    if (!result)
    {
        osudb::init();
        result = osudb::get_hash(filename, hash);
    }
    if (!result) return FALSE;
    replayengine::MutableCurrentView()->mut_metadata().beatmap_hash = hash;
    return TRUE;
}


DLLFUN(void) OnResize(int width, int height)
{
    zoom_pan.on_resize(width, height);
}

DLLFUN(void) MouseDown(float x, float y)
{
    glm::vec2 v(x, y);
    pixel_to_virtual(v);
    ui::mouse_down(v);
}

DLLFUN(void) MouseUp(float x, float y)
{
    glm::vec2 v(x, y);
    pixel_to_virtual(v);
    ui::mouse_up(v);
}

DLLFUN(void) MouseMove(float x, float y)
{
    glm::vec2 v(x, y);
    pixel_to_virtual(v);
    ui::mouse_move(v);
}

DLLFUN(void) MouseDownRight(float x, float y)
{
    glm::vec2 v(x, y);
    pixel_to_virtual(v);
    ui::mouse_down_right(v);
}

DLLFUN(void) MouseUpRight(float x, float y)
{
    glm::vec2 v(x, y);
    pixel_to_virtual(v);
    ui::mouse_up_right(v);
}

DLLFUN(void) MouseWheel(float x, float y, BOOL isUp)
{
    glm::vec2 v(x, y);
    pixel_to_virtual(v);
    ui::mouse_wheel(v, isUp);
}

DLLFUN(void) SetDrawFramesOnTimeline(BOOL value)
{
    ui::draw_frames_on_timeline = value;
}

DLLFUN(void) SetDrawHitWindows(BOOL value)
{
    ui::draw_hitwindows_on_timeline = value;
}

DLLFUN(void) SetDrawSliderendRange(BOOL value)
{
    ui::draw_sliderend_range = value;
}

DLLFUN(void) Render()
{
    zoom_pan.set_projection();
    glClear(GL_COLOR_BUFFER_BIT);
    const double t = audioengine::handle->get_time();
    beatmapengine::draw(t, ui::draw_sliderend_range);
    replayengine::MutableCurrentView()->draw(t);
    ui::draw(t);
    if (ui::draw_hitwindows_on_timeline) beatmapengine::draw_windows(t);
    if (ui::draw_frames_on_timeline) ui::draw_frame_lines(t);
    wglSwapLayerBuffers(graphics_handle.hdc, WGL_SWAP_MAIN_PLANE);
}

DLLFUN(void) Play()
{
    audioengine::handle->play();
}

DLLFUN(void) Pause()
{
    audioengine::handle->pause();
}

DLLFUN(void) Stop()
{
    audioengine::handle->stop();
}

DLLFUN(void) TogglePause()
{
    audioengine::handle->toggle_pause();
}

DLLFUN(BOOL) IsPlaying()
{
    return audioengine::handle->is_playing();
}

DLLFUN(BOOL) IsPaused()
{
    return audioengine::handle->is_paused();
}

DLLFUN(BOOL) IsStopped()
{
    return audioengine::handle->is_stopped();
}

DLLFUN(void) JumpTo(SongTime_t ms)
{
    audioengine::handle->jump_to(ms);
}

DLLFUN(void) RelJump(double ms)
{
    audioengine::handle->rel_jump(ms);
}

DLLFUN(void) SetVolume(float percent)
{
    audioengine::handle->set_volume(percent);
}

DLLFUN(float) GetVolume()
{
    return audioengine::handle->get_volume();
}

DLLFUN(void) SetPlaybackSpeed(float multiplier)
{
    audioengine::handle->set_playback_speed(multiplier);
}

DLLFUN(float) GetPlaybackSpeed()
{
    return audioengine::handle->get_playback_speed();
}

DLLFUN(double) GetTime()
{
    return audioengine::handle->get_time();
}

// TRAIL_OFF = 0;
// TRAIL_RAW = 1;
// TRAIL_HITS = 2;
DLLFUN(void) SetCursorTrail(int kind)
{
    if (kind >= 0 && kind < static_cast<int>(ui::TrailMode::num_trail_modes)) {
        ui::trail_mode = static_cast<ui::TrailMode>(kind);
    }
}

DLLFUN(void) SetCursorTrailLength(SongTime_t ms)
{
    ui::trail_length = ms;
}

DLLFUN(SongTime_t) GetReplayStartMs()
{
    return beatmapengine::first_hitobject_ms;
}

DLLFUN(SongTime_t) GetReplayEndMs()
{
    return beatmapengine::last_hitobject_ms;
}

DLLFUN(float) GetBeatmapStackLeniency()
{
    return beatmapengine::stack_leniency;
}

DLLFUN(float) GetBeatmapHP()
{
    return beatmapengine::hp;
}

DLLFUN(float) GetBeatmapCS()
{
    return beatmapengine::cs;
}

DLLFUN(float) GetBeatmapOD()
{
    return beatmapengine::od;
}

DLLFUN(float) GetBeatmapAR()
{
    return beatmapengine::ar;
}

DLLFUN(float) GetBeatmapSliderMult()
{
    return beatmapengine::slider_mult;
}

DLLFUN(void) SetBeatmapStackLeniency(float value)
{
    beatmapengine::stack_leniency = value;
}

DLLFUN(void) SetBeatmapHP(float value)
{
    beatmapengine::hp = value;
}

DLLFUN(void) SetBeatmapCS(float value)
{
    beatmapengine::cs = value;
}

DLLFUN(void) SetBeatmapOD(float value)
{
    beatmapengine::od = value;
}

DLLFUN(void) SetBeatmapAR(float value)
{
    beatmapengine::ar = value;
}

DLLFUN(void) SetBeatmapSliderMult(float value)
{
    beatmapengine::slider_mult = value;
}

DLLFUN(void) PlaceMarkIn()
{
    using ::replayengine::Replay;
    replayengine::MutateCurrentView([](Replay &replay) {
        replay.place_mark_in();
        return true;
    });
}

DLLFUN(void) PlaceMarkOut()
{
    using ::replayengine::Replay;
    replayengine::MutateCurrentView([](Replay &replay) {
        replay.place_mark_out();
        return true;
    });
}

DLLFUN(void) PlaceMarkMid()
{
    using ::replayengine::Replay;
    replayengine::MutateCurrentView([](Replay &replay) {
        replay.place_mark_mid();
        return true;
    });
}

DLLFUN(void) PlaceMarkAll()
{
    using ::replayengine::Replay;
    replayengine::MutateCurrentView([](Replay &replay) {
        replay.place_mark_all();
        return true;
    });
}

DLLFUN(void) ClearMarks()
{
    using ::replayengine::Replay;
    replayengine::MutateCurrentView([](Replay &replay) {
        replay.clear_marks();
        return true;
    });
}

DLLFUN(BOOL) AreMarksMidConsistent()
{
    return replayengine::CurrentView()->are_all_marks_consistent();
}

DLLFUN(BOOL) AreMarksInOutConsistent()
{
    return replayengine::CurrentView()->are_in_out_marks_consistent();
}

DLLFUN(BOOL) SetFrameKeyPress(int mask)
{
    using ::replayengine::Replay;
    if (replayengine::CurrentView()->are_in_out_marks_consistent()) {
        return replayengine::MutateCurrentView([mask](Replay &replay) {
            for (auto &frame : replay.mut_selection()) {
                frame.keys = mask;
            }
            return true;
        });
    } else {
        return FALSE;
    }
}

DLLFUN(BOOL) MoveFrames(int ms)
{
    using ::replayengine::Replay;
    if (replayengine::CurrentView()->are_in_out_marks_consistent()) {
        return replayengine::MutateCurrentView([ms](Replay &replay) { return replay.move_frames_range(ms); });
    } else
        return false;
}

DLLFUN(BOOL) DeleteFrames()
{
    using ::replayengine::Replay;
    if (replayengine::CurrentView()->are_in_out_marks_consistent()) {
        return replayengine::MutateCurrentView([](Replay &replay) { return replay.delete_frames_range(); });
    } else
        return false;
}

DLLFUN(BOOL) CenterFrames()
{
    using ::replayengine::Replay;
    
    if (replayengine::CurrentView()->are_in_out_marks_consistent()) {
        return replayengine::MutateCurrentView([](Replay &replay) { return replay.center_frames_range(); });
    } else
        return false;
}

DLLFUN(BOOL) ScaleFrames(float scale)
{
    using ::replayengine::Replay;
    if (replayengine::CurrentView()->are_in_out_marks_consistent()) {
        return replayengine::MutateCurrentView([scale](Replay &replay) { return replay.scale_frames(scale); });
    } else
        return false;
}

DLLFUN(BOOL) AddFrame(int ms)
{
    using ::replayengine::Replay;

    if (ms <= 0) 
        return replayengine::MutateCurrentView([](Replay &replay) { return replay.insert_new_frame_between(); });
    else
        return replayengine::MutateCurrentView([ms](Replay &replay) { return replay.insert_new_frame(ms); });

}

DLLFUN(BOOL) DeviceMarkAllFrames(BOOL isKeyboard)
{
    using ::replayengine::Replay;

    return replayengine::MutateCurrentView([isKeyboard](Replay &replay) {
        replay.mark_all_frames(isKeyboard);
        return true;
    });
}

DLLFUN(BOOL) LoadSave(const wchar_t *saveFileName)
{
    return FALSE;
}

DLLFUN(BOOL) WriteSave(const wchar_t *saveFileName)
{
    return FALSE;
}

DLLFUN(BOOL) ExportAsOsr(const wchar_t *osrFileName)
{
    return replayengine::MutableCurrentView()->write_to(osrFileName);
}

DLLFUN(void) VisualMapInvert(BOOL value)
{
    beatmapengine::hitobjects_inverted = value;
}

DLLFUN(void) InvertCursorData()
{
    using ::replayengine::Replay;
    replayengine::MutateCurrentView([](Replay &replay) {
        replay.invert_replay_frames();
        return true;
    });
}

DLLFUN(BYTE) Replay_GetGamemode()
{
    return replayengine::CurrentView()->metadata().game_mode;
}

DLLFUN(INT32) Replay_GetVersion()
{
    return replayengine::CurrentView()->metadata().version;
}

DLLFUN(INT64) Replay_GetPlayTimestamp()
{
    return replayengine::CurrentView()->metadata().play_timestamp;
}

DLLFUN(void) Replay_GetPlayerName(BYTE *out_str, INT *len)
{
    return_a_string(out_str, len, replayengine::CurrentView()->metadata().player_name);
}

DLLFUN(INT16) Replay_GetNum300()
{
    return replayengine::CurrentView()->metadata().num_300;
}

DLLFUN(INT16) Replay_GetNum100()
{
    return replayengine::CurrentView()->metadata().num_100;
}

DLLFUN(INT16) Replay_GetNum50()
{
    return replayengine::CurrentView()->metadata().num_50;
}

DLLFUN(INT16) Replay_GetNumGeki()
{
    return replayengine::CurrentView()->metadata().num_geki;
}

DLLFUN(INT16) Replay_GetNumKatu()
{
    return replayengine::CurrentView()->metadata().num_katu;
}

DLLFUN(INT16) Replay_GetNumMiss()
{
    return replayengine::CurrentView()->metadata().num_miss;
}

DLLFUN(INT32) Replay_GetTotalScore()
{
    return replayengine::CurrentView()->metadata().total_score;
}

DLLFUN(INT16) Replay_GetMaxCombo()
{
    return replayengine::CurrentView()->metadata().max_combo;
}

DLLFUN(BOOL) Replay_GetFullCombo()
{
    return replayengine::CurrentView()->metadata().full_combo;
}

DLLFUN(UINT32) Replay_GetMods()
{
    return replayengine::CurrentView()->metadata().mods;
}

DLLFUN(void) Replay_SetVersion(INT value)
{
    replayengine::MutableCurrentView()->mut_metadata().version = value;
}

DLLFUN(void) Replay_SetPlayTimestamp(INT64 value)
{
    replayengine::MutableCurrentView()->mut_metadata().play_timestamp = value;
}

DLLFUN(void) Replay_SetGamemode(BYTE value)
{
    replayengine::MutableCurrentView()->mut_metadata().game_mode = value;
}

DLLFUN(void) Replay_SetPlayerName(const wchar_t *str, INT len)
{
    set_a_string(str, len, replayengine::MutableCurrentView()->mut_metadata().player_name);
}

DLLFUN(void) Replay_SetNum300(INT16 value)
{
    replayengine::MutableCurrentView()->mut_metadata().num_300 = value;
}

DLLFUN(void) Replay_SetNum100(INT16 value)
{
    replayengine::MutableCurrentView()->mut_metadata().num_100 = value;
}

DLLFUN(void) Replay_SetNum50(INT16 value)
{
    replayengine::MutableCurrentView()->mut_metadata().num_50 = value;
}

DLLFUN(void) Replay_SetNumGeki(INT16 value)
{
    replayengine::MutableCurrentView()->mut_metadata().num_geki = value;
}

DLLFUN(void) Replay_SetNumKatu(INT16 value)
{
    replayengine::MutableCurrentView()->mut_metadata().num_katu = value;
}

DLLFUN(void) Replay_SetNumMiss(INT16 value)
{
    replayengine::MutableCurrentView()->mut_metadata().num_miss = value;
}

DLLFUN(void) Replay_SetTotalScore(INT32 value)
{
    replayengine::MutableCurrentView()->mut_metadata().total_score = value;
}

DLLFUN(void) Replay_SetMaxCombo(INT16 value)
{
    replayengine::MutableCurrentView()->mut_metadata().max_combo = value;
}

DLLFUN(void) Replay_SetFullCombo(BOOL value)
{
    replayengine::MutableCurrentView()->mut_metadata().full_combo = value;
}

DLLFUN(void) Replay_SetMods(UINT32 value)
{
    replayengine::MutableCurrentView()->mut_metadata().mods = value;
}

DLLFUN(void) ResetPanZoom()
{
    zoom_pan.reset();
}

DLLFUN(void) ZoomIn()
{
    zoom_pan.mut_zoom() += 5.0;
    zoom_pan.set_dirty();
}

DLLFUN(void) ZoomOut()
{
    zoom_pan.mut_zoom() -= 5.0;
    zoom_pan.set_dirty();
}

DLLFUN(BOOL) Undo()
{
    return replayengine::Undo();
}

DLLFUN(void) MakeUndoSnapshot()
{
    replayengine::DuplicateCurrentView();
}

DLLFUN(BOOL) Redo()
{
    return replayengine::Redo();
}

DLLFUN(void)
AnalyzeAccuracy(BOOL do_trace, int *num_300, int *num_100, int *num_50, int *num_miss, float *accuracy,
                float *avg_early, float *avg_late, float *unstable_rate)
{
    accuracy_analyzer::Stats stats;
    accuracy_analyzer::analyze(&stats, do_trace);
    *num_300 = stats.num_300;
    *num_100 = stats.num_100;
    *num_50 = stats.num_50;
    *num_miss = stats.num_miss;
    *accuracy = stats.accuracy;
    *avg_early = stats.avg_early;
    *avg_late = stats.avg_late;
    *unstable_rate = stats.unstable_rate;
}

DLLFUN(INT) NextMiss()
{
    return accuracy_analyzer::next_hitobject([](const hitobject_t &obj) { return obj.is_miss; });
}

DLLFUN(INT) Next50()
{
    const int w50 = beatmapengine::hitwindow50();
    const int w100 = beatmapengine::hitwindow100();
    const int w300 = beatmapengine::hitwindow300();
    return accuracy_analyzer::next_hitobject(
        [=](const hitobject_t &obj) { return obj.get_points(w50, w100, w300) == 50; });
}

DLLFUN(INT) Next100()
{
    const int w50 = beatmapengine::hitwindow50();
    const int w100 = beatmapengine::hitwindow100();
    const int w300 = beatmapengine::hitwindow300();
    return accuracy_analyzer::next_hitobject(
        [=](const hitobject_t &obj) { return obj.get_points(w50, w100, w300) == 100; });
}

DLLFUN(INT) Next300()
{
    const int w50 = beatmapengine::hitwindow50();
    const int w100 = beatmapengine::hitwindow100();
    const int w300 = beatmapengine::hitwindow300();
    return accuracy_analyzer::next_hitobject(
        [=](const hitobject_t &obj) { return obj.get_points(w50, w100, w300) == 300; });
}

DLLFUN(INT) NextHitObject()
{
    return accuracy_analyzer::next_hitobject([](const hitobject_t &obj) { return true; });
}

DLLFUN(BOOL) GetHitInfo(INT index, INT *kind, BOOL *is_miss, INT *hit_error, INT *points)
{
    if (index < 0 || index >= beatmapengine::hitobjects.size()) return FALSE;
    const auto &obj = beatmapengine::hitobjects[index];
    *kind = static_cast<int>(obj.hitobject_type);
    *is_miss = obj.is_miss;
    *hit_error = obj.hit_error;
    const int w50 = beatmapengine::hitwindow50();
    const int w100 = beatmapengine::hitwindow100();
    const int w300 = beatmapengine::hitwindow300();
    *points = obj.get_points(w50, w100, w300);
    return TRUE;
}

DLLFUN(void) SetTool(INT tool)
{
    if (tool >= 0 && tool < static_cast<int>(tool::ToolType::num_tools)) {
        tool::CurrentToolType(static_cast<tool::ToolType>(tool));
    }
}

DLLFUN(void) SetBrushRadius(INT brush_radius)
{
    tool::brush_radius = static_cast<float>(brush_radius);
}
