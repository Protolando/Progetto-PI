#ifndef PLAYERCHARACTERINC
#define PLAYERCHARACTERINC

#include "character.hpp"
#include "bullet.hpp"

class PlayerCharacter : public Character{
public :
	PlayerCharacter();
	void moveUp(double time) override;
	void moveDown(double time) override;
	void moveLeft(double time) override;
	void moveRight(double time) override;
	void update(double time) override;
	int getSpeed() override;
	int getWidth() override;
	int getHeight() override;
	void draw(glm::mat4) override;
	Bullet *shoot(glm::mat4, glm::vec2) override;
	float getFireRate();
private :
	const int charspeed = 200;
	const int animationSpeed = 10;
	const float firerate = 5.0f;// bullets/sec
	texture_t *bullettex;
	float blink = 0.0f;
};

#endif
