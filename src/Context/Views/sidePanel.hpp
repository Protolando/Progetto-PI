#ifndef SIDEPANELINC
#define SIDEPANELINC

#include "GGView.hpp"
#include "../context.hpp"
#include "../colors.hpp"

class SidePanel : public GGView{
public:
	SidePanel(int, glm::vec4*);
	~SidePanel();
	
	void setColor(glm::vec4*);
	void set_width(int);
	
	int getWidth();
	bool isResizing();
	
	bool onMouseUp(GGClickEvent*) override;
	bool onMouseDown(GGClickEvent*) override;
	void onMouseMove(GGMouseMoveEvent*) override;
	void onResize(GGResizeEvent*) override;
	
	void draw() override;
private:
	int width;
	bool resizing;
	glm::vec4* color = new glm::vec4(WHITE);
	bool wasClicked(GGClickEvent*) override;
};

#endif
