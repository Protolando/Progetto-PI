#include "grid.hpp"
#include "../Context/context.hpp"
#include "../color_palette.h"

Grid::Grid(glm::vec2* pos, int width, int height, int totRows, int totCols){
	int scrollbarThickness = 20;
	Grid::position = pos;
	Grid::width = width - scrollbarThickness;
	Grid::height = height - scrollbarThickness;
	Grid::totRows = totRows;
	Grid::totCols = totCols;
	Grid::firstColShown = 0;
	Grid::firstRowShown = 0;
	Grid::currTexture = -1;
	Grid::start = 0;
	Grid::focused = false;
	Grid::drawBumpmap = false;
	Grid::currentBumpmapValue = WALL;
	
	Grid::hSb = new Scrollbar(
		new glm::vec2(Grid::position->x, Grid::position->y + Grid::height),
		Grid::width,
		false,
		totCols - 1
	);
	Grid::hSb->setThickness(scrollbarThickness);
	Grid::hSb->setCursorColor(new glm::vec4(P_LIGHT_BLUE));
	Grid::hSb->setBgColor(new glm::vec4(P_DARK_BLUE));
	GGView::addView(Grid::hSb);

	Grid::vSb = new Scrollbar(
		new glm::vec2(Grid::position->x + Grid::width, Grid::position->y),
		Grid::height,
		true,
		totRows - 1
	);
	Grid::vSb->setThickness(scrollbarThickness);
	Grid::vSb->setCursorColor(new glm::vec4(P_LIGHT_BLUE));
	Grid::vSb->setBgColor(new glm::vec4(P_DARK_BLUE));
	GGView::addView(Grid::vSb);
	
	Grid::texmap = (int **) malloc(sizeof(int *) * Grid::totRows);
	Grid::bumpmap = (BumpmapValues **) malloc(sizeof(BumpmapValues*) * Grid::totRows);
	for(int i = 0; i < Grid::totRows; i++){
		Grid::texmap[i] = (int *) malloc(sizeof(int) * Grid::totCols);
		Grid::bumpmap[i] = (BumpmapValues *) malloc(sizeof(BumpmapValues) * Grid::totCols);
		for(int j = 0; j < Grid::totCols; j++){
			bumpmap[i][j] = NORMAL_GROUND;
			texmap[i][j] = -1;
		}
	}
}

Grid::~Grid(){
	delete Grid::position;
	
	for(int i = 0; i < Grid::totRows; i++){
		free(Grid::texmap[i]);
	}
	free(Grid::texmap);
	
	delete Grid::hSb;
	delete Grid::vSb;
}

