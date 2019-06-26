#include "galleryitem.hpp"
#include "../colors.hpp"

GalleryItem::GalleryItem(
		std::string *path,
		std::string* name,
		glm::vec2* pos,
		int height,
		int width,
		glm::vec4* textColor
	){
	GalleryItem::name = name;
	GalleryItem::position = pos;
	GalleryItem::height = height;
	GalleryItem::width = width;
	GalleryItem::selected = false;
	GalleryItem::display = false;
	GalleryItem::path = path;
	
	texture_t* tex = Context::renderer->generate_texture(path->c_str());
	
	if(tex == NULL){
		throw "no texture";
		return;
	}
	
	glm::vec2* posi = new glm::vec2(pos->x, pos->y);
	GalleryItem::pic = new Picture(tex, posi, 0, 4*height/5);
	
	posi = new glm::vec2(
		pos->x + width/2 - GalleryItem::pic->getWidth()/2,
		pos->y
	);
	GalleryItem::pic->setPosition(posi);
	
	posi = new glm::vec2(pos->x, pos->y + 4*height/5);
	GalleryItem::lable = new Lable(new std::string(*name), height/5, textColor, posi);
	int tmph = height/5;
	while(GalleryItem::lable->getWidth() > GalleryItem::width - 10)
		GalleryItem::lable->setSize(tmph--);
	
	posi->x = pos->x + width/2 - GalleryItem::lable->getWidth()/2;
	GalleryItem::lable->setPosition(posi);
	
	GGView::addView(GalleryItem::pic);
	GGView::addView(GalleryItem::lable);
}

GalleryItem::~GalleryItem(){
	delete GalleryItem::lable;
	delete GalleryItem::name;
	delete GalleryItem::position;
	delete GalleryItem::pic;
	delete GalleryItem::path;
}

void GalleryItem::draw(){
	if(selected){
		Context::renderer->draw_rect(
			*GalleryItem::position,
			glm::vec2(position->x+width, position->y+height),
			glm::vec4(WHITE)
		);
	}
	
	GGView::draw();
}

bool GalleryItem::onMouseUp(GGClickEvent* evt){
	if(GalleryItem::wasClicked(evt)){
		selected = true;
		return true;
	}
	return false;
}

bool GalleryItem::wasClicked(GGClickEvent* evt){
	return ((evt->getX() > GalleryItem::position->x) &&
		(evt->getX() < GalleryItem::position->x + GalleryItem::width) &&
		(evt->getY() > GalleryItem::position->y) &&
		(evt->getY() < GalleryItem::position->y + GalleryItem::height)
	);
}

bool GalleryItem::isSelected(){
	return selected;
}

void GalleryItem::setPosition(glm::vec2* pos){
	delete GalleryItem::position;
	GalleryItem::position = pos;
	GalleryItem::pic->setPosition(
		new glm::vec2(
			pos->x + width/2 - GalleryItem::pic->getWidth()/2,
			pos->y
		)
	);
	
	GalleryItem::lable->setPosition(
		new glm::vec2(
			pos->x + width/2 - GalleryItem::lable->getWidth()/2,
			pos->y + 4*height/5
		)
	);
}

void GalleryItem::setWidth(int width){
	GalleryItem::width = width;
}

void GalleryItem::setHeight(int height){
	GalleryItem::height = height;
}

void GalleryItem::unselect(){
	GalleryItem::selected = false;
}

texture_t* GalleryItem::getTexture(){
	return GalleryItem::pic->getTexture();
}

void GalleryItem::select(){
	GalleryItem::selected = true;
}

std::string GalleryItem::getName(){
	return *name;
}
