#include "button.hpp"
#include <iostream>
#include "../colors.hpp"
#define TEXT_PADDING (Button::height/10)

Button::Button(glm::vec2* pos, int width, int height) : 
	Button(NULL, pos, width, height){}

Button::Button(std::string* lable, glm::vec2* pos, int height) : 
Button(lable, pos, 0, height){}

void Button::changeColorMouseDown(GGClickEvent* evt){
	Button::currentColor = Button::clickedColor;
}

Button::Button(std::string* lable, glm::vec2* pos, int width, int height){
	Button::position = pos;
	Button::width = width;
	Button::height = height;
	Button::bgColor = new glm::vec4(244.0f/255.0f, 66.0f/255.0f, 241.0f/255.0f, 1.0f);
	Button::currentColor = bgColor;
	Button::clickedColor = NULL;
	Button::adaptClickedColor();
	Button::lable = NULL;

	if(lable)
		Button::setLable(lable);
}

Button::~Button(){
	delete Button::bgColor;
	delete Button::clickedColor;
	delete Button::position;
	if(Button::lable)
		delete Button::lable;
}

void Button::setLable(std::string* lable){
	int fontSize = Button::height - (TEXT_PADDING);
	
	glm::vec2* lablepos = new glm::vec2(
		Button::position->x + (TEXT_PADDING)/2,
		Button::position->y + (TEXT_PADDING)/2
	);

	Lable* newLab = new Lable(lable, fontSize, new glm::vec4(BLACK), lablepos);
	Button::width = newLab->getWidth() + (TEXT_PADDING);
	
	if(Button::lable){
		GGView::removeView(Button::lable);
		delete Button::lable;
	}
	
	Button::lable = newLab;
	Button::addView(newLab);
	Button::setHeight(fontSize + (TEXT_PADDING));
}

void Button::setLable(const char* lable){
	Button::setLable(new std::string(lable));
}

void Button::resetLable(){
	int fontSize = Button::height - (TEXT_PADDING);
	
	glm::vec2* lablepos = new glm::vec2(
		Button::position->x + (TEXT_PADDING)/2,
		Button::position->y + (TEXT_PADDING)/2
	);
	
	Button::lable->setPosition(lablepos);
	Button::lable->setSize(fontSize);
}

void Button::resetWidth(){
	Button::width = Button::lable->getWidth() + (TEXT_PADDING);
}

void Button::setWidth(int width){
	Button::width = width;
}

void Button::setHeight(int height){
	Button::height = height;
	Button::resetLable();
}

void Button::setSize(int width, int height){
	Button::width = width;
	Button::height = height;
	Button::resetLable();
}

void Button::setPosition(glm::vec2* pos){
	delete Button::position;
	Button::position = pos;
	Button::resetLable();
}

void Button::setColor(glm::vec4* color){
	if(Button::currentColor == Button::bgColor)
		Button::currentColor = color;
	delete Button::bgColor;
	Button::bgColor = color;
}

void Button::adaptClickedColor(){
	const float amount = 20.0f/255.0f;
	Button::setClickedColor(new glm::vec4(
		(Button::bgColor->x-amount) < 0? 0 : Button::bgColor->x-amount,
		(Button::bgColor->y-amount) < 0? 0 : Button::bgColor->y-amount,
		(Button::bgColor->z-amount) < 0? 0 : Button::bgColor->z-amount,
		1.0f
	));
}

void Button::setClickedColor(glm::vec4* color){
	if(Button::clickedColor){
		if(Button::currentColor == Button::clickedColor)
			Button::currentColor = color;
		delete Button::clickedColor;
	}
		
	Button::clickedColor = color;
}

void Button::setTextColor(glm::vec4* color){
	Button::lable->setColor(color);
}

void Button::draw(){
	Context::renderer->fill_rect(
		*Button::position,
		Button::width,
		Button::height,
		*Button::currentColor
	);
	
	GGView::draw();
}

Lable* Button::getLable(){
	return Button::lable;
}

bool Button::onMouseDown(GGClickEvent* evt){
	GGView::onMouseDown(evt);
	
	if(wasClicked(evt)){
		Button::currentColor = Button::clickedColor;
		return true;
	} else
		return false;
}

bool Button::onMouseUp(GGClickEvent* evt){
	GGView::onMouseUp(evt);
	Button::currentColor = Button::bgColor;
	
	if(wasClicked(evt)){
		return true;
	} else
		return false;
}

void Button::onResize(GGResizeEvent*){
	Button::resetLable();
	Button::resetWidth();
}

bool Button::wasClicked(GGClickEvent* evt){
	double x = evt->getX();
	double y = evt->getY();

	if( x > Button::position->x &&
	    x < Button::position->x + Button::width &&
	    y > Button::position->y &&
	    y < Button::position->y + Button::height
	    ) {
		return true;
	}
	else return false;
}

int Button::getWidth(){
	return Button::width;
}
