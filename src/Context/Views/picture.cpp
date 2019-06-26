#include "picture.hpp"
#include <stdlib.h>

Picture::Picture(texture_t* tex, glm::vec2* pos, int width, int height){
	Picture::tex = tex;
	Picture::position = pos;

	if(width == 0){
		Picture::width = (float)tex->width / (float)tex->height * height;
		Picture::height = height;
	} else if(height == 0){
		Picture::width = width;
		Picture::height = (float)tex->height / (float)tex->width * height;
	}
}

Picture::~Picture(){
	free(Picture::tex);
	delete Picture::position;
}
	
int Picture::getWidth(){
	return Picture::width;
}
int Picture::getHeight(){
	return Picture::height;
}

void Picture::draw(){
	Context::renderer->draw_bitmap(
		*Picture::position,
		Picture::tex,
		Picture::width,
		Picture::height
	);
	
	GGView::draw();
}

void Picture::setPosition(glm::vec2* pos){
	if(Picture::position != NULL && Picture::position != pos)
		delete Picture::position;
	Picture::position = pos;
}

bool Picture::wasClicked(GGClickEvent* evt){
	return ((evt->getX() > Picture::position->x) &&
		(evt->getX() < Picture::position->x + Picture::width) &&
		(evt->getY() > Picture::position->y) &&
		(evt->getY() < Picture::position->y + Picture::height)
	);
}

texture_t* Picture::getTexture(){
	return Picture::tex;
}
