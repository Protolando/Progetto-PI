#ifndef GGMOUSEMOVEEVENTINC
#define GGMOUSEMOVEEVENTINC

class GGMouseMoveEvent{
public:
	GGMouseMoveEvent(double xpos, double ypos){
		GGMouseMoveEvent::xpos = xpos;
		GGMouseMoveEvent::ypos = ypos;
	}
	int getX(){return xpos;}
	int getY(){return ypos;}
private:
	int xpos;
	int ypos;
};

#endif
