#include "bullet.hpp"

#define SPRITE_WIDTH 20
#define SPRITE_HEIGHT 20

Bullet::Bullet(int damage, double startspeed, float theta, glm::vec2 pos, texture_t image){
	Bullet::damage = damage;
	Bullet::speed += startspeed;
	Bullet::transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 1.0f));	
	Bullet::transformMatrix = glm::rotate(Bullet::transformMatrix, theta, glm::vec3(0.0f, 0.0f, 1.0f));	
	Bullet::image = (texture_t *) malloc(sizeof(texture_t));
	Bullet::image->path = image.path;
	Bullet::image->width = image.width;
	Bullet::image->height = image.height;
	Bullet::image->buffer = image.buffer;
	Bullet::hitbox = new Rect(pos, SPRITE_WIDTH, SPRITE_HEIGHT);
	Bullet::hitbox->rotate(theta);
}

Bullet::Bullet(int damage, double startspeed, float theta, glm::vec2 pos, const char* textPath){
	Bullet::damage = damage;
	Bullet::speed += startspeed;
	Bullet::transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 1.0f));	
	Bullet::transformMatrix = glm::rotate(Bullet::transformMatrix, theta, glm::vec3(0.0f, 0.0f, 1.0f));	
	Bullet::image = Context::renderer->generate_texture(textPath);
	Bullet::hitbox = new Rect(pos, SPRITE_WIDTH, SPRITE_HEIGHT);
	Bullet::hitbox->rotate(theta);
}

Bullet::~Bullet(){
	delete Bullet::hitbox;
	free(Bullet::image);
}

int Bullet::getDamage(){
	return Bullet::damage;
}

void Bullet::draw(glm::mat4 view){
	GLint pics_program = Context::getTexureProgram();
	glUseProgram(pics_program);
 	glBindTexture( GL_TEXTURE_2D, Bullet::image->buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float width = SPRITE_WIDTH;
	float height = SPRITE_HEIGHT;

    //Put the vertices position in an array
	float points[] = {
		0.0f, 0.0f,
		0.0f, 0.0f + height,
		0.0f + width, 0.0f + height,
		0.0f + width, 0.0f,
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

	GLint attrib = Context::renderer->get_attrib(pics_program, "position");
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	GLuint tex_coord_vbo = 0;
	glGenBuffers(1, &tex_coord_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord), tex_coord, GL_STATIC_DRAW);
	GLint textcoord = Context::renderer->get_attrib(pics_program, "texcoord");
	glEnableVertexAttribArray(textcoord);
	glVertexAttribPointer(textcoord, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);

	GLint uloc = Context::renderer->get_uniform(pics_program, "projection");
	glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(Context::projection * view * Bullet::transformMatrix));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &points_vbo);
	glDeleteBuffers(1, &tex_coord_vbo);
    glDeleteBuffers(1, &ebo);
}

void Bullet::move(double time){
	Bullet::speed += Bullet::acceler*time;

	Bullet::transformMatrix = glm::translate(
		Bullet::transformMatrix,
		glm::vec3(
			0.0f,
			speed*time,
			0.0f
		)
	);
	
	Bullet::hitbox->translate(glm::vec2(0.0f, speed*time));
}

glm::vec2 Bullet::getPosition(){
	glm::vec4 tmp = Bullet::transformMatrix*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return glm::vec2(tmp.x, tmp.y);
}

int Bullet::getWidth(){
	return SPRITE_WIDTH;
}

int Bullet::getHeight(){
	return SPRITE_HEIGHT;
}

Rect Bullet::getHitbox(){
	return *Bullet::hitbox;
}
