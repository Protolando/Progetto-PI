#include "lable.hpp"

Lable::Lable(std::string* text, int size, glm::vec4* color, glm::vec2* pos){
	Lable::text = text;
	Lable::size = size;
	Lable::color = color;
	Lable::position = pos;
}

Lable::Lable(const char* text, int size, glm::vec4* color, glm::vec2* pos){
	Lable::text = new std::string(text);
	Lable::size = size;
	Lable::color = color;
	Lable::position = pos;
}

Lable::~Lable(){
	delete Lable::text;
	delete Lable::color;
	delete Lable::position;
}

void Lable::setText(std::string* text){
	if(Lable::text) delete Lable::text;
	Lable::text = text;
}

void Lable::setText(const char* text){
	Lable::setText(new std::string(text));
}

void Lable::setSize(int size){
	Lable::size = size;
}

std::string* Lable::getText(){
	return Lable::text;
}

void Lable::setColor(glm::vec4* color){
	if(Lable::color) delete Lable::color;
	Lable::color = color;
}

void Lable::setPosition(glm::vec2* pos){
	if(Lable::position != NULL && Lable::position != pos)
		delete Lable::position;
	Lable::position = pos;
}

int Lable::getWidth(){
	std::pair<int, int> text_size = Context::renderer->get_string_size(*Lable::text, Lable::size);
	
	return std::get<0>(text_size); 
}

void Lable::draw(){
	Context::renderer->draw_text(*Lable::text, *Lable::position, *Lable::color, Lable::size);
}

bool Lable::wasClicked(GGClickEvent* evt){
	return false;
}

glm::vec2 Lable::getPosition(){
	return *(Lable::position);
}

int Lable::getHeight(){
	return Lable::size;
}
