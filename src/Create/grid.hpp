#ifndef GRIDINC
#define GRIDINC

#include "../Context/Views/views.hpp"
#include <vector>
#include <string>
#include <ctime>
#include "../json.hpp"
#include "../bumpmap_values.hpp"

using namespace Bumpmap;

class Grid : public GGView {
public:
	Grid(glm::vec2*, int, int, int, int);
	~Grid();
	
	void onMouseMove(GGMouseMoveEvent*) override;
	bool onMouseDown(GGClickEvent*) override;
	bool onMouseUp(GGClickEvent*) override;

	void draw() override;

	void setWidth(int);
	void setHeight(int);
	void setPosition(glm::vec2*);
	void setZoom(double);
	
	void setNrows(int);
	void setNcols(int);
	
	void addTexture(texture_t *);
	void setCurrTexture(texture_t *);
	
	nlohmann::json exportMap();
	void importMap(nlohmann::json);
	
	void setBumpmapMode();
	void resetBumpmapMode();
	bool isBumpmapMode();
	void setBumpmapValue(BumpmapValues);
	
private:
	std::vector<texture_t *> textures;
	BumpmapValues** bumpmap = NULL;
	int** texmap = NULL;
	int currTexture = -1;
	bool focused = false;
	glm::vec2* position = 0;
	int width = 0;
	int height = 0;
	int totRows = 0;
	int totCols = 0;
	int firstColShown = 0;
	int firstRowShown = 0;
	const int tilesize = 50;
	double zoom = 1;
	clock_t start;
	bool wasClicked(GGClickEvent*) override;
	Scrollbar *hSb = NULL;
	Scrollbar *vSb = NULL;
	bool drawBumpmap;
	BumpmapValues currentBumpmapValue;
};

#endif
