#include "renderer.hpp"
#include "soil/SOIL.h" 
#include <string.h>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

Renderer::Renderer(GLFWwindow *window, int window_width, int window_height, std::string* font){
	Renderer::window = window;
	Renderer::background_color = {1.0f, 1.0f, 1.0f, 1.0f};
	Renderer::window_width = window_width;
	Renderer::window_height = window_height;
	Renderer::default_font = NULL;

	if(shader_shapes_v && shader_shapes_f) {
		shapes_program = create_program(
			shader_shapes_v,
			shader_shapes_f
			);
	} else {
		fprintf(stderr, "Shapes shader not found\n");
	}

	if(shader_text_v && shader_text_f) {
		text_program = create_program(
			shader_text_v,
			shader_text_f
			);
	} else {
		fprintf(stderr, "Text shader not found\n");
	}
	
	if(shader_pics_v && shader_pics_f) {
		pics_program = create_program(
			shader_pics_v,
			shader_pics_f
			);
	} else {
		fprintf(stderr, "Text shader not found\n");
	}

	Renderer::set_default_font(font);
	//Initlalize freetype
	freetype_init();
}

Renderer::~Renderer(){
	free(Renderer::default_font);
	
	//Delete the programs
	glDeleteProgram(shapes_program);
	glDeleteProgram(text_program);
	glDeleteProgram(pics_program);
	
	//Delete the textures
	std::map<std::string,std::map<int,std::map<GLchar, Character> > >::iterator it1;
	std::map<int,std::map<GLchar, Character> >::iterator it2;
	std::map<GLchar, Character>::iterator it3;

	for(it1 = fonts.begin(); it1 != fonts.end(); it1++){
		
		for(it2 = it1->second.begin(); it2 != it1->second.end(); it2++){
			
			for(it3 = it2->second.begin(); it3 != it2->second.end(); it3++){
				glDeleteTextures(1, &(it3->second.TextureID));
			}
		}
	}
}

GLint Renderer::getTexureProgram(){
	return pics_program;
}

void Renderer::set_default_font(const char *font){
	if(default_font != NULL)
		free(default_font);
	
	char *tmp = (char*) malloc(sizeof(char) * (strlen(font)+1));
	strncpy(tmp, font, strlen(font));
	default_font = tmp;
}

void Renderer::set_default_font(std::string *font){
	Renderer::set_default_font(font->c_str());
	delete font;
}

/**
 * \brief Resizes the window.
 * \param width The width of the window.
 * \param height The height of the window.
 */
void Renderer::resize_window(int width, int height){
	Renderer::window_width = width;
	Renderer::window_height = height;
	glViewport(0, 0, width, height);
	
	Renderer::freetype_init();
}

/**
 * \brief Returns the window height.
 * \return The window h.
 */
int Renderer::get_window_height(){
	return Renderer::window_height;
}

/**
 * \brief Returns the window width.
 * \return The window width.
 */
int Renderer::get_window_width(){
	return Renderer::window_width;
}

/**
 * \brief Clears the window
 */
void Renderer::clear_window(){
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glClearColor(
		background_color[0],
		background_color[1],
		background_color[2],
		background_color[3]
	);
}

/**
 * \brief Sets the background color.
 * \param r Red as float [0.0f - 1.0f]
 * \param g Green as float [0.0f - 1.0f]
 * \param b Blue as float [0.0f - 1.0f]
 * \param alpha Alpha as float [0.0f - 1.0f]
 */
void Renderer::set_background_color(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha){
	Renderer::background_color[0] = r;
	Renderer::background_color[1] = g;
	Renderer::background_color[2] = b;
	Renderer::background_color[3] = alpha;
}

/**
 * \brief Sets the background color.
 * \param color Sets the background color equals to the one
 *              passed as parameter. Values must be floats
 *              between 0.0f and 1.0f and represent Red, Green,
 *		Blue and Alpha in sequential order.
 */
void Renderer::set_background_color(glm::vec4 color){
	Renderer::background_color = color;
}

/**
 * \brief Draws a line.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 */
void Renderer::draw_line(glm::vec2 start,
                         glm::vec2 end,
                         glm::vec4 color){
	draw_line(start, end, color, 1.0f);
}

/**
 * \brief Draws a line.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 * \param thickness The thickness of the line as a float
 *                      (Default = 1)
 */
