#include "createview.hpp"
#include "../color_palette.h"
#include "../Context/context.hpp"
#include <math.h>
#include <dirent.h>
#include "../json.hpp"
#include "../bumpmap_values.hpp"
#include "../def.hpp"
#include "../mainmenu.hpp"
#define LARGE_PADDING 40
#define SMALL_PADDING 20
#define VERY_SMALL_PADDING 5
#define BACKTEXT "back"
#define SAVE "save"
#define OPEN "open"
#define RESIZE "resize"
#define ROWS "rows"
#define COLS "cols"
#define NAME "file name"
#define DRAWBUMPMAP "draw bumpmap"
#define DRAWMAP "draw map"
using namespace nlohmann;

void backButtonCallback(GGClickEvent *){
	Context::switchView(new MainMenu());
}

bool is_number(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

CreateView::CreateView(){
	Context::renderer->set_background_color(P_DARK_BLACK);
	
	CreateView::currentTexSelected = NULL;

	CreateView::sideP = new CreateSidePanel(50, new glm::vec4(P_LIGHT_BLACK));
	
	int xPos = LARGE_PADDING;
	int yPos = LARGE_PADDING;
	
	CreateView::backButton = new Button(
		new std::string(BACKTEXT),
		new glm::vec2(xPos, yPos),
		SMALL_PADDING
	);
	CreateView::backButton->setColor(new glm::vec4(P_LIGHT_BLUE));
    CreateView::backButton->setClickedColor(new glm::vec4(P_DARK_BLUE));
    CreateView::backButton->addMouseUp(&backButtonCallback);
	GGView::addView(CreateView::backButton);
	
	CreateView::nameLable = new Lable(
		new std::string(NAME),
		SMALL_PADDING,
		new glm::vec4(WHITE),
		new glm::vec2(xPos, yPos)
	);
	GGView::addView(CreateView::nameLable);
	CreateView::name = new Textbox(new glm::vec2(xPos, yPos), 100, SMALL_PADDING);
	GGView::addView(CreateView::name);
	
	CreateView::save = new Button(
		new std::string(SAVE),
		new glm::vec2(xPos, yPos),
		SMALL_PADDING
	);
	CreateView::save->setColor(new glm::vec4(P_LIGHT_BLUE));
    CreateView::save->setClickedColor(new glm::vec4(P_DARK_BLUE));
	GGView::addView(CreateView::save);
	
	CreateView::open = new Lable(
		new std::string(OPEN),
		SMALL_PADDING,
		new glm::vec4(WHITE),
		new glm::vec2(xPos, yPos)
	);
	GGView::addView(CreateView::open);
	
	CreateView::openDropdown = new Dropdown(
		new glm::vec2(xPos, yPos),
		100,
		SMALL_PADDING
	);
	
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (MAPSPATH)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if(strncmp(ent->d_name, ".", 1) != 0)
		  		CreateView::openDropdown->addElement(new std::string(ent->d_name));
			}
	 	closedir(dir);
	 }
	
	CreateView::openBtn = new Button(
		new std::string(OPEN),
		new glm::vec2(xPos, yPos),
		SMALL_PADDING
	);
	CreateView::openBtn->setColor(new glm::vec4(P_LIGHT_BLUE));
    CreateView::openBtn->setClickedColor(new glm::vec4(P_DARK_BLUE));
	GGView::addView(CreateView::openBtn);
	
	CreateView::rowsLable = new Lable(
		new std::string(ROWS),
		SMALL_PADDING,
		new glm::vec4(WHITE),
		new glm::vec2(xPos, yPos)
	);
	GGView::addView(CreateView::rowsLable);
	CreateView::nrows = new Textbox(new glm::vec2(xPos, yPos), 50, SMALL_PADDING);
	GGView::addView(CreateView::nrows);
	
	CreateView::colsLable = new Lable(
		new std::string(COLS),
		SMALL_PADDING,
		new glm::vec4(WHITE),
		new glm::vec2(xPos, yPos)
	);
	GGView::addView(CreateView::colsLable);
	CreateView::ncols = new Textbox(new glm::vec2(xPos, yPos), 50, SMALL_PADDING);
	GGView::addView(CreateView::ncols);
	
	CreateView::resizeBtn = new Button(
		new std::string(RESIZE),
		new glm::vec2(xPos, yPos),
		SMALL_PADDING
	);
	CreateView::resizeBtn->setColor(new glm::vec4(P_LIGHT_BLUE));
    CreateView::resizeBtn->setClickedColor(new glm::vec4(P_DARK_BLUE));
	GGView::addView(CreateView::resizeBtn);
	
	CreateView::zoomBar = new Scrollbar(new glm::vec2(xPos, yPos), 100, false, 100);
	GGView::addView(CreateView::zoomBar);
	CreateView::zoomBar->setCursorPos(50);
	CreateView::zoomBar->setSmallSize(SMALL_PADDING);
	CreateView::zoomBar->setCursorColor(new glm::vec4(P_LIGHT_BLUE));
	CreateView::zoomBar->setBgColor(new glm::vec4(P_DARK_BLUE));
	
	CreateView::zoomLable = new Lable(
		new std::string("100%"),
		SMALL_PADDING,
		new glm::vec4(WHITE),
		new glm::vec2(xPos, yPos)
	);
	GGView::addView(CreateView::zoomLable);
	
	CreateView::grid = new Grid(
		new glm::vec2(LARGE_PADDING, yPos + 2*SMALL_PADDING),
		Context::window_width - CreateView::sideP->getWidth() - LARGE_PADDING * 2,
		Context::window_height - yPos - LARGE_PADDING - SMALL_PADDING,
		100,
		100
	);
	CreateView::nrows->setText(std::string("100"));
	CreateView::ncols->setText(std::string("100"));
	
	CreateView::drawBumpmap = new Button(
		new std::string(DRAWBUMPMAP),
		new glm::vec2(xPos, yPos),
		SMALL_PADDING
	);
	CreateView::drawBumpmap->setColor(new glm::vec4(P_LIGHT_BLUE));
    CreateView::drawBumpmap->setClickedColor(new glm::vec4(P_DARK_BLUE));
	GGView::addView(CreateView::drawBumpmap);
	
	CreateView::dropdownMenu = new Dropdown(
		new glm::vec2(xPos, yPos),
		100,
		SMALL_PADDING
	);
	
	for(int i = 0; i < Bumpmap::numValues; i++){
		dropdownMenu->addElement(new std::string(Bumpmap::values[i]));
	}
	CreateView::dropdownMenu->deactivate();

	GGView::addView(CreateView::grid);
	GGView::addView(CreateView::dropdownMenu);
	GGView::addView(CreateView::openDropdown);
	GGView::addView(CreateView::sideP);
	
	std::vector<texture_t *> texArray = CreateView::sideP->getTextures();
	for(auto i = texArray.begin(); i != texArray.end(); i++){
		CreateView::grid->addTexture((*i));
	}
	
	CreateView::alignItems();
	
}

