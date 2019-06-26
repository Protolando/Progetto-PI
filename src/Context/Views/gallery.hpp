#ifndef GALLERYINC
#define GALLERYINC

#include "GGView.hpp"
#include <vector>
#include <string>
#include "galleryitem.hpp"
#include "scrollbar.hpp"

class Gallery : public GGView{
public :
	Gallery(glm::vec2*, int, int);
	~Gallery();
	
	GalleryItem* getSelected();
	void addItem(GalleryItem*);
	void setColor(glm::vec4*);
	
	void setPosition(glm::vec2*);
	void setWidth(int);
	void setHeight(int);
	
	void draw() override;
	void onResize(GGResizeEvent*) override;
	
	std::vector<texture_t *> getTextures();
	void setScrollbarColors(glm::vec4 *, glm::vec4 *);
	
	bool onMouseUp(GGClickEvent*) override;
	void onMouseMove(GGMouseMoveEvent *) override;
	
private :
	std::vector<GalleryItem*> items;
	int firstItemShown;
	glm::vec2* position;
	int width;
	int height;
	glm::vec4* color;
	
	void alignItems();
	
	Scrollbar *s;
	
	const int itemSize = 100;
	int itemsPerLine;
	int nLinesShown;
	
	bool wasClicked(GGClickEvent*) override;
};

#endif