void Renderer::draw_line(glm::vec2 start,
                         glm::vec2 end,
                         glm::vec4 color,
                         GLfloat thickness){
	//Put the vertices position in an array
	float points[] = {
		start[0], start[1],
		end[0], end[1],
	};

	glUseProgram(shapes_program);
	//Load the vertices position into the GPU buffers
	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLint attrib = get_attrib(shapes_program, "vp");
	glEnableVertexAttribArray(attrib);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	//Binding uniforms
	GLint uloc = 0;
	uloc = get_uniform(shapes_program, "color");
	glUniform4f(uloc, color[0], color[1], color[2], color[3]);

	uloc = get_uniform(shapes_program, "projection");
	glm::mat4 projection = glm::ortho(0.0f, (float) window_width, (float) window_height, 0.0f);
	glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(projection));

	//Bind the buffers to the program
	if(thickness != 1)
		glLineWidth(thickness);

	//Draw the line
	glDrawArrays(GL_LINES, 0, 2);

	glDeleteBuffers(1, &points_vbo);
	if(thickness != 1)
		glLineWidth(1);
}

/**
 * \brief Draws an empty rectangle.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 */
void Renderer::draw_rect(
	glm::vec2 start,
	glm::vec2 end,
	glm::vec4 color){
	draw_rect(start, end, color, 1.0f);
}

/**
 * \brief Draws an empty rectangle.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 * \param thickness The thickness of the line as a float
 *                      (Default = 1)
 */
void Renderer::draw_rect(
	glm::vec2 start,
	glm::vec2 end,
	glm::vec4 color,
	GLfloat thickness){
	
	Renderer::draw_rect(
		glm::vec2(start.x, start.y),
		glm::vec2(end.x, start.y),
		glm::vec2(end.x, end.y),
		glm::vec2(start.x, end.y),
		color,
		thickness
	);
}

void Renderer::draw_rect(
	Rect r,
	glm::vec4 color){
	Renderer::draw_rect(r, color, 1.0f);
}

void Renderer::draw_rect(
	Rect r,
	glm::vec4 color,
	GLfloat thickness){
	
	Renderer::draw_rect(
		r.getTopLeft(),
		r.getTopRight(),
		r.getBotRight(),
		r.getBotLeft(),
		color,
		thickness
	);
}

void Renderer::draw_rect(
	glm::vec2 top_left,
	glm::vec2 top_right,
	glm::vec2 bot_right,
	glm::vec2 bot_left,
	glm::vec4 color,
	GLfloat thickness){

	//Put the vertices position in an array
	float points[] = {
		top_left.x, top_left.y,
		top_right.x, top_right.y,
		bot_right.x, bot_right.y,
		bot_left.x, bot_left.y,
	};

	glUseProgram(shapes_program);
	//Load the vertices position into the GPU buffers
	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLint attrib = get_attrib(shapes_program, "vp");
	glEnableVertexAttribArray(attrib);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	//Binding uniforms
	GLint uloc = 0;
	uloc = get_uniform(shapes_program, "color");
	glUniform4f(uloc, color[0], color[1], color[2], color[3]);

	uloc = get_uniform(shapes_program, "projection");
	glm::mat4 projection = glm::ortho(0.0f, (float) window_width, (float) window_height, 0.0f);
	glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(projection));

	//Bind the buffers to the program
	if(thickness != 1)
		glLineWidth(thickness);

	//Draw the line
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glDeleteBuffers(1, &points_vbo);
	if(thickness != 1)
		glLineWidth(1);
}

/**
 * \brief Draws a filled rectangle.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 */
void Renderer::fill_rect(glm::vec2 start,
                         glm::vec2 end,
                         glm::vec4 color){
	fill_rect(start, end, color, 1.0f);
}


void Renderer::fill_rect(
	glm::vec2 start,
	int width,
	int height,
	glm::vec4 color
	){
	glm::vec2 end = {start.x+width, start.y+height};

	Renderer::fill_rect(start, end, color, 1.0f);
}

/**
 * \brief Draws a filled rectangle.
 * \param start The start position in screen coordinates
 * \param end The end position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 * \param thickness The thickness of the line as a float
 *                      (Default = 1)
 */
void Renderer::fill_rect(glm::vec2 start,
                         glm::vec2 end,
                         glm::vec4 color,
                         GLfloat thickness){

	//Put the vertices position in an array
	float points[] = {
		start[0], start[1],
		start[0], end[1],
		end[0], end[1],
		end[0], start[1],
	};

	glUseProgram(shapes_program);
	//Load the vertices position into the GPU buffers
	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	
	GLint attrib = get_attrib(shapes_program, "vp");
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	//Binding uniforms
	GLint uloc = 0;
	uloc = get_uniform(shapes_program, "color");
	glUniform4f(uloc, color[0], color[1], color[2], color[3]);

	uloc = get_uniform(shapes_program, "projection");
	glm::mat4 projection = glm::ortho(0.0f, (float) window_width, (float) window_height, 0.0f);
	glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(projection));

	if(thickness != 1)
		glLineWidth(thickness);
		
    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	

	glDeleteBuffers(1, &points_vbo);
	glDeleteBuffers(1, &ebo);
	if(thickness != 1)
		glLineWidth(1);
}

