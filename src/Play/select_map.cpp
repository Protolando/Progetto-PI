#include "select_map.hpp"
#include "../mainmenu.hpp"
#include "../color_palette.h"
#include "../def.hpp"
#include "playview.hpp"
#include <dirent.h>
#define BACK "back"
#define MAP_NAME_PADDING 20
#define SELECT_MAP "Select a map"

SelectMap::SelectMap(){
	SelectMap::selectMapLable = new Lable(
		new std::string(SELECT_MAP),
		MAP_NAME_PADDING*2,
		new glm::vec4(BLACK),
		new glm::vec2(0, 0)
	);
	SelectMap::selectMapLable->setPosition(
		new glm::vec2(
			Context::window_width/2 - SelectMap::selectMapLable->getWidth()/2,
			MAP_NAME_PADDING*3
		)
	);
	GGView::addView(SelectMap::selectMapLable);
	
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (MAPSPATH)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if(strncmp(ent->d_name, ".", 1) != 0){
				Lable *tmp = new Lable(
					new std::string(ent->d_name),
					MAP_NAME_PADDING,
					new glm::vec4(BLACK),
					new glm::vec2(0, 0)
				);
				SelectMap::map_list.push_back(tmp);
			}
		}
		closedir(dir);
	}
	
	SelectMap::mapNamesWidth = 0;
	
	for(auto i = SelectMap::map_list.begin(); i != SelectMap::map_list.end(); i++){
		if((*i)->getWidth() > SelectMap::mapNamesWidth)
			SelectMap::mapNamesWidth = (*i)->getWidth();
	}
	
	int xPos = (Context::window_width - SelectMap::mapNamesWidth)/2;
	int yPos = SelectMap::selectMapLable->getHeight()
		+ SelectMap::selectMapLable->getPosition().y
		+ 3*MAP_NAME_PADDING;

	for(auto i = SelectMap::map_list.begin(); i != SelectMap::map_list.end(); i++){
		(*i)->setPosition(new glm::vec2(xPos, yPos));
		yPos += 2*MAP_NAME_PADDING;
		GGView::addView((*i));
	}

	SelectMap::backButton = new Button(
		new std::string(BACK),
		new glm::vec2(0, 0),
		Context::window_height/20
	);
	SelectMap::backButton->setPosition(
		new glm::vec2(
			Context::window_width/2 - SelectMap::backButton->getWidth()/2,
			yPos + MAP_NAME_PADDING*2 
		)
	);
	SelectMap::backButton->setColor(new glm::vec4(P_LIGHT_BLUE));
    SelectMap::backButton->setClickedColor(new glm::vec4(P_DARK_BLUE));
	GGView::addView(SelectMap::backButton);
}

SelectMap::~SelectMap(){	
	for(auto i = SelectMap::map_list.begin(); i != SelectMap::map_list.end(); i++){
		delete (*i);
	}
	
	delete SelectMap::backButton;
}

void SelectMap::draw(){
	int xPos = (Context::window_width - SelectMap::mapNamesWidth)/2;
	int yPos = SelectMap::selectMapLable->getHeight()
		+ SelectMap::selectMapLable->getPosition().y
		+ 3*MAP_NAME_PADDING;
	
	for(int i = 1; i < (int) SelectMap::map_list.size(); i++){
		glm::vec4 color;
	
		if((i-1)%2 == 0) {
			color = glm::vec4(P_DARKISH_WHITE);
		} else {
			color = glm::vec4(P_MORE_DARKISH_WHITE); 
		}
		
		Context::renderer->fill_rect(
			glm::vec2(
				xPos - MAP_NAME_PADDING/2,
				2*MAP_NAME_PADDING*(i - 1) + yPos - MAP_NAME_PADDING/2
			),
			glm::vec2(
				xPos + SelectMap::mapNamesWidth + MAP_NAME_PADDING/2,
				2*MAP_NAME_PADDING*i + yPos - MAP_NAME_PADDING/2
			),
			color
		);
	
		Context::renderer->draw_line(
			glm::vec2(
				xPos - MAP_NAME_PADDING/2,
				2*MAP_NAME_PADDING*i + yPos - MAP_NAME_PADDING/2
			),
			glm::vec2(
				xPos + SelectMap::mapNamesWidth + MAP_NAME_PADDING/2,
				2*MAP_NAME_PADDING*i + yPos - MAP_NAME_PADDING/2
			),
			glm::vec4(P_DARK_BLACK)
		);
	}
	
	Context::renderer->draw_rect(
		glm::vec2(xPos - MAP_NAME_PADDING/2, yPos - MAP_NAME_PADDING/2),
		glm::vec2(
			xPos + SelectMap::mapNamesWidth + MAP_NAME_PADDING/2,
			yPos + 2*MAP_NAME_PADDING * SelectMap::map_list.size() - MAP_NAME_PADDING/2
		),
		glm::vec4(P_DARK_BLACK)
	);
	
	GGView::draw();	
}
	
