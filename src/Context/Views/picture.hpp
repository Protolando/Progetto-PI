#ifndef PICTUREINC
#define PICTUREINC

#include "GGView.hpp"
#include "../texture.hpp"
#include "../context.hpp"

class Picture : public GGView{
public :
	Picture(texture_t*, glm::vec2*, int, int);
	~Picture();
	
	int getWidth();
	int getHeight();
	
	void setPosition(glm::vec2*);
	texture_t* getTexture();
	
	void draw() override;
private :
	bool wasClicked(GGClickEvent*) override;
	texture_t* tex;
	int width;
	int height;
	glm::vec2* position;
};

#endif
