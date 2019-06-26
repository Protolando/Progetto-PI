#ifndef STRUCTTEXTUREINC
#define STRUCTTEXTUREINC

#include <string>

typedef struct texture{
	std::string *path;
	int width;
	int height;
	unsigned int buffer;
} texture_t;

#endif