void Grid::draw(){
	Context::renderer->fill_rect(*Grid::position, Grid::width, Grid::height , P_WHITE);
	
	int nrows = Grid::height / (Grid::tilesize*Grid::zoom);
	int ncols = Grid::width / (Grid::tilesize*Grid::zoom);
	
	int row = Grid::firstRowShown;
	int col = Grid::firstColShown;
	
	for(row = Grid::firstRowShown; row < Grid::firstRowShown + nrows + 1; row++){
		for(col = Grid::firstColShown; col < Grid::firstColShown + ncols + 1; col++){
			int cellX = Grid::position->x + (col - Grid::firstColShown)*Grid::tilesize*Grid::zoom;
			int cellY = Grid::position->y + (row - Grid::firstRowShown)*Grid::tilesize*Grid::zoom;
			
			int widthsize = Grid::tilesize * Grid::zoom;
			int heightsize = Grid::tilesize * Grid::zoom;
			
			if(row == Grid::firstRowShown + nrows)
				heightsize = (Grid::position->y + Grid::height) - cellY;
			if(col == Grid::firstColShown + ncols)
				widthsize = (Grid::position->x + Grid::width) - cellX;
			
			if(row < Grid::totRows && col < Grid::totCols && Grid::texmap[row][col] != -1){
				Context::renderer->draw_cropped_bitmap(
					{cellX, cellY},
					Grid::textures.at(texmap[row][col]),
					((float)Grid::tilesize*Grid::zoom) /(float) Grid::textures.at(texmap[row][col])->width,
					widthsize,
					heightsize
				);
			} else if(row >= Grid::totRows || col >= Grid::totCols){
				Context::renderer->fill_rect(
					{cellX, cellY},
					widthsize,
					heightsize,
					BLACK
				);
			}
			
			if(Grid::drawBumpmap && row < Grid::totRows && col < Grid::totCols){
			glm::vec4 color;
			
			switch(Grid::bumpmap[row][col]){
				case NORMAL_GROUND :
					color = glm::vec4(P_GROUND_COLOR);
					break;
				case WALL :
					color = glm::vec4(P_WALL_COLOR);
					break;
				default : color = glm::vec4();
			}
			
				Context::renderer->fill_rect(
					{cellX, cellY},
					widthsize,
					heightsize,
					color
				);
			}
			
		}
	}
	
	for(int i = 1; i < ncols + 1; i++){
		//Draw vertical line
		Context::renderer->draw_line(
			{Grid::position->x + Grid::tilesize*Grid::zoom*i, Grid::position->y},
			{Grid::position->x + Grid::tilesize*Grid::zoom*i, Grid::position->y + Grid::height},
			P_DARK_BLACK,
			3.0f*Grid::zoom
		);
	}
	for(int i = 1; i < nrows + 1; i++){
		//Draw horizontal line
		Context::renderer->draw_line(
			{Grid::position->x, Grid::position->y + Grid::tilesize*Grid::zoom*i},
			{Grid::position->x + Grid::width, Grid::position->y + Grid::tilesize*Grid::zoom*i},
			P_DARK_BLACK,
			3.0f*Grid::zoom
		);
	}
	
	//White square between scrollbars
	Context::renderer->fill_rect(
		glm::vec2(
			Grid::position->x + Grid::width,
			Grid::position->y + Grid::height
		),
		Grid::vSb->getSmallSize(),
		Grid::hSb->getSmallSize(),
		WHITE
	);
	
	GGView::draw();
}

void Grid::onMouseMove(GGMouseMoveEvent* evt){
	GGView::onMouseMove(evt);
	
	Grid::firstRowShown = Grid::vSb->getCursorPos();
	Grid::firstColShown = Grid::hSb->getCursorPos();
	
	if(!Grid::focused) return;
	
	double x = evt->getX();
	double y = evt->getY();
	
	if(x > Grid::position->x &&
	    x < Grid::position->x + Grid::width &&
	    y > Grid::position->y &&
	    y < Grid::position->y + Grid::height
	){
		int nthcol = (x - Grid::position->x)/(Grid::tilesize*Grid::zoom);
		int xcol = Grid::firstColShown + nthcol;
		int nthrow = (y - Grid::position->y)/(Grid::tilesize*Grid::zoom);
		int xrow = Grid::firstRowShown + nthrow;
	
		if(!Grid::drawBumpmap){
			if(Grid::currTexture != -1 && xcol < Grid::totCols && xrow < Grid::totRows)
				Grid::texmap[xrow][xcol] = Grid::currTexture;
		} else {
			if(xcol < Grid::totCols && xrow < Grid::totRows)
				Grid::bumpmap[xrow][xcol] = Grid::currentBumpmapValue;
		}
	} else if((std::clock() - Grid::start)/(double) CLOCKS_PER_SEC > 0.01){
		if(x < Grid::position->x && Grid::firstColShown > 0){
			//Move the pic to the left
			Grid::firstColShown--;
		}
		if(x > Grid::position->x + Grid::width && firstColShown < Grid::totCols){
			//Move the pic to the left
			Grid::firstColShown++;
		}
		if(y < Grid::position->y && Grid::firstRowShown > 0){
			//Move the pic to the left
			Grid::firstRowShown--;
		}
		if(y > Grid::position->y + Grid::height && firstRowShown < Grid::totRows){
			//Move the pic to the left
			Grid::firstRowShown++;
		}
		
		Grid::vSb->setCursorPos(Grid::firstRowShown);
		Grid::hSb->setCursorPos(Grid::firstColShown);
		
		Grid::start = std::clock();
	}
}

