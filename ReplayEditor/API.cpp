// clang-format off
#include "stdafx.h"
// clang-format on
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

class GraphicsHandle
{
   public:
    HWND hWnd;
    HDC hdc;
    HGLRC hrc;
};

static GraphicsHandle graphics_handle;

static void setup_graphics(HWND hWnd)
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
    if (!replayengine::init(L"")) return REPLAYENGINE_FAILURE;
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

DLLFUN(BOOL) LoadReplay(const wchar_t *fname)
{
    if (!replayengine::init(fname)) return FALSE;
    std::wstring osu_path;
    std::wstring song_path;
    const bool found = osudb::get_entry(replayengine::beatmap_hash, osu_path, song_path);
    if (!found) return FALSE;
    if (!audioengine::load(song_path)) return FALSE;
    if (!beatmapengine::init(osu_path)) return FALSE;
    audioengine::set_playback_speed(1.0f);
    return TRUE;
}

DLLFUN(void) OnResize(int width, int height)
{
    zoom_pan.on_resize(width, height);
}

static void pixel_to_virtual(glm::vec2 &v)
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

DLLFUN(void) Render()
{
    zoom_pan.set_projection();
    glClear(GL_COLOR_BUFFER_BIT);
    const SongTime_t t = audioengine::get_time();
    beatmapengine::draw(t);
    replayengine::draw(t);
    ui::draw(t);
    wglSwapLayerBuffers(graphics_handle.hdc, WGL_SWAP_MAIN_PLANE);
}

DLLFUN(void) Play()
{
    audioengine::play();
}

DLLFUN(void) Pause()
{
    audioengine::pause();
}

DLLFUN(void) Stop()
{
    audioengine::stop();
}

DLLFUN(void) TogglePause()
{
    audioengine::toggle_pause();
}

DLLFUN(BOOL) IsPlaying()
{
    return audioengine::is_playing();
}

DLLFUN(BOOL) IsPaused()
{
    return audioengine::is_paused();
}

DLLFUN(BOOL) IsStopped()
{
    return audioengine::is_stopped();
}

DLLFUN(void) JumpTo(SongTime_t ms)
{
    audioengine::jump_to(ms);
}

DLLFUN(void) RelJump(SongTime_t ms)
{
    audioengine::rel_jump(ms);
}

DLLFUN(void) SetVolume(float percent)
{
    audioengine::set_volume(percent);
}

DLLFUN(float) GetVolume()
{
    return audioengine::get_volume();
}

DLLFUN(void) SetPlaybackSpeed(float multiplier)
{
    audioengine::set_playback_speed(multiplier);
}

DLLFUN(float) GetPlaybackSpeed()
{
    return audioengine::get_playback_speed();
}

