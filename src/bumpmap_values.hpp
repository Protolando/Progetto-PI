#ifndef BUMPMAPVALUESINC
#define BUMPMAPVALUESINC

namespace Bumpmap{
	enum BumpmapValues{
		NORMAL_GROUND = 0,
		WALL = 1
	};
  
  extern int numValues;
  extern const char *values[];
  extern BumpmapValues valuesArray[];
}

#endif