bool SelectMap::onMouseUp(GGClickEvent *evt){
	int x = evt->getX();
	int y = evt->getY();
	
	int yPos = SelectMap::selectMapLable->getHeight()
		+ SelectMap::selectMapLable->getPosition().y
		+ 2*MAP_NAME_PADDING;
	int xPos = (Context::window_width - SelectMap::mapNamesWidth)/2;

	if(SelectMap::backButton->onMouseUp(evt)){
		Context::switchView(new MainMenu());
	} else if(x > xPos - MAP_NAME_PADDING/2 &&
		x < SelectMap::mapNamesWidth + xPos + MAP_NAME_PADDING/2 &&
		y > yPos + MAP_NAME_PADDING/2 &&
		y < 2*MAP_NAME_PADDING*((int)SelectMap::map_list.size()) + yPos + MAP_NAME_PADDING/2
		){
		std::string mapname = *(SelectMap::map_list.at(
			(int) ceil((y - yPos)/(3*MAP_NAME_PADDING))
		)->getText());
		
		Context::switchView(new PlayView(new std::string(mapname)));
		
		Context::setCursorToNormal();
	}
	
	return true;
}

bool SelectMap::wasClicked(GGClickEvent *){
	return true;
}

void SelectMap::onMouseMove(GGMouseMoveEvent *evt){
	int x = evt->getX();
	int y = evt->getY();
	
	int yPos = SelectMap::selectMapLable->getHeight()
		+ SelectMap::selectMapLable->getPosition().y
		+ 3*MAP_NAME_PADDING;
	int xPos = (Context::window_width - SelectMap::mapNamesWidth)/2;
	
	if( x > xPos - MAP_NAME_PADDING/2 &&
		x < SelectMap::mapNamesWidth + xPos + MAP_NAME_PADDING/2 &&
		y > yPos - MAP_NAME_PADDING/2 &&
		y < 2*MAP_NAME_PADDING*((int)SelectMap::map_list.size()) + yPos - MAP_NAME_PADDING/2
		)
		Context::setCursorToClick();
	else
		Context::setCursorToNormal();
}

void SelectMap::onResize(GGResizeEvent *evt){
	SelectMap::selectMapLable->setPosition(
		new glm::vec2(
			Context::window_width/2 - SelectMap::selectMapLable->getWidth()/2,
			MAP_NAME_PADDING*3
		)
	);

	int yPos = SelectMap::selectMapLable->getHeight()
		+ SelectMap::selectMapLable->getPosition().y
		+ 3*MAP_NAME_PADDING;
	int xPos = (Context::window_width - SelectMap::mapNamesWidth)/2;
	
	for(auto i = SelectMap::map_list.begin(); i != SelectMap::map_list.end(); i++){
		(*i)->setPosition(new glm::vec2(xPos, yPos));
		yPos += 2*MAP_NAME_PADDING;
	}
	
	SelectMap::backButton->setPosition(
		new glm::vec2(
			Context::window_width/2 - SelectMap::backButton->getWidth()/2,
			yPos + MAP_NAME_PADDING*2 
		)
	);
}
