#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include "thirdparty/glm/glm.hpp"

class texture_t
{
   private:
    GLuint id;
    texture_t(int w, int h, void *data, GLint text_filter);
    // texture_t(int w, int h, void *data) : texture_t(w, h, data, GL_LINEAR) {}
   public:
    GLuint get_id() const
    {
        return id;
    }
    const int width;
    const int height;
    static texture_t *from_file(const std::wstring &path, const int width, const int height, GLint tex_filter);
    void draw(glm::vec2 pos, glm::vec2 origin) const;
    void draw(glm::vec2 pos, glm::vec2 origin, glm::vec2 size) const;
    void draw(glm::vec2 pos, glm::vec2 origin, float rotation) const;
    void draw(glm::vec2 pos, glm::vec2 origin, glm::vec2 size, float rotation) const;
};

namespace textures
{

extern texture_t *hitcircle;
extern texture_t *approachcircle;
extern texture_t *cursor;
// extern texture_t *slider_body;
extern texture_t *sliderbody;
extern texture_t *slideredge;
extern texture_t *vertex;
extern texture_t *markerA;
extern texture_t *markerB;
extern texture_t *slidertick;
bool init();

}  // namespace textures
