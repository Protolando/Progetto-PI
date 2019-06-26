#ifndef GGKEYBOARDEVENTINC
#define GGKEYBOARDEVENTINC

#include <string.h>
#define GG_EVENT_REGULAR 0
#define GG_EVENT_SPECIAL 1

class GGKeyboardEvent{
public:
	GGKeyboardEvent(int key, int type, int mods){
		GGKeyboardEvent::key = key;
		GGKeyboardEvent::type = type;
		GGKeyboardEvent::mods = mods;
	}
	
	int getType(){
		return type;
	}
	
	int getKey(){
		return GGKeyboardEvent::key;
	}
	
	int getMods(){
		return mods;
	}

private:
	int key;
	int type;
	int mods;
};

#endif
