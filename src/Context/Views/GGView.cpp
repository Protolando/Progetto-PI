#include "GGView.hpp"
#include <iterator>

void GGView::addView(GGView* view){
	GGView::views.push_back(view);
}

void GGView::removeView(GGView *view){
	if(view == NULL || !containsView(view)) return;
	
	std::vector<GGView*>::iterator i = GGView::views.begin();
	
	while(i != GGView::views.end() && *i != view) i++;
	
	if(i != GGView::views.end()){
		GGView::views.erase(i);
	}
}

bool GGView::containsView(GGView *view){
	if(view == NULL) return false;
	
	std::vector<GGView*>::iterator i = GGView::views.begin();
	
	while(i != GGView::views.end() && *i != view) i++;
	
	return i != GGView::views.end();
}

void GGView::removeAllChilds(){
	for(std::vector<GGView*>::iterator i = GGView::views.begin(); i != GGView::views.end(); i++){
		views.erase(i);
	}
}

bool GGView::onMouseDown(GGClickEvent* evt){
	bool ret = false;
	int i = (int) GGView::views.size() - 1;
	
	while(i >= 0){
		ret = GGView::views.at(i)->onMouseDown(evt) || ret;
		i--;
	}
	
	if(this->wasClicked(evt) && mouseDownList.size() > 0){
		std::vector<void (*)(GGClickEvent*)>::iterator i2 = GGView::mouseDownList.begin();
		while(i2 != GGView::mouseDownList.end()){
			(*i2)(evt);
			i2++;
		}
		return true;
	}
	
	return ret;
}

bool GGView::onMouseUp(GGClickEvent* evt){
	bool ret = false;
	int i = (int) GGView::views.size() - 1;
	
	while(i >= 0){
		ret = GGView::views.at(i)->onMouseUp(evt) || ret;
		i--;
	}
	
	if(this->wasClicked(evt) && GGView::mouseUpList.size() > 0){
		std::vector<void (*)(GGClickEvent*)>::iterator i2 = GGView::mouseUpList.begin();
		while(i2 != GGView::mouseUpList.end()){
			(*i2)(evt);
			i2++;
		}
		return true;
	}
	
	return ret;
}

void GGView::draw(){
	std::vector<GGView*>::iterator i = GGView::views.begin();
	
	while(i != GGView::views.end()){
		(*i)->draw();
		i++;
	}
}

void GGView::onResize(GGResizeEvent* evt){
	int i = (int) GGView::views.size() - 1;
	
	while(i >= 0){
		GGView::views.at(i)->onResize(evt);
		i--;
	}
}

void GGView::addMouseDown(void (*function)(GGClickEvent*)){
	GGView::mouseDownList.push_back(function);
}

void GGView::addMouseUp(void (*function)(GGClickEvent*)){
	GGView::mouseUpList.push_back(function);
}

std::vector<GGView*> GGView::getViews(){
	return GGView::views;
}

bool GGView::onKeyDown(GGKeyboardEvent* evt){
	bool ret = false;
	int i = (int) GGView::views.size() - 1;
	
	while(i >= 0 && !ret){
		ret = GGView::views.at(i)->onKeyDown(evt);
		i--;
	}
	
	return ret;
}

bool GGView::onKeyUp(GGKeyboardEvent* evt){
	bool ret = false;
	int i = (int) GGView::views.size() - 1;
	
	while(i >= 0 && !ret){
		ret = GGView::views.at(i)->onKeyUp(evt);
		i--;
	}
	
	return ret;
}

void GGView::onMouseMove(GGMouseMoveEvent* evt){
	int i = (int) GGView::views.size() - 1;
	
	while(i >= 0){
		GGView::views.at(i)->onMouseMove(evt);
		i--;
	}
}

void GGView::viewLogic(){
	return;
}