CreateView::~CreateView(){
	delete CreateView::grid;
	delete CreateView::sideP;
	delete CreateView::name;
	delete CreateView::save;
	delete CreateView::openDropdown;
	delete CreateView::open;
	delete CreateView::openBtn;
	delete CreateView::rowsLable;
	delete CreateView::nrows;
	delete CreateView::colsLable;
	delete CreateView::ncols;
	delete CreateView::resizeBtn;
	delete CreateView::zoomBar;
	delete CreateView::zoomLable;
	delete CreateView::drawBumpmap;
	delete CreateView::dropdownMenu;
}

void CreateView::draw(){
	GGView::draw();
}

bool CreateView::onMouseDown(GGClickEvent* evt){
	return GGView::onMouseDown(evt);
}

bool CreateView::onMouseUp(GGClickEvent* evt){
	// <Spaghetticode>
	bool dropdownwasclicked = CreateView::dropdownMenu->was_clicked(evt);
	// </Spaghetticode>
	bool tmp = GGView::onMouseUp(evt);
	
	if(CreateView::sideP->currentTexSelected != CreateView::currentTexSelected){
		if(sideP->currentTexSelected){
			CreateView::currentTexSelected = CreateView::sideP->currentTexSelected;
			CreateView::grid->setCurrTexture(CreateView::currentTexSelected);
		} else{
			CreateView::currentTexSelected = NULL;
			CreateView::grid->setCurrTexture(NULL);
		}
	}
	
	if(CreateView::resizeBtn->onMouseUp(evt)){
		if(
			!is_number(CreateView::ncols->getText()) ||
			!is_number(CreateView::nrows->getText()) ||
			atoi(CreateView::ncols->getText().c_str()) <= 0 ||
			atoi(CreateView::nrows->getText().c_str()) <= 0
		) {
			CreateView::ncols->displayError();
			CreateView::nrows->displayError();
		} else {
			CreateView::grid->setNcols(atoi(CreateView::ncols->getText().c_str()));
			CreateView::grid->setNrows(atoi(CreateView::nrows->getText().c_str()));
		}
	} else if(CreateView::drawBumpmap->onMouseUp(evt)){
		if(CreateView::grid->isBumpmapMode()){
			CreateView::drawBumpmap->setLable(new std::string(DRAWBUMPMAP));
			CreateView::dropdownMenu->deactivate();
			CreateView::grid->resetBumpmapMode();
			CreateView::alignItems();
		} else {
			CreateView::drawBumpmap->setLable(new std::string(DRAWMAP));
			CreateView::grid->setBumpmapMode();
			CreateView::dropdownMenu->activate();
			CreateView::alignItems();
		}
	} else if(
			dropdownwasclicked &&
			CreateView::dropdownMenu->isActive() &&
			!CreateView::dropdownMenu->isExpanded()){
		std::string tmps = CreateView::dropdownMenu->getCurrentChoice();
		
		int idx = 0;
		while(idx < Bumpmap::numValues && strcmp(tmps.c_str(), Bumpmap::values[idx]) != 0) idx++;
		
		if(idx < Bumpmap::numValues)
			CreateView::grid->setBumpmapValue(Bumpmap::valuesArray[idx]);
	} else if(CreateView::save->onMouseUp(evt)){
		if(CreateView::name->getText() == ""){
			CreateView::name->displayError();
		} else {
			std::string filename = CreateView::name->getText();
			std::string path = std::string(MAPSPATH) + "/" + filename;
			json j = CreateView::grid->exportMap();
			Context::writeToFile(j.dump().c_str(), path.c_str());
		}
	} else if(CreateView::openBtn->onMouseUp(evt)){
		if(CreateView::openDropdown->getCurrentChoice() != ""){
			std::string path = std::string(MAPSPATH) + "/" + CreateView::openDropdown->getCurrentChoice();
			
			json j = json::parse(Context::readFromFile(path.c_str()));
			CreateView::grid->importMap(j);
		}
	}
	
	return tmp;
}

