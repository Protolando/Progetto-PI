#include "dropdown.hpp"
#include "../colors.hpp"
#define DROPDOWN_LABLE_PADDING 5
#include <iostream>
Dropdown::Dropdown(glm::vec2 *position, int width, int height){
	Dropdown::position = position;
	Dropdown::width = width;
	Dropdown::height = height;
	Dropdown::active = true;
	Dropdown::expanded = false;
	Dropdown::currentChoice = NULL;
}

Dropdown::~Dropdown(){
	for(std::vector<Lable *>::iterator i = lables.begin(); i != lables.end(); i++)
		delete *i;
		
	delete Dropdown::position;
}
	
void Dropdown::draw(){
	if(active){
		if(!expanded){
			Context::renderer->fill_rect(
				*Dropdown::position,
				Dropdown::width,
				Dropdown::height,
				glm::vec4(WHITE)
			);
			
			Context::renderer->draw_rect(
				*Dropdown::position,
				glm::vec2(
					Dropdown::position->x + Dropdown::width,
					Dropdown::position->y + Dropdown::height
				),
				glm::vec4(BLACK)
			);
			
			if(Dropdown::currentChoice){
				Dropdown::currentChoice->draw();
			}
		} else {
			Context::renderer->fill_rect(
				*Dropdown::position,
				Dropdown::width,
				Dropdown::height * (Dropdown::lables.size() + 1),
				glm::vec4(WHITE)
			);
			
			Context::renderer->draw_rect(
				*Dropdown::position,
				glm::vec2(
					Dropdown::position->x + Dropdown::width,
					Dropdown::position->y + Dropdown::height * (Dropdown::lables.size() + 1)
				),
				glm::vec4(BLACK)
			);
			
			for(auto i = Dropdown::lables.begin(); i != Dropdown::lables.end(); i++){
				(*i)->draw();
				Context::renderer->draw_line(
					glm::vec2(
						Dropdown::position->x,
						(*i)->getPosition().y - DROPDOWN_LABLE_PADDING
					),
					glm::vec2(
						Dropdown::position->x + Dropdown::width,
						(*i)->getPosition().y - DROPDOWN_LABLE_PADDING
					),
					glm::vec4(BLACK)
				);
			}
		}
	} else {
		Context::renderer->fill_rect(
			*Dropdown::position,
			Dropdown::width,
			Dropdown::height,
			glm::vec4(GREY)
		);
		
		Context::renderer->draw_rect(
			*Dropdown::position,
			glm::vec2(
				Dropdown::position->x + Dropdown::width,
				Dropdown::position->y + Dropdown::height
			),
			glm::vec4(BLACK)
		);
	}
	
	GGView::draw();
}
	
void Dropdown::addElement(std::string *name){
	Dropdown::lables.push_back(
		new Lable(
			name,
			Dropdown::height - 2*DROPDOWN_LABLE_PADDING,
			new glm::vec4(BLACK),
			new glm::vec2(
				Dropdown::position->x + DROPDOWN_LABLE_PADDING,
				Dropdown::position->y + DROPDOWN_LABLE_PADDING
			)
		)
	);
}

void Dropdown::activate(){
	Dropdown::active = true;
}

void Dropdown::deactivate(){
	Dropdown::active = false;
	Dropdown::expanded = false;
}

bool Dropdown::onMouseUp(GGClickEvent *evt){
	bool res = GGView::onMouseUp(evt);
	
	if(!Dropdown::active)
		return false || res;

	if(Dropdown::wasClicked(evt)){
		if(!Dropdown::expanded){
			Dropdown::expanded = true;
			Dropdown::placeItems();
			if(Dropdown::currentChoice)
				Dropdown::currentChoice->setColor(new glm::vec4(RED));
		} else if(Dropdown::expanded){
			if(Dropdown::currentChoice)
				Dropdown::currentChoice->setColor(new glm::vec4(BLACK));
			int n = (int) ceil((evt->getY() - Dropdown::position->y)/Dropdown::height) - 2;
			if(n >= 0)
				Dropdown::currentChoice = Dropdown::lables.at(n);
			else
				Dropdown::currentChoice = NULL;
			Dropdown::expanded = false;
			Dropdown::placeItems();
			if(Dropdown::currentChoice)
				Dropdown::currentChoice->setColor(new glm::vec4(BLACK));
		}
		return true || res;
	} else {
		Dropdown::expanded = false;
		return false || res;
	}
}

bool Dropdown::onMouseDown(GGClickEvent *evt){
	if(!Dropdown::wasClicked(evt)){	
		Dropdown::expanded = false;
		if(Dropdown::currentChoice)
			Dropdown::currentChoice->setColor(new glm::vec4(BLACK));
	}
	
	return GGView::onMouseDown(evt);
}

bool Dropdown::wasClicked(GGClickEvent *evt){
	double xpos = evt->getX();
	double ypos = evt->getY();
	
	int height;
	
	if(!Dropdown::expanded){
		height = Dropdown::height;
	} else {
		height = Dropdown::height * (Dropdown::lables.size() + 1);
	}
	
	return (
		xpos > Dropdown::position->x &&
		xpos < Dropdown::position->x + Dropdown::width &&
		ypos > Dropdown::position->y &&
		ypos < Dropdown::position->y + height
	);	
}

void Dropdown::placeItems(){
	if(Dropdown::expanded){
		for(int i = 0; i < (int) Dropdown::lables.size(); i++){
			Dropdown::lables.at(i)->setPosition(
				new glm::vec2(
					Dropdown::position->x + DROPDOWN_LABLE_PADDING,
					Dropdown::position->y + Dropdown::height*(i + 1)  + DROPDOWN_LABLE_PADDING
				)
			);
		}
	} else {
		if(Dropdown::currentChoice){
			Dropdown::currentChoice->setPosition(
				new glm::vec2(
					Dropdown::position->x + DROPDOWN_LABLE_PADDING,
					Dropdown::position->y + DROPDOWN_LABLE_PADDING
				)
			);
		}
	}
}

void Dropdown::setPosition(glm::vec2 *pos){
	if(Dropdown::position)
		delete Dropdown::position;
	Dropdown::position = pos;
	Dropdown::placeItems();
}
void Dropdown::setWidth(int w){
	Dropdown::width = w;
}

int Dropdown::getWidth(){
	return Dropdown::width;
}

std::string Dropdown::getCurrentChoice(){
	if(Dropdown::currentChoice)
		return *Dropdown::currentChoice->getText();
	else
		return std::string("");
}

bool Dropdown::was_clicked(GGClickEvent *evt){
	return Dropdown::wasClicked(evt);
}

bool Dropdown::isExpanded(){
	return expanded;
}

bool Dropdown::isActive(){
	return active;
}
