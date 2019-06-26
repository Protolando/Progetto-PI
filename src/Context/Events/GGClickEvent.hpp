#ifndef GGCLICKEVENTINC
#define GGCLICKEVENTINC

class GGClickEvent{
public:
	GGClickEvent(int button, int action, int mods, double x, double y){
		GGClickEvent::button = button;
		GGClickEvent::action = action;
		GGClickEvent::mods = mods;
		GGClickEvent::mouseX = x;
		GGClickEvent::mouseY = y;
	}
	int getButton(){return button;}
	int getAction(){return action;}
	int getMods(){return mods;}
	double getX(){return mouseX;}
	double getY(){return mouseY;}
private:
	int button;
	int action;
	int mods;
	double mouseX;
	double mouseY;
};

#endif