void Renderer::draw_circle(){
	throw "Not implemented";
}

void Renderer::fill_circle(){
	throw "Not implemented";
}


texture_t* Renderer::generate_texture(const char* location){
	// Load texture
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    int w, h;
    unsigned char* image = SOIL_load_image(location, &w, &h, 0, SOIL_LOAD_RGBA);
    
    if(image == NULL)
    	return NULL;
    
    glTexImage2D(
    	GL_TEXTURE_2D,
    	0,
    	GL_RGBA,
    	w,
    	h,
    	0,
    	GL_RGBA,
    	GL_UNSIGNED_BYTE,
    	image
    );
    SOIL_free_image_data(image);
    
    texture_t* t = (texture_t*) malloc(sizeof(texture_t));
    t->path = new std::string(location);
    t->width = w;
    t->height = h;
    t->buffer = tex;
    
    return t;
}

void Renderer::draw_bitmap(glm::vec2 pos, const char* location){
	Renderer::draw_bitmap(pos, location, 0, 0);
}

void Renderer::draw_bitmap(glm::vec2 pos, const char* location, int width, int height){

	glUseProgram(pics_program);
 	
 	texture_t* tex = Renderer::generate_texture(location);
 	Renderer::draw_bitmap(pos, tex, width, height);
	glDeleteTextures(1, &tex->buffer);
	free(tex);
}

void Renderer::draw_bitmap(glm::vec2 pos, texture_t* tex){
	Renderer::draw_bitmap(pos, tex, 0, 0);
}

void Renderer::draw_bitmap(glm::vec2 pos, texture_t* tex, int width, int height){
	glUseProgram(pics_program);
 	glBindTexture( GL_TEXTURE_2D, tex->buffer);
	
	if(width == 0 && height == 0){
    	width = tex->width;
    	height = tex->height;
    } else if(width == 0){
    	width = ((float)tex->width)/((float)tex->height) * height;
    } else if(height == 0){
    	height = ((float)tex->height)/((float)tex->width) * width;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Put the vertices position in an array
	float points[] = {
		pos.x, pos.y,
		pos.x, pos.y + height,
		pos.x + width, pos.y + height,
		pos.x + width, pos.y,
	};
	
	float tex_coord[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	
	//Load the vertices position into the GPU buffers
	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLint attrib = get_attrib(pics_program, "position");
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	GLuint tex_coord_vbo = 0;
	glGenBuffers(1, &tex_coord_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord), tex_coord, GL_STATIC_DRAW);
	GLint textcoord = get_attrib(pics_program, "texcoord");
	glEnableVertexAttribArray(textcoord);
	glVertexAttribPointer(textcoord, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	GLint uloc = get_uniform(pics_program, "projection");
	glm::mat4 projection = glm::ortho(0.0f, (float) window_width, (float) window_height, 0.0f);
	glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &points_vbo);
	glDeleteBuffers(1, &tex_coord_vbo);
    glDeleteBuffers(1, &ebo);
}

void Renderer::draw_cropped_bitmap(glm::vec2 pos, texture_t* tex, float scale, int width, int height){
	glUseProgram(pics_program);
 	glBindTexture( GL_TEXTURE_2D, tex->buffer);
	
	if(width == 0 && height == 0){
    	width = tex->width;
    	height = tex->height;
    } else if(width == 0){
    	width = ((float)tex->width)/((float)tex->height) * height;
    } else if(height == 0){
    	height = ((float)tex->height)/((float)tex->width) * width;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Put the vertices position in an array
	float points[] = {
		pos.x, pos.y,
		pos.x, pos.y + height,
		pos.x + width, pos.y + height,
		pos.x + width, pos.y,
	};
	
	float tex_y = ((float)height / ((float)tex->height * scale));
	float tex_x = ((float)width / ((float)tex->width * scale));
	
	float tex_coord[] = {
		0.0f, 0.0f,
		0.0f, tex_y,
		tex_x, tex_y,
		tex_x, 0.0f
	};

    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	
	//Load the vertices position into the GPU buffers
	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLint attrib = get_attrib(pics_program, "position");
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	GLuint tex_coord_vbo = 0;
	glGenBuffers(1, &tex_coord_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord), tex_coord, GL_STATIC_DRAW);
	GLint textcoord = get_attrib(pics_program, "texcoord");
	glEnableVertexAttribArray(textcoord);
	glVertexAttribPointer(textcoord, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	GLint uloc = get_uniform(pics_program, "projection");
	glm::mat4 projection = glm::ortho(0.0f, (float) window_width, (float) window_height, 0.0f);
	glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &points_vbo);
	glDeleteBuffers(1, &tex_coord_vbo);
    glDeleteBuffers(1, &ebo);
}

std::pair<int, int> Renderer::get_string_size(
	std::string text,
	int font_size
	){
	if(!Renderer::default_font) throw "No default font set";
	else
		return Renderer::get_string_size(text, font_size, Renderer::default_font);
}

/*
 * \brief Returns the size in pixels the string passed as parameters will take
 * \param text The text to be calculated
 * \param font_size The size of the font
 * \param font_location The path to the font
 * \return A pair (x, y) indicating the size of the string
 */
std::pair<int, int> Renderer::get_string_size(
	std::string text,
	int font_size,
	const char *font_location
	){
	
	std::map<GLchar, Character> *Characters;
	std::map<int, std::map<GLchar, Character> > *tmp;

	//FIXME
	if(fonts.find(font_location) == fonts.end())
		font_init(std::string(font_location), font_size);
	tmp = &fonts[font_location];
	if(tmp->find(font_size) == tmp->end())
		font_init(std::string(font_location), font_size);

	Characters = &(*tmp)[font_size];

	GLfloat w = 0, h = 0;
	GLfloat pipeSize = (*Characters)['|'].Size.y;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = (*Characters)[*c];

		w += (ch.Advance >> 6);

		if(ch.Size.y > h)
			h = ch.Size.y;
	}

	if(h < pipeSize)
		h = pipeSize;

	return std::pair<int, int>(w, h);
}

