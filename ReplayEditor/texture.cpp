// clang-format off
#include "stdafx.h"
// clang-format on
#include "texture.hpp"

#include <string.h>

#include <memory>

#include "../LZMA/LzmaDec.h"
#include "breader.hpp"

#define glTranslate(x, y, z) glTranslatef(static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z))
#define glRotate(r, x, y, z) \
    glRotatef(static_cast<GLfloat>(r), static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z))

texture_t::texture_t(int w, int h, void *data, GLint tex_filter) : width(w), height(h)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex_filter);
    glGetError();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    GLenum err = glGetError();
    switch (err) {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            fatal("glTexImage2D: GL_INVALID_ENUM");
        case GL_INVALID_VALUE:
            fatal("glTexImage2D: GL_INVALID_VALUE");
        case GL_INVALID_OPERATION:
            fatal("glTexImage2D: GL_INVALID_OPERATION");
        default:
            fatal("glTexImage2D: unknown error");
    }
}

texture_t *texture_t::from_file(const std::wstring &path, const int width, const int height, GLint tex_filter)
{
    breader_t r(path);
    if (r.is_closed()) {
        return nullptr;
    }
    const int size = width * height * 4;
    auto img = std::make_unique<uint8_t[]>(size);
    r.fill_buf(img.get(), size);
    texture_t *const tex = new texture_t(width, height, img.get(), tex_filter);
    return tex;
}

void texture_t::draw(glm::vec2 pos, glm::vec2 origin, float rotation) const
{
    pos -= origin;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (rotation != 0) {
        glm::vec2 diff = -pos - origin;
        glTranslate(-diff.x, -diff.y, 0);
        glRotate(rotation, 0.0f, 0.0f, 1.0f);
        glTranslate(diff.x, diff.y, 0);
    }
    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(pos.x, pos.y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(pos.x + width, pos.y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(pos.x + width, pos.y + height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(pos.x, pos.y + height);
    glEnd();
}

void texture_t::draw(glm::vec2 pos, glm::vec2 origin, glm::vec2 size, float rotation) const
{
    pos -= origin;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (rotation != 0) {
        glm::vec2 diff = -pos - origin;
        glTranslate(-diff.x, -diff.y, 0);
        glRotate(rotation, 0.0f, 0.0f, 1.0f);
        glTranslate(diff.x, diff.y, 0);
    }
    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(pos.x, pos.y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(pos.x + size.x, pos.y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(pos.x + size.x, pos.y + size.y);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(pos.x, pos.y + size.y);
    glEnd();
}

void texture_t::draw(glm::vec2 pos, glm::vec2 origin) const
{
    pos -= origin;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(pos.x, pos.y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(pos.x + width, pos.y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(pos.x + width, pos.y + height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(pos.x, pos.y + height);
    glEnd();
}

void texture_t::draw(glm::vec2 pos, glm::vec2 origin, glm::vec2 size) const
{
    pos -= origin;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(pos.x, pos.y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(pos.x + size.x, pos.y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(pos.x + size.x, pos.y + size.y);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(pos.x, pos.y + size.y);
    glEnd();
}

texture_t *textures::hitcircle = nullptr;
texture_t *textures::approachcircle = nullptr;
texture_t *textures::cursor = nullptr;
texture_t *textures::sliderbody = nullptr;
texture_t *textures::slideredge = nullptr;
texture_t *textures::vertex = nullptr;
texture_t *textures::markerA = nullptr;
texture_t *textures::markerB = nullptr;
texture_t *textures::slidertick = nullptr;

#define INIT_TEX(var_name, w, h)                                                        \
    do {                                                                                \
        var_name = texture_t::from_file(L"assets/" #var_name L".raw", w, h, GL_LINEAR); \
        if (var_name == nullptr) return false;                                          \
    } while (0)

bool textures::init()
{
    INIT_TEX(hitcircle, 128, 128);
    INIT_TEX(approachcircle, 128, 128);
    INIT_TEX(cursor, 32, 32);
    INIT_TEX(sliderbody, 128, 128);
    INIT_TEX(slideredge, 128, 128);
    INIT_TEX(vertex, 32, 32);
    INIT_TEX(markerA, 32, 32);
    INIT_TEX(markerB, 32, 32);
    INIT_TEX(slidertick, 16, 16);
    return true;
}