bool Grid::onMouseDown(GGClickEvent* evt){
	bool ret = GGView::onMouseDown(evt);

	if(Grid::wasClicked(evt)){
		Grid::focused = true;
		ret = true;
	}
	
	return ret;
}

bool Grid::onMouseUp(GGClickEvent* evt){
	Grid::focused = false;

	return GGView::onMouseUp(evt);
}

void Grid::setWidth(int w){
	Grid::width = w;
	
	Grid::hSb->setBigSize(w);
	Grid::vSb->setPosition(new glm::vec2(Grid::position->x + Grid::width, Grid::position->y));
}

void Grid::setHeight(int h){
	Grid::height = h;
	Grid::vSb->setBigSize(h);
	Grid::hSb->setPosition(new glm::vec2(Grid::position->x, Grid::position->y + Grid::height));
}

void Grid::setPosition(glm::vec2* pos){
	if(Grid::position && Grid::position != pos)
		delete Grid::position;
	
	Grid::position = pos;
	Grid::hSb->setPosition(new glm::vec2(Grid::position->x, Grid::position->y + Grid::height));
	Grid::vSb->setPosition(new glm::vec2(Grid::position->x + Grid::width, Grid::position->y));
}

bool Grid::wasClicked(GGClickEvent* evt){
	double x = evt->getX();
	double y = evt->getY();

	if( x > Grid::position->x &&
	    x < Grid::position->x + Grid::width &&
	    y > Grid::position->y &&
	    y < Grid::position->y + Grid::height
	    ) {
		return true;
	}
	else return false;
}

void Grid::setCurrTexture(texture_t* tex){
	if(tex == NULL){
		Grid::currTexture = -1;
		return;
	}
	int i = 0;

	while(i < (int) Grid::textures.size() && Grid::textures.at(i) != tex){
		i++;
	}
	if(i >= (int) Grid::textures.size()){
		Grid::textures.push_back(tex);
	}
	
	Grid::currTexture = i;
}

nlohmann::json Grid::exportMap(){
	/*
	 * Map format: Json file made as follows:
	 * 
	 * {
	 *   "ntextures" : k,
	 *   "textures" : [path_0, path_1, ..., path_k],
	 *	 "nrows" : n,
	 *   "ncols" : m,
	 *   "map" : [
	 				[TextureAt_00, TextureAt_01, ..., TextureAt_0m],
	 				[TextureAt_10, TextureAt_11, ..., TextureAt_1m],
	 				...,
	 				[TextureAt_n0, TextureAt_n1, ..., TextureAt_nm],
	 			 ],
	 	 "bumpmap" : [
	 	 				[valueAt_00, valueAt_01, ... valueAt_0m],
	 	 				[valueAt_10, valueAt_11, ... valueAt_1m],
	 	 				...,
	 	 				[valueAt_n0, valueAt_n1, ... valueAt_nm],
	 	 			 ]
	 * }
	 */
	nlohmann::json res;
	
	res["ntextures"] = (int) Grid::textures.size();
	res["textures"] = nlohmann::json::array();
	for(int i = 0; i < (int) Grid::textures.size(); i++){
		res["textures"].emplace_back(*(Grid::textures.at(i)->path));
	}
	
	res["nrows"] = Grid::totRows;
	res["ncols"] = Grid::totCols;
	
	for(int i = 0; i < Grid::totRows; i++){
		res["map"].emplace_back(nlohmann::json::array());
		for(int j = 0; j < Grid::totCols; j++){
			res["map"][i].emplace_back(texmap[i][j]);
		}
	}
	
	for(int i = 0; i < Grid::totRows; i++){
		res["bumpmap"].emplace_back(nlohmann::json::array());
		for(int j = 0; j < Grid::totCols; j++){
			res["bumpmap"][i].emplace_back(bumpmap[i][j]);
		}
	}
	
	return res;
}

