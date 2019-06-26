#ifndef DRAWABLEINC
#define DRAWABLEINC
class Drawable{
public:
	virtual void draw() = 0;
	virtual ~Drawable() = default;
};
#endif
