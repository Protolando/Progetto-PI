#include "sidePanel.hpp"

SidePanel::SidePanel(int width, glm::vec4* color){
	int window_width = Context::window_width;
	if(window_width - width > window_width/3*2){
		if(window_width - width > 250) {
			SidePanel::width = 250;
		} else {
			SidePanel::width = window_width - width;
		}
	} else {
		SidePanel::width = window_width/3*2;
	}
	
	SidePanel::color = color;
	SidePanel::resizing = false;
}

SidePanel::~SidePanel(){
	if(SidePanel::color)
	delete SidePanel::color;
}
	
void SidePanel::setColor(glm::vec4* color){
	SidePanel::color = color;
}

void SidePanel::set_width(int width){
	SidePanel::width = width;
}
	
bool SidePanel::onMouseUp(GGClickEvent* evt){
	SidePanel::resizing = false;

	return GGView::onMouseUp(evt);
}

bool SidePanel::onMouseDown(GGClickEvent* evt){
	bool ret = GGView::onMouseDown(evt);

	if(!ret){
		int thisX = Context::window_width - SidePanel::width;
		
		if(evt->getX() > thisX -10 && evt->getX() < thisX + 10){
			SidePanel::resizing = true;
			ret = true;
		}
	}
	
	return ret;
}

void SidePanel::onMouseMove(GGMouseMoveEvent* evt){
	int window_width = Context::window_width;
	int thisX = window_width - SidePanel::width;
	double xpos = evt->getX();
	
	if((xpos > thisX - 10 && xpos < thisX + 10) || SidePanel::resizing){
		Context::setCursorToResize();
	} else {
		Context::setCursorToNormal();
	}
	
	if(SidePanel::resizing){
		//Always keep the toolsbar size between 2/3 of the window and 250
		if(window_width - xpos < window_width/3*2){
			if(window_width - xpos < 250) {
				SidePanel::width = 250;
			} else {
				SidePanel::width = window_width - xpos;
			}
		} else {
			SidePanel::width = window_width/3*2;
		}
	}
	
	GGView::onMouseMove(evt);
}

void SidePanel::onResize(GGResizeEvent* evt){
	GGView::onResize(evt);
}
	
void SidePanel::draw(){	
	Context::renderer->fill_rect(
		glm::vec2(Context::window_width - SidePanel::width, 0.0f),
		glm::vec2(Context::window_width, Context::window_height),
		*SidePanel::color
	);

	GGView::draw();
}

bool SidePanel::wasClicked(GGClickEvent* evt){
	return false;
}

int SidePanel::getWidth(){
	return SidePanel::width;
}

bool SidePanel::isResizing(){
	return SidePanel::resizing;
}
