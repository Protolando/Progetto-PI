#include <stdio.h>
#include <stdlib.h>
#include "Context/context.hpp"
#include "mainmenu.hpp"
#include <dirent.h>
#include "def.hpp"
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char** argv){
	int dir_err = mkdir(DIRPATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (-1 == dir_err && errno != EEXIST){
		fprintf(stderr, "Error creating directory!\n");
		exit(1);
	}
	
	mkdir(TEXPATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (-1 == dir_err && errno != EEXIST){
		fprintf(stderr, "Error creating directory!\n");
		exit(1);
	}
	
	mkdir(MAPSPATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (-1 == dir_err && errno != EEXIST){
		fprintf(stderr, "Error creating directory!\n");
		exit(1);
	}
	mkdir(ASSETSPATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (-1 == dir_err && errno != EEXIST){
		fprintf(stderr, "Error creating directory!\n");
		exit(1);
	}
	
	Context::init("Progetto Pi");
	Context::setView(new MainMenu());
	Context::mainLoop();
	
	return 0;
}
