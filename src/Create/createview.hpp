#ifndef CREATEVIEWINC
#define CREATEVIEWINC

#include "../Context/Views/views.hpp"
#include "../Context/context.hpp"
#include "grid.hpp"
#include "createsidepanel.hpp"

class CreateView : public GGView{
public:
	CreateView();
	~CreateView();

	void draw() override;
	bool onMouseDown(GGClickEvent*) override;
	bool onMouseUp(GGClickEvent*) override;
	void onResize(GGResizeEvent*) override;
	void onMouseMove(GGMouseMoveEvent*) override;
private:
	bool wasClicked(GGClickEvent*) override;
	CreateSidePanel *sideP;
	Grid *grid;
	texture_t *currentTexSelected;
	Button *backButton;
	Lable *nameLable;
	Textbox *name;
	Button *save;
	Lable *open;
	Dropdown *openDropdown;
	Button *openBtn;
	Lable *rowsLable;
	Textbox *nrows;
	Lable *colsLable;
	Textbox *ncols;
	Button *resizeBtn;
	Scrollbar *zoomBar;
	Lable *zoomLable;
	Button *drawBumpmap;
	Dropdown *dropdownMenu;
	void alignItems();
};

#endif
