#ifndef CREATESIDEPANELINC
#define CREATESIDEPANELINC

#include "../Context/Views/views.hpp"

class CreateSidePanel : public SidePanel {
public :
	CreateSidePanel(int, glm::vec4*);
	~CreateSidePanel();
	
	void onResize(GGResizeEvent*) override;
	void onMouseMove(GGMouseMoveEvent*) override;
	bool onMouseUp(GGClickEvent*) override;
	std::vector<texture_t *> getTextures();
	
	texture_t* currentTexSelected;
private:
	Textbox* tb;
	Gallery* gallery;
	Button* submitButton;
	Button* clearButton;
};

#endif
