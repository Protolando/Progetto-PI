#ifndef GALLERYITEMINC
#define GALLERYITEMINC

#include "GGView.hpp"
#include "lable.hpp"
#include "../context.hpp"
#include "../texture.hpp"
#include "picture.hpp"

class GalleryItem : public GGView {
public : 
	GalleryItem(std::string*, std::string*, glm::vec2*, int, int, glm::vec4*);
	~GalleryItem();
	
	void draw() override;
	bool onMouseUp(GGClickEvent*) override;
	bool isSelected();
	void unselect();
	void select();
	int getWidth();
	int getHeight();
	void setWidth(int);
	void setHeight(int);
	void setPosition(glm::vec2*);
	bool display;
	texture_t* getTexture();
	std::string getName();
	
private :
	glm::vec2* position;
	std::string* name;
	int height;
	int width;
	Lable* lable;
	Picture *pic;
	bool selected;
	std::string *path;
	
	bool wasClicked(GGClickEvent*) override;
};

#endif
