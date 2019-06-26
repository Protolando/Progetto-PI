#ifndef buttoninc
#define buttoninc

#include "GGView.hpp"
#include <string>
#include "../context.hpp"
#include "lable.hpp"

class Button : public GGView {
public:
	Button(glm::vec2*, int, int);
	Button(std::string*, glm::vec2*, int);
	Button(std::string*, glm::vec2*, int, int);
	void setLable(std::string*);
	void setLable(const char*);
	void setWidth(int);
	void setHeight(int);
	void setSize(int, int);
	void setPosition(glm::vec2*);
	void setColor(glm::vec4*);
	void adaptClickedColor();
	void setClickedColor(glm::vec4*);
	void setTextColor(glm::vec4*);
	Lable* getLable();
	int getWidth();
	void draw() override;
	bool onMouseDown(GGClickEvent*) override;
	bool onMouseUp(GGClickEvent*) override;
	void onResize(GGResizeEvent*) override;
	void resetWidth();
	~Button();
private:
	glm::vec4* currentColor;
	glm::vec4* bgColor;
	glm::vec4* clickedColor;
	glm::vec2* position;
	int width;
	int height;
	Lable* lable;
	bool wasClicked(GGClickEvent*) override;
	void resetLable();
	void changeColorMouseDown(GGClickEvent*);
};

#endif
