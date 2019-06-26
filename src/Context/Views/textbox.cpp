#include "textbox.hpp"
#include "../colors.hpp"
#include <iostream>
#define TEXT_PADDING (Textbox::height/4)
#define BLINKDURATION 0.5f

Textbox::Textbox(glm::vec2* pos, int width, int height){
	Textbox::position = pos;
	Textbox::width = width;
	Textbox::height = height;
	Textbox::isFocused = false;
	Textbox::text = "";
	Textbox::cursorPos = 0;
	error = false;
}

Textbox::~Textbox(){
	delete Textbox::position;
}

void Textbox::draw(){
	Context::renderer->fill_rect(
		*(Textbox::position),
		glm::vec2(
			Textbox::position->x + Textbox::width,
			Textbox::position->y + Textbox::height
		),
		WHITE
	);
	
	
	if(error){
		Context::renderer->draw_rect(
			*(Textbox::position),
			glm::vec2(
				Textbox::position->x + Textbox::width,
				Textbox::position->y + Textbox::height
			),
			RED
		);
	}

	int i = 0;
	int textWidth = 0;
	int cursorX = 0;
	std::string printedText;
	
	//Trim the string from behind
	do {
		printedText = Textbox::text.substr(i, Textbox::cursorPos-i);
		textWidth = std::get<0>(Context::renderer->get_string_size(
					printedText,
					Textbox::height - TEXT_PADDING / 2
				));
		i++;
	} while(i <= Textbox::cursorPos && textWidth > Textbox::width - TEXT_PADDING / 2);
	
	i--;
	cursorX = textWidth + Textbox::position->x + TEXT_PADDING / 2;
	
	//Add the characters after the cursor
	int j = 0;
	do {
		printedText = Textbox::text.substr(i, Textbox::cursorPos - i + j);
		textWidth = std::get<0>(Context::renderer->get_string_size(
					printedText,
					Textbox::height - TEXT_PADDING / 2
				));
		j++;
	} while(Textbox::cursorPos + j <= (int) Textbox::text.size()
			&& textWidth < Textbox::width - TEXT_PADDING / 2);

	j--;
	printedText = Textbox::text.substr(i, Textbox::cursorPos - i + j);
	
	//Draw content
	Context::renderer->draw_text(
		printedText,
		glm::vec2(Textbox::position->x + TEXT_PADDING/2, Textbox::position->y + TEXT_PADDING/2),
		BLACK,
		Textbox::height - TEXT_PADDING/2
	);

	//Draw cursor
	if(Textbox::isFocused){
		//Make cursor blink every second
		if(glfwGetTime() < BLINKDURATION){
			Context::renderer->draw_line(
				{cursorX, Textbox::position->y + TEXT_PADDING/2},
				{cursorX, Textbox::position->y + Textbox::height - TEXT_PADDING/2},
				BLACK
			);
		} else if(glfwGetTime() > 2*BLINKDURATION){
			glfwSetTime(0.0f);
		}
	}
	
	GGView::draw();
}

std::string Textbox::getText(){
	return Textbox::text;
}
	
bool Textbox::onMouseDown(GGClickEvent* evt){
	Textbox::isFocused = false;
	Textbox::removeError();
	
	return GGView::onMouseDown(evt);
}

bool Textbox::onMouseUp(GGClickEvent* evt){
	bool ret = false;

	if(Textbox::wasClicked(evt)){
		Textbox::isFocused = true;
		glfwSetTime(0.0f);
		ret = true;
	} else {
		Textbox::isFocused = false;
		Textbox::removeError();
	}
	
	return GGView::onMouseDown(evt) || ret;
}

void Textbox::displayError(){
	error = true;
}

void Textbox::removeError(){
	error = false;
}

bool Textbox::onKeyUp(GGKeyboardEvent* evt){
	if(!Textbox::isFocused)
		return false;

	if(GGView::onKeyUp(evt)){
		return true;
	} else if(evt->getMods() == GLFW_MOD_CONTROL){
		if(evt->getKey() == GLFW_KEY_V){
				std::string tmp = Context::getClipboardContent();
				Textbox::text = Textbox::text.substr(0, Textbox::cursorPos) +
					tmp +
					Textbox::text.substr(
						Textbox::cursorPos,
						Textbox::text.size() -
						Textbox::cursorPos
					);
				Textbox::cursorPos += tmp.size();
				return true;
		}
		return false;
	} else if(evt->getType() == GG_EVENT_REGULAR){
		Textbox::text =
			Textbox::text.substr(0, Textbox::cursorPos) +
			(char) evt->getKey() +
			Textbox::text.substr(Textbox::cursorPos, Textbox::text.size());
		Textbox::cursorPos++;
		return true;
	} else if(evt->getType() == GG_EVENT_SPECIAL){
		if(evt->getKey() == GLFW_KEY_BACKSPACE && Textbox::cursorPos > 0){
			Textbox::text =
				Textbox::text.substr(0, Textbox::cursorPos-1) +
				Textbox::text.substr(Textbox::cursorPos, Textbox::text.size());
			Textbox::cursorPos--;
		} else if(evt->getKey() == GLFW_KEY_DELETE
				&& Textbox::cursorPos < (int) Textbox::text.size()){
			Textbox::text =
				Textbox::text.substr(0, Textbox::cursorPos) +
				Textbox::text.substr(Textbox::cursorPos + 1, Textbox::text.size());
		} else if(evt->getKey() == GLFW_KEY_LEFT && Textbox::cursorPos > 0){
			Textbox::cursorPos--;
		} else if(evt->getKey() == GLFW_KEY_RIGHT
					&& Textbox::cursorPos < (int) Textbox::text.size()){
			Textbox::cursorPos++;
		}
		
		return true;
	} else
		return false;
}

void Textbox::setPosition(glm::vec2* pos){
	delete Textbox::position;
	Textbox::position = pos;
}

void Textbox::setWidth(int width){
	Textbox::width = width;
}

void Textbox::setHeight(int height){
	Textbox::height = height;
}

bool Textbox::wasClicked(GGClickEvent* evt){
	double xpos = evt->getX();
	double ypos = evt->getY();
	
	return (xpos > (Textbox::position->x)
		 && xpos < (Textbox::position->x + Textbox::width)
		 && ypos > (Textbox::position->y)
		 && ypos < (Textbox::position->y + Textbox::height)
	);
}

void Textbox::resetText(){
	Textbox::text = std::string("");
	Textbox::cursorPos = 0;
}

int Textbox::getWidth(){
	return Textbox::width;
}

void Textbox::setText(std::string str){
	Textbox::text = str;
	Textbox::cursorPos = str.size();
}
