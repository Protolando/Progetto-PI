#ifndef GAMEMAPINC
#define GAMEMAPINC

#include "../Context/context.hpp"
#include "../Context/rect.hpp"
#include "../Context/texture.hpp"
#include "../bumpmap_values.hpp"

using namespace Bumpmap;

class GameMap{
public:
	GameMap(std::string);
	~GameMap();
	
	void draw(glm::mat4);
	
	int getWidth();
	int getHeight();
	
	bool isWalkable(Rect);
	
	void toggleTileNumber();
private:
	void importMap(std::string);
	int **texturemap;
	BumpmapValues **bumpmap;
	std::vector<texture_t *> textures;
	int mapWidth;
	int mapHeight;
	bool showTileNumber = false;
};

#endif
