#ifndef SLIMEENEMYINC
#define SLIMEENEMYINC

#include "character.hpp"

class SlimeEnemy : public Character{
public:
	SlimeEnemy(std::string, glm::vec2);
	~SlimeEnemy();
	
	void draw(glm::mat4) override;
	void moveUp(double time) override;
	void moveDown(double time) override;
	void moveLeft(double time) override;
	void moveRight(double time) override;
	void update(double time) override;
	
	Bullet *shoot(glm::mat4, glm::vec2) override;
	
	int getWidth() override;
	int getHeight() override;
	
private:
	const float animationspeed = 6.0f;
	int direction;
	texture_t *bullettex;
};

#endif
