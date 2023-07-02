#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <functional>

#include "replayengine.hpp"

namespace tool
{

enum class WhichTool { Sel, Grab, Brush, num_tools };
class ToolState
{
   public:
    bool enabled = false;
    glm::vec2 v0{0.f, 0.f};
    glm::vec2 v1{0.f, 0.f};
    I64 curr_frame_buf_in = 0;
    I64 curr_frame_buf_mid = 0;
    I64 curr_frame_buf_out = 0;
    std::vector<replayengine::replayframe_t> frame_buf;
    void copy_frames_into_buf();
};
using ToolCallback = std::function<void(ToolState& s)>;
WhichTool current_tool();
void current_tool(WhichTool new_tool);
extern ToolCallback apply;
extern ToolCallback draw;
extern float brush_radius;
void Sel_apply(ToolState& s);
void Sel_draw(ToolState& s);
void Grab_apply(ToolState& s);
void Grab_draw(ToolState& s);
void Brush_apply(ToolState& s);
void Brush_draw(ToolState& s);

}  // namespace tool