void Renderer::draw_text(
	std::string text,
	glm::vec2 position,
	glm::vec4 color,
	int font_size
	){
	if(!Renderer::default_font) throw "No default font set";
	else
		draw_text(text, position, color, font_size, Renderer::default_font);
}

/**
 * \brief Draws Text on screen
 * \param text The text to be drawn
 * \param position The position in screen coordinates
 * \param color The color of the line as a vector in 4
 *              dimensions(r, g, b, alpha as floats [0.0f - 1.0f]
 * \param font_size The size of the font
 * \param font_location the path to the font
 */
void Renderer::draw_text(
	std::string text,
	glm::vec2 position,
	glm::vec4 color,
	int font_size,
	const char *font_location
	){

	GLuint vbo, vao;
	std::map<GLchar, Character> *Characters;
	std::map<int, std::map<GLchar, Character> > *tmp;

	if(fonts.count(font_location) > 0)
		font_init(std::string(font_location), font_size);
	tmp = &fonts[font_location];
	if(tmp->count(font_size) > 0)
		font_init(std::string(font_location), font_size);

	Characters = &(*tmp)[font_size];

	// Activate corresponding render state
	glUseProgram(text_program);

	GLint attribute = get_attrib(text_program, "vertex");
	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(vao);

	GLint uniform = get_uniform(text_program, "textColor");
	glUniform3f(uniform, color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	std::string text2 = text + "|";

	std::pair<int, int> pos = get_string_size(text2, font_size, font_location);

	GLfloat x = position[0];
	GLfloat y = Renderer::get_window_height() -
	            position[1] +
	            get_lowest_character_hanging(text2, font_size, font_location) -
	            std::get<1>(pos);;

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = (*Characters)[*c];

		GLfloat xpos = x + ch.Bearing.x;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y);

		GLfloat w = ch.Size.x;
		GLfloat h = ch.Size.y;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6);                 // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vao);
	glDeleteVertexArrays(1, &vao);
}

void Renderer::render(){
	// Swap the buffers
	glfwSwapBuffers(window);
}

/**
 * \brief Returns a pointer to the GLFW window.
 * \return A pointer to the GLFW window.
 */
GLFWwindow *Renderer::get_window(){
	return Renderer::window;
}

void Renderer::set_cursor(int shape){
	if(Renderer::cursor_shape != shape){
		glfwDestroyCursor(cursor);
		Renderer::cursor_shape = shape;
		Renderer::cursor = glfwCreateStandardCursor(shape);
		glfwSetCursor(Renderer::get_window(), cursor);
	}
}