bool CreateView::wasClicked(GGClickEvent* evt){
	return true;
}

void CreateView::onResize(GGResizeEvent *evt){
	GGView::onResize(evt);
	
	CreateView::alignItems();
}

void CreateView::onMouseMove(GGMouseMoveEvent *evt){
	GGView::onMouseMove(evt);
	
	double zoomLevel = 0;
	
	if(CreateView::zoomBar->getCursorPos() < 50){
		zoomLevel = 0.2f + (double)(CreateView::zoomBar->getCursorPos())*(1.0f - 0.2)/50.0f;
	} else {
		zoomLevel = 1 + ((double)CreateView::zoomBar->getCursorPos() - 50.0f) * (3.0f - 1.0f)/50.0f;
	}
	
	char *str = (char*) malloc(sizeof(char) * 10);
	sprintf(str, "%d%%", (int) floor(zoomLevel * 100));
	CreateView::zoomLable->setText(new std::string(str));
	free(str);
	CreateView::grid->setZoom(zoomLevel);
	
	CreateView::alignItems();
}

void CreateView::alignItems(){
	int xPos = LARGE_PADDING;
	int yPos = LARGE_PADDING;
	int nextItemsPos = 0;

	CreateView::backButton->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::backButton->getWidth() + SMALL_PADDING;

	nextItemsPos = CreateView::nameLable->getWidth()
		+ VERY_SMALL_PADDING
		+ CreateView::name->getWidth()
		+ VERY_SMALL_PADDING
		+ CreateView::save->getWidth()
		+ SMALL_PADDING;

	if(Context::window_width - xPos - nextItemsPos - CreateView::sideP->getWidth() < 0){
		xPos = LARGE_PADDING;
		yPos += 2*SMALL_PADDING;
	}

	CreateView::nameLable->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::nameLable->getWidth() + VERY_SMALL_PADDING;
	CreateView::name->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::name->getWidth() + VERY_SMALL_PADDING;
	CreateView::save->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::save->getWidth() + SMALL_PADDING;
	
	nextItemsPos = CreateView::open->getWidth()
	+ VERY_SMALL_PADDING
	+ CreateView::openDropdown->getWidth()
	+ VERY_SMALL_PADDING
	+ CreateView::openBtn->getWidth();
	
	if(Context::window_width - xPos - nextItemsPos - CreateView::sideP->getWidth() < 0){
		xPos = LARGE_PADDING;
		yPos += 2*SMALL_PADDING;
	}
	
	CreateView::open->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::open->getWidth() + VERY_SMALL_PADDING;
	CreateView::openDropdown->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::openDropdown->getWidth() + VERY_SMALL_PADDING;
	CreateView::openBtn->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::openBtn->getWidth() + SMALL_PADDING;
	
	nextItemsPos = CreateView::rowsLable->getWidth()
		+ VERY_SMALL_PADDING
		+ CreateView::nrows->getWidth() 
		+ VERY_SMALL_PADDING 
		+ CreateView::colsLable->getWidth()
		+ VERY_SMALL_PADDING
		+ CreateView::ncols->getWidth()
		+ VERY_SMALL_PADDING
		+ CreateView::resizeBtn->getWidth()
		+ SMALL_PADDING;
	
	if(Context::window_width - xPos - nextItemsPos - CreateView::sideP->getWidth() < 0){
		xPos = LARGE_PADDING;
		yPos += 2*SMALL_PADDING;
	}
	
	CreateView::rowsLable->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::rowsLable->getWidth() + VERY_SMALL_PADDING;
	CreateView::nrows->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::nrows->getWidth() + VERY_SMALL_PADDING;
	CreateView::colsLable->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::colsLable->getWidth() + VERY_SMALL_PADDING;
	CreateView::ncols->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::ncols->getWidth() + VERY_SMALL_PADDING;
	CreateView::resizeBtn->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::resizeBtn->getWidth() + SMALL_PADDING;
	
	nextItemsPos = CreateView::zoomBar->getBigSize()
		+ SMALL_PADDING
		+ CreateView::zoomLable->getWidth();
	
	if(Context::window_width - xPos - nextItemsPos - CreateView::sideP->getWidth() < 0){
		xPos = LARGE_PADDING;
		yPos += 2*SMALL_PADDING;
	}
	
	CreateView::zoomBar->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::zoomBar->getBigSize() + VERY_SMALL_PADDING;
	CreateView::zoomLable->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::zoomLable->getWidth() + SMALL_PADDING;
	
	nextItemsPos = CreateView::drawBumpmap->getWidth();
	
	if(Context::window_width - xPos - nextItemsPos - CreateView::sideP->getWidth() < 0){
		xPos = LARGE_PADDING;
		yPos += 2*SMALL_PADDING;
	}
	CreateView::drawBumpmap->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::drawBumpmap->getWidth() + SMALL_PADDING;
	
	nextItemsPos = CreateView::dropdownMenu->getWidth();
	
	if(Context::window_width - xPos - nextItemsPos - CreateView::sideP->getWidth() < 0){
		xPos = LARGE_PADDING;
		yPos += 2*SMALL_PADDING;
	}
	CreateView::dropdownMenu->setPosition(new glm::vec2(xPos, yPos));
	xPos += CreateView::dropdownMenu->getWidth() + SMALL_PADDING;
	
	CreateView::grid->setPosition(new glm::vec2(LARGE_PADDING, yPos + 2 * SMALL_PADDING));
	
	int w = Context::window_width - CreateView::sideP->getWidth() - LARGE_PADDING * 2;
	if(w < 0) w = 0;
	CreateView::grid->setWidth(w);
	
	int h = Context::window_height - yPos - 2 * LARGE_PADDING - SMALL_PADDING;
	if(h < 0) h = 0;
	CreateView::grid->setHeight(h);
}
