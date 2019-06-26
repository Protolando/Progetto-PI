#ifndef RECTINC
#define RECTINC

#include <glm/glm.hpp>

class Rect{
public:
	Rect(float, float, float, float);
	Rect(glm::vec2 *, float, float);
	Rect(glm::vec2, float, float);
	~Rect();
	float getWidth();
	float getHeight();
	glm::vec2 getTopLeft();
	glm::vec2 getTopRight();
	glm::vec2 getBotLeft();
	glm::vec2 getBotRight();
	void rotate(float);
	void translate(glm::vec2);
	void transform(glm::mat4);
	static bool intersect(Rect, Rect);
	static bool contains(Rect, glm::vec2);
	Rect clone();
private:
	float width;
	float height;
	glm::mat4 transformMatrix;
};

#endif