GLuint Renderer::create_shader(const char* shader, GLenum type){
	//Create the shader
	GLuint res = glCreateShader(type);
	glShaderSource(res, 1, &shader, NULL);
	glCompileShader(res);

	//Check for errors
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE) {
		fprintf(stderr, "Error creating shader: ");

		//Get the log size
		GLint log_length = 0;
		glGetShaderiv(res, GL_INFO_LOG_LENGTH, &log_length);
		//Print the log
		char *log = (char *) malloc(sizeof(char)*(log_length+1));
		glGetShaderInfoLog(res, log_length+1, &log_length,log);
		fprintf(stderr,"%s\n", log);

		//Delete the shader and quit
		glDeleteShader(res);
		return 0;
	}

	return res;
}

GLuint Renderer::create_program(
	const char *vertex_shader,
	const char *fragment_shader
	) {
	//Create an empty program
	GLuint program = glCreateProgram();
	GLuint shader;

	//Create the vertex shader
	if(vertex_shader) {
		shader = Renderer::create_shader(
			vertex_shader,
			GL_VERTEX_SHADER
			);
		if(!shader) {
			fprintf(stderr, "Vertex shader creation failed\n");
			return 0;
		}
		glAttachShader(program, shader);
	}

	//Create the fragment shader
	if(fragment_shader) {
		shader = Renderer::create_shader(
			fragment_shader,
			GL_FRAGMENT_SHADER
			);
		if(!shader) {
			fprintf(stderr, "Fragment shader creation failed\n");
			return 0;
		}
		glAttachShader(program, shader);
	}

	//Finalize the program
	glLinkProgram(program);

	//Check for errors
	GLint link_ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "Error creating program: ");

		//Get log size
		GLint log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		//Print the log
		char *log = (char *) malloc(sizeof(char)*(log_length+1));
		glGetProgramInfoLog(program, log_length+1, &log_length,log);
		fprintf(stderr, "%s\n", log);

		//Delete the program and quit
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

GLint Renderer::get_attrib(GLuint program, const char *name) {
	GLint attribute = glGetAttribLocation(program, name);
	if(attribute == -1)
		fprintf(stderr, "Could not bind attribute %s\n", name);
	return attribute;
}

GLint Renderer::get_uniform(GLuint program, const char *name) {
	GLint uniform = glGetUniformLocation(program, name);
	if(uniform == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", name);
	}
	return uniform;
}

void Renderer::freetype_init(){
	glm::mat4 projection = glm::ortho(
		0.0f,
		(float) Renderer::get_window_width(),
		0.0f,
		(float) Renderer::get_window_height()
		);

	glUseProgram(text_program);
	GLint uniform = get_uniform(text_program, "projection");
	glUniformMatrix4fv(
		uniform,
		1,
		GL_FALSE,
		glm::value_ptr(projection)
		);
}

void Renderer::font_init(std::string font_location, const int size){
	std::map<GLchar, Character> Characters;
	std::map<int, std::map<GLchar, Character> > *font_map = NULL;
	//If the font was already loaded quit, else initialize it.

	if(fonts.find(font_location) != fonts.end()) {
		font_map = &(fonts[font_location]);
	}
	if((font_map != NULL) && (font_map->find(size) != font_map->end()))
		return;

	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		fprintf(stderr, "ERROR::FREETYPE: Could not init FreeType Library\n");

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_location.c_str(), 0, &face))
		fprintf(stderr, "ERROR::FREETYPE: Failed to load font\n");

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, size);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			fprintf(stderr, "ERROR::FREETYTPE: Failed to load Glyph\n");
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int) face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	//Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	if(font_map == NULL)
		font_map = new std::map<int, std::map<GLchar, Character> >();
	(*font_map)[size] = Characters;
	fonts[font_location] = *font_map;
}

int Renderer::get_lowest_character_hanging(
	std::string text,
	int font_size,
	const char *font_location
	){

	std::map<GLchar, Character> *Characters;
	std::map<int, std::map<GLchar, Character> > *tmp;

	if(fonts.find(font_location) == fonts.end())
		font_init(font_location, font_size);
	tmp = &fonts[font_location];
	if(tmp->find(font_size) == tmp->end())
		font_init(font_location, font_size);

	Characters = &(*tmp)[font_size];

	GLfloat minpos = 0;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = (*Characters)[*c];

		if(ch.Size.y - ch.Bearing.y > minpos)
			minpos = ch.Size.y - ch.Bearing.y;
	}

	return minpos;
}
