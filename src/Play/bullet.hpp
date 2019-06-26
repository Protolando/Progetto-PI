#ifndef BULLETINC
#define BULLETINC

#include "../Context/context.hpp"
#include "../Context/rect.hpp"

class Bullet{
public:
	Bullet(int damage, double startspeed, float theta, glm::vec2 pos, const char *textPath);
	Bullet(int damage, double startspeed, float theta, glm::vec2 pos, texture_t);
	~Bullet();
	int getDamage();
	void draw(glm::mat4);
	void move(double time);
	glm::vec2 getPosition();
	int getWidth();
	int getHeight();
	Rect getHitbox();
private:
	int damage;
	int acceler = 600;
	glm::mat4 transformMatrix;
	texture_t *image;
	double speed = 5.0f;
	Rect *hitbox;
};

#endif
