#include "scrollbar.hpp"
#include "../colors.hpp"
#define CURSOR_SIZE (((int)ceil(Scrollbar::nelems/10)%2 == 0) ? \
		ceil(Scrollbar::nelems/10)+1 : \
		ceil(Scrollbar::nelems/10))
#define UNIT (Scrollbar::bigSize/(Scrollbar::nelems + CURSOR_SIZE))
#include <iostream>

Scrollbar::Scrollbar(glm::vec2 *position, int size, bool vertical, int nelems){
	Scrollbar::isVertical = vertical;
	Scrollbar::position = position;
	Scrollbar::bigSize = size;
	Scrollbar::smallSize = 40;
	Scrollbar::nelems = nelems;
	Scrollbar::cursorColor = new glm::vec4(BLACK);
	Scrollbar::bgColor = new glm::vec4(GREY);
	Scrollbar::selected = false;
	Scrollbar::cursorPos = 0;
}

Scrollbar::~Scrollbar(){
	delete Scrollbar::position;
	delete Scrollbar::cursorColor;
	delete Scrollbar::bgColor;
}
	
void Scrollbar::draw(){
	
	if(Scrollbar::isVertical){
		Context::renderer->fill_rect(
			*Scrollbar::position,
			Scrollbar::smallSize,
			Scrollbar::bigSize,
			*Scrollbar::bgColor
		);
		
		glm::vec2 cursorcoord = {
			Scrollbar::position->x,
			Scrollbar::position->y + (UNIT)*Scrollbar::cursorPos
		};
		
		Context::renderer->fill_rect(
			cursorcoord,
			Scrollbar::smallSize,
			CURSOR_SIZE*UNIT,
			*Scrollbar::cursorColor
		);
	} else {
		Context::renderer->fill_rect(
			*Scrollbar::position,
			bigSize,
			smallSize,
			*Scrollbar::bgColor
		);
		
		glm::vec2 cursorcoord = {
			Scrollbar::position->x + UNIT*Scrollbar::cursorPos,
			Scrollbar::position->y
		};
		
		Context::renderer->fill_rect(
			cursorcoord,
			CURSOR_SIZE*UNIT,
			Scrollbar::smallSize,
			*Scrollbar::cursorColor
		);
	}
	
	GGView::draw();
}

bool Scrollbar::wasClicked(GGClickEvent *evt){
	double x = evt->getX();
	double y = evt->getY();

	if(isVertical){
		return (
			x > Scrollbar::position->x &&
			x < Scrollbar::position->x + Scrollbar::smallSize &&
			y > Scrollbar::position->y &&
			y < Scrollbar::position->y + Scrollbar::bigSize
			);
	} else {
		return(
			x > Scrollbar::position->x &&
			x < Scrollbar::position->x + Scrollbar::bigSize &&
			y > Scrollbar::position->y &&
			y < Scrollbar::position->y + Scrollbar::smallSize
			);
	}
}

bool Scrollbar::onMouseDown(GGClickEvent *evt){
	if(GGView::onMouseDown(evt))
		return true;
	
	if(wasClicked(evt)){
		Scrollbar::selected = true;
		return true;
	}
	
	return false;
}

bool Scrollbar::onMouseUp(GGClickEvent *evt){
	Scrollbar::selected = false;
	return GGView::onMouseUp(evt);
}

void Scrollbar::onMouseMove(GGMouseMoveEvent *evt){
	GGView::onMouseMove(evt);

	if(!Scrollbar::selected)
		return;
	
	if(isVertical){
		if(evt->getY() < Scrollbar::position->y)
			Scrollbar::cursorPos = 0;
		else if(evt->getY() > Scrollbar::position->y + Scrollbar::bigSize)
			Scrollbar::cursorPos = Scrollbar::nelems;
		else{
			double y = evt->getY() - Scrollbar::position->y;
			
			Scrollbar::cursorPos = ceil(floor(y/UNIT) - CURSOR_SIZE/2);
			
			if(Scrollbar::cursorPos < 0)
				Scrollbar::cursorPos = 0;
			if(Scrollbar::cursorPos > Scrollbar::nelems)
				Scrollbar::cursorPos = nelems;
		}
	} else {
		if(evt->getX() < Scrollbar::position->x)
			Scrollbar::cursorPos = 0;
		else if(evt->getX() > Scrollbar::position->x + Scrollbar::bigSize)
			Scrollbar::cursorPos = Scrollbar::nelems;
		else{
			double x = evt->getX() - Scrollbar::position->x;
			
			Scrollbar::cursorPos = ceil(floor(x/UNIT) - CURSOR_SIZE/2);
			if(Scrollbar::cursorPos < 0)
				Scrollbar::cursorPos = 0;
			if(Scrollbar::cursorPos > Scrollbar::nelems) 
				Scrollbar::cursorPos = Scrollbar::nelems;
		}
	}
}

void Scrollbar::setCursorColor(glm::vec4 *color){
	if(Scrollbar::cursorColor)
		delete Scrollbar::cursorColor;

	Scrollbar::cursorColor = color;
}

void Scrollbar::setBgColor(glm::vec4 *color){
	if(Scrollbar::bgColor)
		delete Scrollbar::bgColor;

	Scrollbar::bgColor = color;
}
	
int Scrollbar::getCursorPos(){
	return cursorPos;
}

void Scrollbar::setThickness(int thickness){
	Scrollbar::smallSize = thickness;
}

void Scrollbar::setCursorPos(int pos){
	if(pos < 0)
		Scrollbar::cursorPos = 0;
	else if(pos > Scrollbar::nelems)
		Scrollbar::cursorPos = Scrollbar::nelems;
	else
		Scrollbar::cursorPos = pos;
}

void Scrollbar::setPosition(glm::vec2* pos){
	if(Scrollbar::position)
		delete Scrollbar::position;
	
	Scrollbar::position = pos;
}

void Scrollbar::setBigSize(int bigSize){
	Scrollbar::bigSize = bigSize;
}

int Scrollbar::getBigSize(){
	return Scrollbar::bigSize;
}

int Scrollbar::getSmallSize(){
	return Scrollbar::smallSize;
}

void Scrollbar::setSmallSize(int size){
	Scrollbar::smallSize = size;
}

void Scrollbar::setNelems(int nelems){
	if(nelems <= 0)
		Scrollbar::nelems = 0;
	else
		Scrollbar::nelems = nelems;
	
	if(Scrollbar::cursorPos > Scrollbar::nelems)
		Scrollbar::cursorPos = Scrollbar::nelems - 1;
	if(Scrollbar::cursorPos < 0)
		Scrollbar::cursorPos = 0;
}
