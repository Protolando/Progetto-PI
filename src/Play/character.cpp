#include "character.hpp"

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 64

Character::Character(std::string sprite, glm::vec2 startpos){
	Character::nStep = 0;
	Character::image = Context::renderer->generate_texture(sprite.c_str());
	Character::transformationMatrix = glm::translate(
		glm::mat4(1.0f), glm::vec3(startpos, 0.0f)
	);
	Character::spriteH = 0;
	Character::spriteV = 0;
	Character::currentHp = Character::maxHp;
	Character::hitbox = new Rect(0.0f, 0.0f, SPRITE_WIDTH, SPRITE_HEIGHT);
	
	Character::hpbar = new HpBar(Character::maxHp, new glm::vec2(startpos), SPRITE_WIDTH, SPRITE_HEIGHT/10);
	Character::invulTimer = 0.0f;
}

Character::~Character(){
	delete Character::hitbox;
	free(Character::image);
}

void Character::moveUp(double time){
	double amount = Character::getStepSize(time);
	
	Character::transformationMatrix = glm::translate(
		Character::transformationMatrix, glm::vec3(0.0f, -amount, 0.0f)
	);
	
	Character::hitbox->translate(glm::vec2(0.0f, amount));
	
	Character::spriteH = 2 + Character::nStep;
	Character::spriteV = 1;
	
	Character::nStep+=time;
	if(Character::nStep >= 2)
		Character::nStep = 0;
	
	Character::hpbar->setPosition(new glm::vec2(Character::getPosition()));
}

void Character::moveDown(double time){
	double amount = Character::getStepSize(time);
	Character::transformationMatrix = glm::translate(
		Character::transformationMatrix, glm::vec3(0.0f, amount, 0.0f)
	);
	
	Character::hitbox->translate(glm::vec2(0.0f, amount));
	
	Character::spriteH = 0 + Character::nStep;
	Character::spriteV = 1;
	
	Character::nStep+=time;
	if(Character::nStep >= 2)
		Character::nStep = 0;
	
	Character::hpbar->setPosition(new glm::vec2(Character::getPosition()));
}

void Character::moveLeft(double time){
	double amount = Character::getStepSize(time);
	Character::transformationMatrix = glm::translate(
		Character::transformationMatrix, glm::vec3(-amount, 0.0f, 0.0f)
	);
	
	Character::hitbox->translate(glm::vec2(-amount, 0.0f));
	
	Character::spriteH = 0 + Character::nStep;
	Character::spriteV = 2;
	
	Character::nStep+=time;
	if(Character::nStep >= 2)
		Character::nStep = 0;
	
	Character::hpbar->setPosition(new glm::vec2(Character::getPosition()));
}

void Character::moveRight(double time){
	double amount = Character::getStepSize(time);
	
	Character::transformationMatrix = glm::translate(
		Character::transformationMatrix, glm::vec3(amount, 0.0f, 0.0f)
	);

	Character::hitbox->translate(glm::vec2(amount, 0.0f));
	
	Character::spriteH = 2 + Character::nStep;
	Character::spriteV = 2;
	
	Character::nStep+=time;
	if(Character::nStep >= 2)
		Character::nStep = 0;
		
	Character::hpbar->setPosition(new glm::vec2(Character::getPosition()));
}

glm::vec2 Character::getPosition(){
	glm::vec4 tmp = Character::transformationMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return glm::vec2(tmp.x, tmp.y);
}

double Character::getStepSize(double time){
	return time*Character::getSpeed();
}

void Character::setPosition(glm::vec2 pos){
	glm::vec2 currPos = Character::getPosition();

	glm::vec2 amount(
			pos.x - currPos.x,
			pos.y - currPos.y
	);

	Character::transformationMatrix = glm::translate(
		Character::transformationMatrix, glm::vec3(
			amount,
			0.0f
		)
	);
	
	Character::hitbox->translate(amount);
	Character::hpbar->setPosition(new glm::vec2(currPos));
}

int Character::getSpeed(){
	return Character::charspeed;
}

int Character::getWidth(){
	return SPRITE_WIDTH;
}

int Character::getHeight(){
	return SPRITE_HEIGHT;
}

bool Character::isAnimating(){
	return Character::animationDuration > 0;
}

Rect Character::getHitbox(){
	return *Character::hitbox;
}

bool Character::isInvulnerable(){
	return Character::invulTimer > 0;
}

void Character::changeHp(int amount){
	if(Character::isInvulnerable())
		return;
	
	if(Character::currentHp + amount > Character::maxHp)
		Character::currentHp = Character::maxHp;
	else{
		Character::currentHp += amount;
		Character::hpbar->changeHp(amount);
	}

	if(amount < 0) //Player took damage
		Character::invulTimer = Character::invulDuration;
}

void Character::setHpToMax(){
	Character::currentHp = Character::maxHp;
	Character::hpbar->setHp(Character::maxHp);
}

int Character::getCurrentHp(){
	return Character::currentHp;
}

bool Character::isDead(){
	return Character::currentHp <= 0;
}
