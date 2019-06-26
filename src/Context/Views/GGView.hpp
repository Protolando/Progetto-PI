//Abstract class that defines a view.
#ifndef GGVIEWINC
#define GGVIEWINC

#include "drawable.hpp"
#include "interactable.hpp"
#include <vector>

class GGView : public Drawable, public Interactable{
public:
	void addView(GGView*);
	void removeView(GGView*);
	bool containsView(GGView *);
	void removeAllChilds();
	
	virtual void onResize(GGResizeEvent*);
	void onMouseMove(GGMouseMoveEvent*) override;
	
	bool onMouseDown(GGClickEvent*) override;
	bool onMouseUp(GGClickEvent*) override;
	void addMouseDown(void (*)(GGClickEvent*));
	void addMouseUp(void (*)(GGClickEvent*));
	
	bool onKeyDown(GGKeyboardEvent*) override;
	bool onKeyUp(GGKeyboardEvent*) override;
	void addKeyDown(void (*) (GGKeyboardEvent*));
	void addKeyUp(void (*) (GGKeyboardEvent*));
	
	virtual void viewLogic();
	
	void draw() override;
private:
	std::vector<GGView*> views;
	std::vector<void (*)(GGClickEvent*)> mouseDownList;
	std::vector<void (*)(GGClickEvent*)> mouseUpList;
	std::vector<GGView*> getViews();
	virtual bool wasClicked(GGClickEvent*) = 0;
};
#endif
