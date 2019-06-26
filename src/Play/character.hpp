#ifndef CHARACTERINC
#define CHARACTERINC

#include "../Context/context.hpp"
#include "../Context/rect.hpp"
#include "hpbar.hpp"
#include "bullet.hpp"
#include "glm/gtx/vector_angle.hpp"

class Character{
public : 
	Character(std::string, glm::vec2);
	virtual ~Character();
	
	virtual void draw(glm::mat4) = 0;
	
	virtual void moveUp(double time);
	virtual void moveDown(double time);
	virtual void moveLeft(double time);
	virtual void moveRight(double time);
	virtual void update(double time) = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	bool isAnimating();
	Rect getHitbox();
	
	double getStepSize(double time);
	
	glm::vec2 getPosition();
	void setPosition(glm::vec2);
	bool isInvulnerable();
	
	void changeHp(int);
	void setHpToMax();
	int getCurrentHp();
	bool isDead();
	virtual Bullet *shoot(glm::mat4, glm::vec2) = 0;
	static const int maxHp = 100;
	double invulDuration = 1.0f;
private :
	int direction; /*Direction: 0 = UP, 1 = left, 2 = down, 3 = right*/
protected :
	virtual int getSpeed();
	texture_t *image;
	glm::mat4 transformationMatrix;
	float nStep;
	int spriteH;
	int spriteV;
	int charspeed = 200;
	double animationDuration;
	Rect *hitbox;
	int currentHp;
	double invulTimer = 0.0f;
	HpBar *hpbar;
};

#endif
