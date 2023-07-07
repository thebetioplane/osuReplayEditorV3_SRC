#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <functional>

#include "replayengine.hpp"

namespace tool
{

enum class ToolType { Select, Grab, Brush, num_tools };

class Tool
{
   public:
    virtual void OnMouseUp(const glm::vec2 &mouse)
    {
    }
    virtual void OnMouseDown(const glm::vec2 &mouse)
    {
    }
    virtual void OnMouseMove(const glm::vec2 &mouse)
    {
    }
    virtual void Draw()
    {
    }
};

ToolType CurrentToolType();
void CurrentToolType(ToolType new_tool);
extern Tool *current_tool;
extern float brush_radius;

}  // namespace tool
