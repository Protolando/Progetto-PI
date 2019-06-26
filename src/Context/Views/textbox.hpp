#ifndef TEXTBOXINC
#define TEXTBOXINC

#include "GGView.hpp"
#include "../context.hpp"
#include <string>
#include <ctime>

//Class of single lined textbox
class Textbox : public GGView {
public : 
	Textbox(glm::vec2*, int, int);
	~Textbox();
	
	std::string getText();
	void resetText();
	void setText(std::string);
	
	void draw() override;
	bool onMouseDown(GGClickEvent*) override;
	bool onMouseUp(GGClickEvent*) override;
	bool onKeyUp(GGKeyboardEvent*) override;
	
	void displayError();
	void removeError();
	
	void setPosition(glm::vec2*);
	void setWidth(int);
	void setHeight(int);
	int getWidth();
private :
	int width;
	int height;
	bool isFocused;
	std::string text;
	glm::vec2* position;
	int cursorPos;
	bool wasClicked(GGClickEvent*) override;
	bool error;
};

#endif