DLLFUN(SongTime_t) GetTime()
{
    return audioengine::get_time();
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

static void place_mark_mid_avg()
{
    if (ui::is_in_out_mark_consistent()) {
        const I64 a = ui::mark_in;
        const I64 b = ui::mark_out;
        ui::mark_mid = a + (b - a) / 2;
    }
}

DLLFUN(void) PlaceMarkIn()
{
    ui::mark_in = replayengine::current_frame_index;
    place_mark_mid_avg();
}

DLLFUN(void) PlaceMarkOut()
{
    ui::mark_out = replayengine::current_frame_index;
    place_mark_mid_avg();
}

DLLFUN(void) PlaceMarkMid()
{
    ui::mark_mid = replayengine::current_frame_index;
}

DLLFUN(void) PlaceMarkAll()
{
    ui::mark_in = 0;
    ui::mark_out = replayengine::frames.size() - 1;
    ui::mark_mid = 1;
}

DLLFUN(void) ClearMarks()
{
    ui::mark_in = -1;
    ui::mark_out = -1;
    ui::mark_mid = -1;
}

DLLFUN(BOOL) AreMarksMidConsistent()
{
    return ui::are_marks_consistent();
}

DLLFUN(BOOL) AreMarksInOutConsistent()
{
    return ui::is_in_out_mark_consistent();
}

DLLFUN(BOOL) SetFrameKeyPress(int mask)
{
    if (ui::is_in_out_mark_consistent()) {
        ui::make_undo_restore_point();
        for (I64 i = ui::mark_in; i <= ui::mark_out; ++i) {
            replayengine::frames[i].keys = mask;
        }
        return TRUE;
    } else {
        return FALSE;
    }
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
    if (replayengine::write_to(osrFileName))
        return TRUE;
    else
        return FALSE;
}

DLLFUN(void) VisualMapInvert(BOOL value)
{
    beatmapengine::hitobjects_inverted = value;
}

DLLFUN(void) InvertCursorData()
{
    replayengine::invert_replay_frames();
}

static void return_a_string(BYTE *out_str, INT *out_len, const std::string &s)
{
    int i;
    for (i = 0; i < *out_len && i < static_cast<int>(s.size()); ++i) {
        out_str[i] = static_cast<BYTE>(s[i]);
    }
    *out_len = i;
}

static void set_a_string(const wchar_t *in_str, INT in_len, std::string &s)
{
    s.resize(in_len);
    for (int i = 0; i < in_len; ++i) {
        if (in_str[i] < 0x20 || in_str[i] >= 127)
            s[i] = '?';
        else
            s[i] = static_cast<char>(in_str[i]);
    }
}

DLLFUN(BYTE) Replay_GetGamemode()
{
    return replayengine::game_mode;
}

DLLFUN(INT32) Replay_GetVersion()
{
    return replayengine::version;
}

DLLFUN(INT64) Replay_GetTimestamp()
{
    return replayengine::timestamp;
}

DLLFUN(void) Replay_GetPlayerName(BYTE *out_str, INT *len)
{
    return_a_string(out_str, len, replayengine::player_name);
}

DLLFUN(INT16) Replay_GetNum300()
{
    return replayengine::num_300;
}

DLLFUN(INT16) Replay_GetNum100()
{
    return replayengine::num_100;
}

DLLFUN(INT16) Replay_GetNum50()
{
    return replayengine::num_50;
}

DLLFUN(INT16) Replay_GetNumGeki()
{
    return replayengine::num_geki;
}

DLLFUN(INT16) Replay_GetNumKatu()
{
    return replayengine::num_katu;
}

DLLFUN(INT16) Replay_GetNumMiss()
{
    return replayengine::num_miss;
}

DLLFUN(INT32) Replay_GetTotalScore()
{
    return replayengine::total_score;
}

DLLFUN(INT16) Replay_GetMaxCombo()
{
    return replayengine::max_combo;
}

DLLFUN(BOOL) Replay_GetFullCombo()
{
    return replayengine::full_combo ? TRUE : FALSE;
}

DLLFUN(UINT32) Replay_GetMods()
{
    return replayengine::mods;
}

DLLFUN(void) Replay_SetVersion(INT value)
{
    replayengine::version = value;
}

DLLFUN(void) Replay_SetTimestamp(INT64 value)
{
    replayengine::timestamp = value;
}

DLLFUN(void) Replay_SetGamemode(BYTE value)
{
    replayengine::game_mode = value;
}

DLLFUN(void) Replay_SetPlayerName(const wchar_t *str, INT len)
{
    set_a_string(str, len, replayengine::player_name);
}

DLLFUN(void) Replay_SetNum300(INT16 value)
{
    replayengine::num_300 = value;
}

DLLFUN(void) Replay_SetNum100(INT16 value)
{
    replayengine::num_100 = value;
}

DLLFUN(void) Replay_SetNum50(INT16 value)
{
    replayengine::num_50 = value;
}

DLLFUN(void) Replay_SetNumGeki(INT16 value)
{
    replayengine::num_geki = value;
}

DLLFUN(void) Replay_SetNumKatu(INT16 value)
{
    replayengine::num_katu = value;
}

DLLFUN(void) Replay_SetNumMiss(INT16 value)
{
    replayengine::num_miss = value;
}

DLLFUN(void) Replay_SetTotalScore(INT32 value)
{
    replayengine::total_score = value;
}

DLLFUN(void) Replay_SetMaxCombo(INT16 value)
{
    replayengine::max_combo = value;
}

DLLFUN(void) Replay_SetFullCombo(BOOL value)
{
    replayengine::full_combo = value;
}

DLLFUN(void) Replay_SetMods(UINT32 value)
{
    replayengine::mods = value;
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
    return ui::undo() ? TRUE : FALSE;
}

DLLFUN(BOOL) Redo()
{
    return ui::redo() ? TRUE : FALSE;
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
    if (tool >= 0 && tool < static_cast<int>(tool::WhichTool::num_tools)) {
        tool::current_tool(static_cast<tool::WhichTool>(tool));
    }
}

DLLFUN(void) SetBrushRadius(INT brush_radius)
{
    tool::brush_radius = static_cast<float>(brush_radius);
}