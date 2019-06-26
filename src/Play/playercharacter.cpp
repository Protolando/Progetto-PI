#include "playercharacter.hpp"
#include "../def.hpp"

#define SPRITE_WIDTH 20
#define SPRITE_HEIGHT 40

PlayerCharacter::PlayerCharacter() :
Character::Character(
	std::string(ASSETSPATH) + "/character.png",
	glm::vec2(Context::window_width/2, Context::window_height/2)
){
	PlayerCharacter::bullettex = 
		Context::renderer->generate_texture(
			std::string(std::string(ASSETSPATH) + std::string("/FriendBullet.png")).c_str()
		);
	Character::charspeed = 200;
	delete Character::hitbox;
	Character::hitbox = new Rect(Context::window_width/2, Context::window_height/2, SPRITE_WIDTH, SPRITE_HEIGHT);
}

void PlayerCharacter::moveUp(double time){
	if(Character::isAnimating())
		return;
	
	Character::spriteH = 2 + Character::nStep;
	Character::spriteV = 1;
	
	Character::nStep+=time*PlayerCharacter::animationSpeed;
	if(Character::nStep >= 2)
		Character::nStep = 0;
}

void PlayerCharacter::moveDown(double time){
	if(Character::isAnimating())
		return;

	Character::spriteH = 0 + Character::nStep;
	Character::spriteV = 1;
	
	Character::nStep+=time*PlayerCharacter::animationSpeed;
	if(Character::nStep >= 2)
		Character::nStep = 0;
}

void PlayerCharacter::moveLeft(double time){
	if(Character::isAnimating())
		return;
	
	Character::spriteH = 0 + Character::nStep;
	Character::spriteV = 2;
	
	Character::nStep+=time*PlayerCharacter::animationSpeed;
	if(Character::nStep >= 2)
		Character::nStep = 0;
}

void PlayerCharacter::moveRight(double time){
	if(Character::isAnimating())
		return;

	Character::spriteH = 2 + Character::nStep;
	Character::spriteV = 2;

	Character::nStep+=time*PlayerCharacter::animationSpeed;
	if(Character::nStep >= 2)
		Character::nStep = 0;
}

int PlayerCharacter::getSpeed(){
	return PlayerCharacter::charspeed;
}

int PlayerCharacter::getWidth(){
	return SPRITE_WIDTH;
}

int PlayerCharacter::getHeight(){
	return SPRITE_HEIGHT;
}

Bullet *PlayerCharacter::shoot(glm::mat4 transfMat, glm::vec2 target){
	glm::vec2 dir = target;
	dir.x = dir.x - (Character::getPosition().x + PlayerCharacter::getWidth()/2);
	dir.y = dir.y - (Character::getPosition().y  + PlayerCharacter::getHeight()/2);
	
	glm::vec2 yFromPlayerAsOrigin = {
		0.0f,
		Character::getPosition().y
	};
	
	float angle = glm::orientedAngle(glm::normalize(yFromPlayerAsOrigin), glm::normalize(dir));
	glm::vec4 pos = transfMat*glm::vec4(Character::getPosition(), 0.0f, 1.0f);
	
	Bullet *bullet = new Bullet(
		10,
		PlayerCharacter::getSpeed(),
		angle,
		{pos.x + PlayerCharacter::getWidth()/2, pos.y + PlayerCharacter::getHeight()/2},
		*PlayerCharacter::bullettex
	);
	
	Character::spriteH = (int) floor((angle + 3.14f + 3.14f/4.0f)/(3.14f/2.0f)) % 4;
	Character::spriteV = 3;
	
	Character::animationDuration = 0.1f;
	
	return bullet;
}

float PlayerCharacter::getFireRate(){
	return PlayerCharacter::firerate;
}

void PlayerCharacter::draw(glm::mat4){
	if(blink > 0.1f && blink < 0.2f)
		return;

	GLint pics_program = Context::getTexureProgram();
	glUseProgram(pics_program);
 	glBindTexture( GL_TEXTURE_2D, Character::image->buffer);

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
		(float) Character::spriteH /(float) 4.0f, (float) Character::spriteV / (float) 4.0f,
		(float)Character::spriteH /(float) 4.0f, ((float)Character::spriteV + 1.0f) / (float)  4.0f,
		((float)Character::spriteH + 1.0f) /(float) 4.0f, (float)(Character::spriteV + 1.0f) / (float)  4.0f,
		((float)Character::spriteH + 1.0f) /(float) 4.0f, (float)Character::spriteV / (float)  4.0f
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
	glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(Context::projection * Character::transformationMatrix));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &points_vbo);
	glDeleteBuffers(1, &tex_coord_vbo);
    glDeleteBuffers(1, &ebo);
}

void PlayerCharacter::update(double time){
	Character::animationDuration -= time;
	Character::invulTimer -= time;
	if(Character::isInvulnerable())
		PlayerCharacter::blink += time;
	else
		blink = 0.0f;

	if(blink > 0.2f)
		blink = 0.0f;
}
