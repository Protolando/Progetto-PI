#ifndef RENDERER_CLASS
#define RENDERER_CLASS

#include <map>
#include <string>

//Opengl
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rect.hpp"

#include "texture.hpp"

class Renderer {
public:
Renderer(GLFWwindow *, int, int, std::string*);
~Renderer();

GLint getTexureProgram();

void set_default_font(const char* font);
void set_default_font(std::string* font);

/**
 * \brief Resizes the window.
 * \param width The width of the window.
 * \param height The height of the window.
 */
void resize_window(int width, int height);

/**
 * \brief Clears the window
 */
void clear_window();

/**
 * \brief Sets the background color.
 * \param r Red as float [0.0f - 1.0f]
 * \param g Green as float [0.0f - 1.0f]
 * \param b Blue as float [0.0f - 1.0f]
 * \param alpha Alpha as float [0.0f - 1.0f]
 */
void set_background_color(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha);

/**
 * \brief Sets the background color.
 * \param color Sets the background color equals to the one
 *              passed as parameter. Values must be floats
 *              between 0.0f and 1.0f and represent Red, Green,
 *		Blue and Alpha in sequential order.
 */
void set_background_color(glm::vec4 color);

/**
 * \brief Draws a line.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 */
void draw_line(glm::vec2 start,
               glm::vec2 end,
               glm::vec4 color);

/**
 * \brief Draws a line.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 * \param thickness The thickness of the line as a float
 *                      (Default = 1)
 */
void draw_line(glm::vec2 start,
               glm::vec2 end,
               glm::vec4 color,
               GLfloat thickness);

/**
 * \brief Draws an empty rectangle.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 */
void draw_rect(
	glm::vec2 start,
	glm::vec2 end,
	glm::vec4 color);

/**
 * \brief Draws an empty rectangle.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 * \param thickness The thickness of the line as a float
 *                      (Default = 1)
 */
void draw_rect(
	glm::vec2 start,
	glm::vec2 end,
	glm::vec4 color,
	GLfloat thickness);

void draw_rect(
	Rect r,
	glm::vec4 color);

void draw_rect(
	Rect r,
	glm::vec4 color,
	GLfloat thickness);

void draw_rect(
	glm::vec2 top_left,
	glm::vec2 top_right,
	glm::vec2 bot_right,
	glm::vec2 bot_left,
	glm::vec4 color,
	GLfloat thickness);

/**
 * \brief Draws a filled rectangle.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 */
void fill_rect(glm::vec2 start,
               glm::vec2 end,
               glm::vec4 color);

void fill_rect(glm::vec2 start,
               int width,
               int height,
               glm::vec4 color);

/**
 * \brief Draws a filled rectangle.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 * \param thickness The thickness of the line as a float
 *                      (Default = 1)
 */
void fill_rect(glm::vec2 start,
               glm::vec2 end,
               glm::vec4 color,
               GLfloat thickness);

void draw_circle();

void fill_circle();

texture_t* generate_texture(const char* );
void draw_bitmap(glm::vec2, const char*);
void draw_bitmap(glm::vec2, const char*, int, int);
void draw_bitmap(glm::vec2, texture_t*);
void draw_bitmap(glm::vec2, texture_t*, int, int);

void draw_cropped_bitmap(glm::vec2, texture_t*, float, int, int);

std::pair<int, int> get_string_size(std::string text, int font_size);

/*
 * \brief Returns the size in pixels the string passed as parameters will take
 * \param text The text to be calculated
 * \param font_size The size of the font
 * \param font_location The path to the font
 * \return A pair (x, y) indicating the size of the string
 */
std::pair<int, int> get_string_size(
	std::string text,
	int font_size,
	const char *font_location
	);


void draw_text(
	std::string text,
	glm::vec2 position,
	glm::vec4 color,
	int font_size
	);

/**
 * \brief Draws Text on screen
 * \param text The text to be drawn
 * \param position The position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 * \param font_size The size of the font
 * \param font_location the path to the font
 */
void draw_text(
	std::string text,
	glm::vec2 position,
	glm::vec4 color,
	int font_size,
	const char *font_location
	);

void render();

/**
 * \brief Returns a pointer to the GLFW window.
 * \return A pointer to the GLFW window.
 */
GLFWwindow *get_window();

void set_cursor(int shape);
GLint get_attrib(GLuint program, const char *name);
GLint get_uniform(GLuint program, const char *name);

private:
/**
 * \brief Returns the window height.
 * \return The window h.
 */
int get_window_height();

/**
 * \brief Returns the window width.
 * \return The window width.
 */
int get_window_width();

int cursor_shape = GLFW_CURSOR_NORMAL;
GLFWcursor *cursor = NULL;

struct Character {
	GLuint TextureID; // ID handle of the glyph texture
	glm::ivec2 Size; // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance; // Horizontal offset to advance to next glyph
};
int window_width;
int window_height;
GLFWwindow* window;
glm::vec4 background_color;
GLuint shapes_program;
GLuint text_program;
GLuint pics_program;
std::map<std::string,std::map<int,std::map<GLchar, Character> > > fonts;
GLuint create_shader(const char *shader, GLenum type);
GLuint create_program(const char *vertex_shader, const char *fragment_shader);
char *default_font;
void freetype_init();
void font_init(std::string font_location, const int size);
int get_lowest_character_hanging(
	std::string text,
	int font_size,
	const char *font_location
	);
//Shaders
const char *shader_shapes_v =
	"#version 400\n"
	"in vec4 vp;"
	"out gl_PerVertex { vec4 gl_Position; };"
	"uniform mat4 projection;"
	"void main() {"
	"	gl_Position = projection * vp;"
	"}";
const char *shader_shapes_f =
	"#version 400\n"
	"uniform vec4 color;"
	"out vec4 frag_color;"
	"void main() {"
	"  frag_color = color;"
	"}";
const char *shader_text_f =
	"#version 330 core\n"
	"in vec2 TexCoords;"
	"out vec4 color;"
	"uniform sampler2D text;"
	"uniform vec3 textColor;"
	"void main()"
	"{"
	"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);"
	"color = vec4(textColor, 1.0) * sampled;"
	"};";
const char *shader_text_v =
	"#version 330 core\n"
	"layout (location = 0) in vec4 vertex;"
	"out vec2 TexCoords;"
	"uniform mat4 projection;"
	"void main()"
	"{"
	"gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);"
	"TexCoords = vertex.zw;"
	"}";
const char *shader_pics_v =
	"#version 400 core\n"
    "in vec4 position;"
	"uniform mat4 projection;"
    "in vec2 texcoord;"
    "out vec2 Texcoord;"
	"out gl_PerVertex { vec4 gl_Position; };"
    "void main()"
    "{"
        "Texcoord = texcoord;"
        "gl_Position = projection * position;"
    "}";
const char *shader_pics_f =
	"#version 400 core\n"
    "in vec2 Texcoord;"
    "uniform sampler2D tex;"
    "out vec4 frag_color;"
    "void main()"
    "{"
    	"vec4 texColor = texture(tex, Texcoord);"
    	"if(texColor.a < 0.1)"
        	"discard;"
        "frag_color = texColor;"
    "}";
};
#endif
