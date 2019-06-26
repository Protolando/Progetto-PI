#include "mainmenu.hpp"
#include <iterator>
#include <string>
#include "color_palette.h"
#include "Create/createview.hpp"
#include "Play/select_map.hpp"

void onClickCreate(GGClickEvent *evt){
	Context::switchView(new CreateView());
}

void onClickPlay(GGClickEvent *evt){
	Context::switchView(new SelectMap());
}

//Constructor
MainMenu::MainMenu(){
	Context::renderer->set_background_color(P_WHITE);
    std::string* play_string = new std::string("play");
    std::string* create_string = new std::string("create");
    std::string* titlestr = new std::string("Progetto PI");
    
    int window_height = Context::window_height;
    int window_width = Context::window_width;
    int height_unit = window_height/6;
    
	int defsize = height_unit < 50? height_unit : 50;
    int posx = window_width/2 -
    	std::get<0>(Context::renderer->get_string_size(*titlestr, defsize))/2;

    glm::vec2* titlepos = new glm::vec2(posx, height_unit);
    glm::vec4* titlecolor = new glm::vec4(P_DARK_BLACK);
    MainMenu::title = new Lable(titlestr, defsize, titlecolor, titlepos);
    MainMenu::addView(MainMenu::title);
    
    posx = window_width/2 -
    	std::get<0>(Context::renderer->get_string_size(*play_string, defsize))/2;
    glm::vec2* play_start = new glm::vec2(posx, height_unit*3);
    
    posx = window_width/2 -
    	std::get<0>(Context::renderer->get_string_size(*create_string, defsize))/2;
    glm::vec2* create_start = new glm::vec2(posx, height_unit*4);
    
    MainMenu::playBtn = new Button(play_string, play_start, defsize);
    MainMenu::playBtn->setColor(new glm::vec4(P_LIGHT_BLUE));
    MainMenu::playBtn->setClickedColor(new glm::vec4(P_DARK_BLUE));
    MainMenu::playBtn->addMouseUp(&onClickPlay);
    GGView::addView(MainMenu::playBtn);
    MainMenu::createBtn = new Button(create_string, create_start, defsize);
    MainMenu::createBtn->setColor(new glm::vec4(P_LIGHT_BLUE));
    MainMenu::createBtn->setClickedColor(new glm::vec4(P_DARK_BLUE));
    MainMenu::createBtn->addMouseUp(&onClickCreate);
    GGView::addView(MainMenu::createBtn);
}

MainMenu::~MainMenu(){
	delete MainMenu::playBtn;
	delete MainMenu::createBtn;
	delete MainMenu::title;
}

void MainMenu::onResize(GGResizeEvent* evt){
	int window_height = evt->getHeight();
    int window_width = evt->getWidth();
    int height_unit = window_height/6;
    
	int defsize = height_unit < 50? height_unit : 50;
    
    //Setting title size and positionint defsize = height_unit < 50? height_unit : 50;
    int posx = window_width/2 -
    	std::get<0>(Context::renderer->get_string_size(
    		*(MainMenu::title->getText()), defsize)
    	)/2;
    glm::vec2* titlepos = new glm::vec2(posx, height_unit);
    
    MainMenu::title->setSize(defsize);
    MainMenu::title->setPosition(titlepos);
    //Setting play button size and position
	posx = window_width/2 -
    	std::get<0>(Context::renderer->get_string_size(
    				*(MainMenu::playBtn->getLable()->getText()),
    				defsize)
    	)/2;
    glm::vec2* play_start = new glm::vec2(posx, height_unit*3);
    MainMenu::playBtn->setPosition(play_start);
    MainMenu::playBtn->setHeight(defsize);
    
    //Setting create button size and position
    posx = window_width/2 -
    	std::get<0>(Context::renderer->get_string_size(
    				*(MainMenu::createBtn->getLable()->getText()),
    				defsize)
    	)/2;
    glm::vec2* create_start = new glm::vec2(posx, height_unit*4);
    MainMenu::createBtn->setPosition(create_start);
    MainMenu::createBtn->setHeight(defsize);
    
    GGView::onResize(evt);
}

bool MainMenu::wasClicked(GGClickEvent* evt){
	return true;
}