void Grid::importMap(nlohmann::json map){

	Grid::setNrows(map["nrows"].get<int>());
	Grid::setNcols(map["ncols"].get<int>());
	
	int mapTextures[map["ntextures"].get<int>()];
	
	for(int i = 0; i < map["ntextures"].get<int>(); i++){
		
		int j = 0;
	
		while(
			*(Grid::textures.at(j)->path) != map["textures"][i].get<std::string>()
			&& j < (int) Grid::textures.size()
		) {
			j++;
		}
		
		if(j > (int) Grid::textures.size())
			mapTextures[i] = -1;
		else
			mapTextures[i] = j;
	}
	
	for(int i = 0; i < Grid::totRows; i++){
		for(int j = 0; j < Grid::totCols; j++){
			if(map["map"][i][j].get<int>() == -1){
				Grid::texmap[i][j] = -1;
			} else {
				Grid::texmap[i][j] = mapTextures[map["map"][i][j].get<int>()];
			}
			
			Grid::bumpmap[i][j] = valuesArray[map["bumpmap"][i][j].get<int>()];
		}
	}
}

void Grid::setZoom(double zoom){
	Grid::zoom = zoom;
}

void Grid::setNrows(int n){	
	if(n == Grid::totRows)
		return;

	int **tmp = (int**) malloc(sizeof(int*)*n);
	BumpmapValues **tmp2 = (BumpmapValues **) malloc(sizeof(BumpmapValues*)*n);

	int k = ((n < Grid::totRows) ? n : Grid::totRows);
	
	for(int i = 0; i < k; i++){
		tmp[i] = Grid::texmap[i];
		tmp2[i] = Grid::bumpmap[i];
	}

	if(n < Grid::totRows) {
		for(int i = n; i < Grid::totRows; i++){
			free(Grid::texmap[i]);
			free(Grid::bumpmap[i]);
		}
	} else {
		for(int i = Grid::totRows; i < n; i++){
			tmp[i] = (int *) malloc(sizeof(int) * Grid::totCols);
			tmp2[i] = (BumpmapValues *) malloc(sizeof(BumpmapValues) * Grid::totCols);
			
			for(int j = 0; j < Grid::totCols; j++){
				tmp[i][j] = -1;
				tmp2[i][j] = NORMAL_GROUND;
			}
		}
	}

	free(Grid::texmap);
	free(Grid::bumpmap);
	Grid::texmap = tmp;
	Grid::bumpmap = tmp2;

	Grid::totRows = n;
	Grid::vSb->setNelems(Grid::totRows - 1);
}

void Grid::setNcols(int n){

	if(n == Grid::totCols)
		return;

	for(int i = 0; i < Grid::totRows; i++){
		
		int *tmp = (int *) malloc(sizeof(int) * n);
		BumpmapValues *tmp2 = (BumpmapValues *) malloc(sizeof(BumpmapValues) * n);
		
		int k = ((n < Grid::totCols) ? n : Grid::totCols);
		for(int j = 0; j < k; j++){
			tmp[j] = Grid::texmap[i][j];
			tmp2[j] = Grid::bumpmap[i][j];
		}
		
		if(n > Grid::totCols) {
			for(int j = Grid::totCols; j < n; j++){
				tmp[j] = -1;
				tmp2[j] = NORMAL_GROUND;
			}
		}
		
		free(Grid::texmap[i]);
		free(Grid::bumpmap[i]);
		Grid::texmap[i] = tmp;
		Grid::bumpmap[i] = tmp2;
	}

	Grid::totCols = n;
	Grid::hSb->setNelems(Grid::totCols - 1);
}

void Grid::setBumpmapMode(){
	Grid::drawBumpmap = true;
}

void Grid::resetBumpmapMode(){
	Grid::drawBumpmap = false;
}

bool Grid::isBumpmapMode(){
	return Grid::drawBumpmap;
}

void Grid::setBumpmapValue(BumpmapValues b){
	Grid::currentBumpmapValue = b;
}

void Grid::addTexture(texture_t *tex){
	for(auto i = Grid::textures.begin(); i != Grid::textures.end(); i++){
		if((*i) == tex)
			return;
	}
	
	Grid::textures.push_back(tex);
}
