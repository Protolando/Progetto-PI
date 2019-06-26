#ifndef SCROLLBARINC
#define SCROLLBARINC

#include "GGView.hpp"
#include "../context.hpp"

class Scrollbar : public GGView{
public : 
	Scrollbar(glm::vec2 *, int, bool, int);
	~Scrollbar();
	
	void draw() override;
	
	bool onMouseDown(GGClickEvent *) override;
	bool onMouseUp(GGClickEvent *) override;
		
	void onMouseMove(GGMouseMoveEvent *) override;
	void setCursorColor(glm::vec4 *);
	void setBgColor(glm::vec4 *);
	
	void setThickness(int);
	void setCursorPos(int);
	void setPosition(glm::vec2*);
	void setBigSize(int);
	void setSmallSize(int);
	void setNelems(int);
	int getBigSize();
	int getSmallSize();
	
	int getCursorPos();
private : 
	glm::vec2* position;
	int bigSize;
	int smallSize;
	int cursorSize;
	bool isVertical;
	int nelems;
	int cursorPos;
	glm::vec4 *cursorColor;
	glm::vec4 *bgColor;
	bool selected;
	bool wasClicked(GGClickEvent *) override;
};

#endif
