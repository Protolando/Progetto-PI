#include "gallery.hpp"
#include "../colors.hpp"
#include <iostream>

Gallery::Gallery(glm::vec2* pos, int width, int height){
	Gallery::width = width;
	Gallery::height = height;
	Gallery::position = pos;
	Gallery::color = new glm::vec4(BLACK);
	Gallery::firstItemShown = 0;
	
	Gallery::s = new Scrollbar(new glm::vec2(0.0f, 0.0f), Gallery::height, true, 0);
	Gallery::s->setSmallSize(20);
	GGView::addView(Gallery::s);
	
	Gallery::width = Gallery::width - Gallery::s->getSmallSize();
	Gallery::s->setPosition(
		new glm::vec2(
			Gallery::position->x + Gallery::width,
			Gallery::position->y
		)
	);
	Gallery::itemsPerLine = floor(Gallery::width/Gallery::itemSize);
	Gallery::nLinesShown = ceil(Gallery::height/Gallery::itemSize);
	Gallery::s->setNelems(ceil(Gallery::items.size()/Gallery::itemsPerLine + 1) - Gallery::nLinesShown);
}

Gallery::~Gallery(){
	for(std::vector<GalleryItem*>::iterator i = Gallery::items.begin(); i != Gallery::items.end(); i++){
		delete *i;
	}
	
	delete Gallery::position;
	delete Gallery::color;
}

GalleryItem* Gallery::getSelected(){
	for(std::vector<GalleryItem*>::iterator i = Gallery::items.begin(); i != Gallery::items.end(); i++){
		if((*i)->isSelected())
			return *i;
	}
	
	return NULL;
}

void Gallery::addItem(GalleryItem* i){
	Gallery::items.push_back(i);
	i->setWidth(Gallery::itemSize);
	i->setHeight(Gallery::itemSize);
	
	Gallery::s->setNelems(ceil((Gallery::items.size() + 1)/Gallery::itemsPerLine) - Gallery::nLinesShown);
	Gallery::alignItems();
}

void Gallery::setColor(glm::vec4* color){
	if(Gallery::color)
		delete Gallery::color;
	Gallery::color = color;
}
	
void Gallery::setPosition(glm::vec2* pos){
	if(Gallery::position)
		delete Gallery::position;
	
	Gallery::position = pos;
	
	Gallery::s->setPosition(
		new glm::vec2(
			Gallery::position->x + Gallery::width,
			Gallery::position->y
		)
	);
	Gallery::alignItems();
}

void Gallery::setWidth(int width){
	Gallery::width = width - Gallery::s->getSmallSize();
	Gallery::itemsPerLine = floor(Gallery::width/Gallery::itemSize);
	Gallery::s->setNelems(ceil(Gallery::items.size()/Gallery::itemsPerLine + 1) - Gallery::nLinesShown);
	Gallery::firstItemShown = s->getCursorPos() * Gallery::itemsPerLine;
	Gallery::alignItems();
}

void Gallery::setHeight(int height){
	Gallery::height = height;
	Gallery::nLinesShown = ceil(Gallery::height/Gallery::itemSize);
	Gallery::s->setNelems(ceil(Gallery::items.size()/Gallery::itemsPerLine + 1) - Gallery::nLinesShown);
	Gallery::firstItemShown = s->getCursorPos() * Gallery::itemsPerLine;
	Gallery::s->setBigSize(height);
	Gallery::alignItems();
}

void Gallery::draw(){
	Context::renderer->fill_rect(
		*Gallery::position,
		glm::vec2(
			Gallery::position->x + Gallery::width,
			Gallery::position->y + Gallery::height
		),
		*Gallery::color
	);
	
	for(int i = Gallery::firstItemShown;
		i - Gallery::firstItemShown < Gallery::nLinesShown * Gallery::itemsPerLine && i < (int) Gallery::items.size();
		i++) {
			Gallery::items.at(i)->draw();
	}

	GGView::draw();
}

bool Gallery::wasClicked(GGClickEvent* evt){
	return ((evt->getX() > Gallery::position->x) &&
		(evt->getX() < Gallery::position->x + Gallery::width) &&
		(evt->getY() > Gallery::position->y) &&
		(evt->getY() < Gallery::position->y + Gallery::height)
	);
}

void Gallery::onResize(GGResizeEvent* evt){
	Gallery::itemsPerLine = floor(Gallery::width/Gallery::itemSize);
	Gallery::nLinesShown = ceil(Gallery::height/Gallery::itemSize);
	Gallery::s->setNelems(ceil(Gallery::items.size()/Gallery::itemsPerLine + 1) - Gallery::nLinesShown);
	Gallery::firstItemShown = s->getCursorPos() * Gallery::itemsPerLine;
	Gallery::alignItems();
	GGView::onResize(evt);
}

bool Gallery::onMouseUp(GGClickEvent* evt){
	if(Gallery::wasClicked(evt)){
		GalleryItem *tmp = Gallery::getSelected();
		
		for(std::vector<GalleryItem*>::iterator i = Gallery::items.begin(); i != Gallery::items.end(); i++){
			(*i)->unselect();
			(*i)->onMouseUp(evt);
		}
		
		GGView::onMouseUp(evt);
		
		if(Gallery::getSelected() == NULL && tmp != NULL){
			tmp->select();
		}
		
		return true;
	}
	
	return GGView::onMouseUp(evt);
}

void Gallery::setScrollbarColors(glm::vec4 *bar, glm::vec4 *cursor){
	if(bar)
		Gallery::s->setBgColor(bar);
	if(cursor)
    	Gallery::s->setCursorColor(cursor);
}

void Gallery::alignItems(){
	
	int firstLineNumber = Gallery::firstItemShown/Gallery::itemsPerLine;
	
	for(int i = 0; i < Gallery::nLinesShown && (i + firstLineNumber) * Gallery::itemsPerLine < (int) Gallery::items.size(); i++) {
		int tmp = (i + firstLineNumber) * Gallery::itemsPerLine;
			for(int j = 0; j <  Gallery::itemsPerLine && tmp + j < (int) Gallery::items.size(); j++){
				Gallery::items.at(tmp + j)->setPosition(
					new glm::vec2(
						Gallery::position->x + Gallery::itemSize*j,
						Gallery::position->y + Gallery::itemSize*i
						)
					);
			}
	}
}

void Gallery::onMouseMove(GGMouseMoveEvent *evt){
	if(Gallery::firstItemShown != Gallery::s->getCursorPos() * Gallery::itemsPerLine){
		Gallery::firstItemShown = Gallery::s->getCursorPos() * Gallery::itemsPerLine;
		Gallery::alignItems();
	}
	
	GGView::onMouseMove(evt);
}

std::vector<texture_t *> Gallery::getTextures(){
	std::vector<texture_t *> textures;
	for(auto i = Gallery::items.begin(); i != Gallery::items.end(); i++)
		textures.push_back((*i)->getTexture());
		
	return textures;
}
