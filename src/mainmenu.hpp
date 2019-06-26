#ifndef MAINMENUINC
#define MAINMENUINC

#include "Context/Views/views.hpp"
#include "Context/context.hpp"

class MainMenu : public GGView{
public:
	MainMenu();
	void onResize(GGResizeEvent*) override;
	~MainMenu();
private:
	Button* playBtn;
	Button* createBtn;
	Lable* title;
	bool wasClicked(GGClickEvent*) override;
};
#endif
