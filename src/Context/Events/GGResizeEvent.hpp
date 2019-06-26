#ifndef GGRESIZEEVENTINC
#define GGRESIZEEVENTINC

class GGResizeEvent{
public:
	GGResizeEvent(int width, int height){
		GGResizeEvent::width = width;
		GGResizeEvent::height = height;
	}
	int getWidth(){return width;}
	int getHeight(){return height;}
private:
	int width;
	int height;
};

#endif
