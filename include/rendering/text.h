#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <freetype/freetype.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader_s.h"
/// Holds all state information relevant to a character as loaded using FreeType
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
void RenderTextMain(std::string text);
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};
Shader loadShaderFromFile2(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
void textDisplayConfig(void);