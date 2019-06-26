#include "slimeenemy.hpp"
#define SPRITE_WIDTH 40
#define SPRITE_HEIGHT 40
#include "../def.hpp"

SlimeEnemy::SlimeEnemy(std::string str, glm::vec2 startpos) : Character::Character(str, startpos){
	Character::charspeed = 100;
	delete Character::hitbox; 
	Character::hitbox = new Rect(startpos, SPRITE_WIDTH, SPRITE_HEIGHT);
	delete Character::hpbar;
	Character::hpbar = new HpBar(Character::maxHp, new glm::vec2(startpos), SPRITE_WIDTH, SPRITE_HEIGHT/10);
	
	SlimeEnemy::bullettex = Context::renderer->generate_texture(
			std::string(std::string(TEXPATH) + std::string("/slimebullet.png")).c_str()
	);
	Character::invulDuration = 0.2f;
}

SlimeEnemy::~SlimeEnemy(){}

void SlimeEnemy::draw(glm::mat4 view){
	//Context::renderer->draw_bitmap(glm::vec2(100.0f, 100.0f), Character::image, 1000, 1000);
	
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
		(float) Character::spriteH /(float) 4.0f, 0.0f,
		(float)Character::spriteH /(float) 4.0f, 1.0f,
		((float)Character::spriteH + 1.0f) /(float) 4.0f, 1.0f,
		((float)Character::spriteH + 1.0f) /(float) 4.0f, 0.0f
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
	glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(Context::projection * view * Character::transformationMatrix));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &points_vbo);
	glDeleteBuffers(1, &tex_coord_vbo);
    glDeleteBuffers(1, &ebo);
    
    glm::vec4 barpos = view * glm::vec4(SlimeEnemy::getPosition(), 0.0f, 1.0f);
    Character::hpbar->setPosition(new glm::vec2(barpos.x, barpos.y));
    Character::hpbar->draw();
}

void SlimeEnemy::moveUp(double time){
	double amount = Character::getStepSize(time);
	
	Character::transformationMatrix = glm::translate(
		Character::transformationMatrix, glm::vec3(0.0f, -amount, 0.0f)
	);
	
	Character::hitbox->translate(glm::vec2(0.0f, -amount));
	
	if(!SlimeEnemy::isAnimating()){
		Character::animationDuration = 0.2f;
		Character::animationDuration -= time;
		direction = 0;
	}
	
	Character::hpbar->setPosition(new glm::vec2(Character::getPosition()));
}

void SlimeEnemy::moveDown(double time){
	double amount = Character::getStepSize(time);
	Character::transformationMatrix = glm::translate(
		Character::transformationMatrix, glm::vec3(0.0f, amount, 0.0f)
	);
	
	Character::hitbox->translate(glm::vec2(0.0f, amount));
	
	if(!SlimeEnemy::isAnimating()){
		Character::animationDuration = 0.2f;
		Character::animationDuration -= time;
		direction = 2;
	}
	
	Character::hpbar->setPosition(new glm::vec2(Character::getPosition()));
}

void SlimeEnemy::moveLeft(double time){
	double amount = Character::getStepSize(time);
	Character::transformationMatrix = glm::translate(
		Character::transformationMatrix, glm::vec3(-amount, 0.0f, 0.0f)
	);

	Character::hitbox->translate(glm::vec2(-amount, 0.0f));
	
	if(!SlimeEnemy::isAnimating()){
		Character::animationDuration = 0.2f;
		Character::animationDuration -= time;
		direction = 1;
	}
	
	Character::hpbar->setPosition(new glm::vec2(Character::getPosition()));
}

void SlimeEnemy::moveRight(double time){
	double amount = Character::getStepSize(time);
	
	Character::transformationMatrix = glm::translate(
		Character::transformationMatrix, glm::vec3(amount, 0.0f, 0.0f)
	);
	
	Character::hitbox->translate(glm::vec2(amount, 0.0f));
	
	if(!SlimeEnemy::isAnimating()){
		Character::animationDuration = 0.2f;
		Character::animationDuration -= time;
		direction = 3;
	}
	
	Character::hpbar->setPosition(new glm::vec2(Character::getPosition()));
}

void SlimeEnemy::update(double time){
	const int idleframes = 10;
	Character::nStep += time*animationspeed;
	
	if(floor(Character::nStep) < idleframes)
		Character::spriteH = 0;
	else{
		Character::spriteH = floor(Character::nStep-idleframes);
	}
	
	if(Character::nStep > idleframes+3)
		Character::nStep = 0;
	
	if(Character::isAnimating()){
		switch(direction){
			case 0: SlimeEnemy::moveUp(time); break;
			case 1: SlimeEnemy::moveLeft(time); break;
			case 2: SlimeEnemy::moveDown(time); break;
			case 3: SlimeEnemy::moveRight(time); break;
		}
		
		Character::animationDuration -= time;
	}

	Character::invulTimer -= time;
}

int SlimeEnemy::getWidth(){
	return SPRITE_WIDTH;
}

int SlimeEnemy::getHeight(){
	return SPRITE_HEIGHT;
}

Bullet *SlimeEnemy::shoot(glm::mat4 transfMat, glm::vec2 target){
	glm::vec2 dir = target;
	dir.x = dir.x - (Character::getPosition().x + SlimeEnemy::getWidth()/2);
	dir.y = dir.y + 10 - (Character::getPosition().y  + SlimeEnemy::getHeight()/2);
	
	glm::vec2 yFromPlayerAsOrigin = {
		0.0f,
		Character::getPosition().y
	};
	
	float angle = glm::orientedAngle(glm::normalize(yFromPlayerAsOrigin), glm::normalize(dir));
	glm::vec4 pos = transfMat*glm::vec4(Character::getPosition(), 0.0f, 1.0f);
	
	Bullet *bullet = new Bullet(10, SlimeEnemy::getSpeed(), angle, {pos.x+SlimeEnemy::getWidth()/2, pos.y + SlimeEnemy::getHeight()/2}, *SlimeEnemy::bullettex);
	
	return bullet;
}
