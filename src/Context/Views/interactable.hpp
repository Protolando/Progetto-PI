#ifndef CLICKABLEINC
#define CLICKABLEIND

#include "../Events/GGMouseMoveEvent.hpp"
#include "../Events/GGClickEvent.hpp"
#include "../Events/GGResizeEvent.hpp"
#include "../Events/GGKeyboardEvent.hpp"

class Interactable{
public:
	virtual bool onMouseDown(GGClickEvent*) = 0;
	virtual bool onMouseUp(GGClickEvent*) = 0;
	virtual void onMouseMove(GGMouseMoveEvent*) = 0;
	virtual bool onKeyDown(GGKeyboardEvent*) = 0;
	virtual bool onKeyUp(GGKeyboardEvent*) = 0;
	virtual ~Interactable() = default;
};

#endif
