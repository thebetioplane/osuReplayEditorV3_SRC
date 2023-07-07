using System;
using System.Runtime.InteropServices;

namespace osuReplayEditor
{
    public static class API
    {
        /// <summary>
        /// Reads the config file, reads osu!.db, sets up graphics engine, sets up audio engine
        /// </summary>
        /// <returns>true on success, false otherwise</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern int Init(IntPtr hWnd, string osuDBPath, string songFolderPath);

        /// <summary>
        /// Cleans up anything created in the dll
        /// </summary>
        /// <returns>true on success, false otherwise</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool Cleanup();

        /// <summary>
        /// loads an osr file, automatically finds beatmap information from osu!.db
        /// </summary>
        /// <param name="fname">the path to the .osr file</param>
        /// <returns>true on success, false otherwise</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern bool LoadReplay(string fname);

        /// <summary>
        /// call this when your openGL contorl resizes
        /// </summary>
        /// <param name="width">width in pixels</param>
        /// <param name="height">height in pixels</param>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void OnResize(int width, int height);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void MouseDown(float x, float y);
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void MouseUp(float x, float y);
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void MouseMove(float x, float y);
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void MouseDownRight(float x, float y);
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void MouseUpRight(float x, float y);
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void MouseWheel(float x, float y, bool isUp);

        /// <summary>
        /// draw the beatmap and cursor data (expects openGL context to be made)
        /// </summary>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Render();

        /// <summary>
        /// plays the song, if it is already playing does nothing
        /// </summary>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Play();

        /// <summary>
        /// pauses the song, if it is already paused does nothing
        /// </summary>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Pause();

        /// <summary>
        /// stops the song, if it is already stopped does nothing
        /// </summary>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Stop();

        /// <summary>
        /// pauses the song if it is not paused, otherwise plays it
        /// </summary>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void TogglePause();

        /// <summary>
        /// checks if song is playing
        /// </summary>
        /// <returns>true if it is playing, false otherwise</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool IsPlaying();

        /// <summary>
        /// checks if song is paused
        /// </summary>
        /// <returns>true if it is paused, false otherwise</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool IsPaused();

        /// <summary>
        /// checks if song is stopped
        /// </summary>
        /// <returns>true if it is stopped, false otherwise</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool IsStopped();

        /// <summary>
        /// goes to a certain position in the beatmap
        /// </summary>
        /// <param name="ms">time in miliseconds since the start</param>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void JumpTo(int ms);

        /// <summary>
        /// jumps ahead or behind the current position
        /// </summary>
        /// <param name="ms">time in miliseconds relative to the current position in the song</param>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void RelJump(int ms);

        /// <summary>
        /// changes the volume of the audio engine
        /// </summary>
        /// <param name="percent">amount from 0.0 (mute) -> 1.0 (maximum)</param>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetVolume(float percent);

        /// <summary>
        /// gets the volume of the audio engine
        /// </summary>
        /// <returns>amount from 0.0 (mute) -> 1.0 (maximum)</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float GetVolume();

        /// <summary>
        /// changes the playback speed of the audio and beatmap engine
        /// </summary>
        /// <param name="multiplier">speed relative from original, ie 0.5 = half speed, 2.0 = double speed</param>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetPlaybackSpeed(float multiplier);

        /// <summary>
        /// gets the playback speed of the audio and beatmap engine
        /// </summary>
        /// <returns>speed relative from original, ie 0.5 = half speed, 2.0 = double speed</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float GetPlaybackSpeed();

        /// <summary>
        /// gets the current position in the song
        /// </summary>
        /// <returns>time in ms from the beginning of the song</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetTime();

        /// <summary>
        /// changes the cursor display
        /// </summary>
        /// <param name="kind">0 = normal, 1 = raw frames, 2 = hits only</param>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetCursorTrail(int kind);

        /// <summary>
        /// changes how far back the cursor trail goes, in miliseconds
        /// </summary>
        /// <param name="sec">how far back the trail should be from the current position, in miliseconds</param>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetCursorTrailLength(int ms);

        /// <summary>
        /// gets animation start of first hit object
        /// </summary>
        /// <returns>beatmap animation start in miliseconds</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetReplayStartMs();

        /// <summary>
        /// gets animation end of last hit object
        /// </summary>
        /// <returns>beatmap animation end in miliseconds</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetReplayEndMs();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float GetBeatmapStackLeniency();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float GetBeatmapHP();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float GetBeatmapCS();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float GetBeatmapOD();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float GetBeatmapAR();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float GetBeatmapSliderMult();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetBeatmapStackLeniency(float value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetBeatmapHP(float value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetBeatmapCS(float value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetBeatmapOD(float value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetBeatmapAR(float value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetBeatmapSliderMult(float value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void PlaceMarkIn();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void PlaceMarkOut();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void PlaceMarkMid();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void PlaceMarkAll();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void ClearMarks();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool AreMarksMidConsistent();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool AreMarksInOutConsistent();

        /// <summary>
        /// changes the keypresses for the marked area
        /// </summary>
        /// <param name="mask">bitmask of keydata, M1 = 1, M2 = 2, K1 = 4, K2 = 8, Smoke = 16) (K1 is always used with M1; K2 is always used with M2: 1+4=5; 2+8=10)</param>
        /// <returns>true if success, false if not (perhaps due to no valid selection)</returns>
        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetFrameKeyPress(int mask);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern bool LoadSave(string saveFileName);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern bool WriteSave(string saveFileName);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern bool ExportAsOsr(string osrFileName);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void VisualMapInvert(bool value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void InvertCursorData();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern byte Replay_GetGamemode();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Replay_GetVersion();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern long Replay_GetPlayTimestamp();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_GetPlayerName(byte[] buf, ref int len);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern short Replay_GetNum300();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern short Replay_GetNum100();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern short Replay_GetNum50();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern short Replay_GetNumGeki();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern short Replay_GetNumKatu();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern short Replay_GetNumMiss();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Replay_GetTotalScore();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern short Replay_GetMaxCombo();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool Replay_GetFullCombo();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern uint Replay_GetMods();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetGamemode(byte value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetVersion(int value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetPlayTimestamp(long value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void Replay_SetPlayerName(string name, int len);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetNum300(short value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetNum100(short value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetNum50(short value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetNumGeki(short value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetNumKatu(short value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetNumMiss(short value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetTotalScore(int value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetMaxCombo(short value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetFullCombo(bool value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Replay_SetMods(uint value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void ResetPanZoom();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void ZoomIn();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void ZoomOut();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool Undo();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool Redo();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void MakeUndoSnapshot();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void AnalyzeAccuracy(bool doTrace, ref int num300, ref int num100, ref int num50, ref int numMiss, ref float accuracy, ref float avgEarly, ref float avgLate, ref float unstableRate);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int NextHitObject();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Next300();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Next100();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Next50();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int NextMiss();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetHitInfo(int index, ref int kind, ref bool isMiss, ref int hitError, ref int points);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetTool(int tool);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetBrushRadius(int brushRadius);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void CfgSetUpdateTimeStampOnExit(int value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int CfgGetUpdateTimeStampOnExit();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void CfgSetVolume(int value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int CfgGetVolume();

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void CfgSetCursorMode(int value);

        [DllImport("ReplayEditor.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int CfgGetCursorMode();
    }
}
