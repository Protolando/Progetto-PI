#include "hpbar.hpp"
#define THICKNESS ceil(HpBar::height/10)
#define WIDTH Context::window_width/4
#define HEIGHT Context::window_height/20

HpBar::HpBar(int tothp, glm::vec2 *pos) : 
HpBar(tothp, pos, WIDTH, HEIGHT){}

HpBar::HpBar(int tothp, glm::vec2 *pos, int width, int height){
	HpBar::tothp = tothp;
	HpBar::currhp = tothp;
	HpBar::position = pos;
	HpBar::width = width;
	HpBar::height = height;
}

HpBar::~HpBar(){
	delete HpBar::position;
}

void HpBar::draw(){
	Context::renderer->fill_rect(
		*HpBar::position,
		glm::vec2(
			HpBar::position->x + HpBar::width,
			HpBar::position->y + HpBar::height
		),
		glm::vec4(1.0f, 0.9f, 0.9f, 1.0f)
	);

	Context::renderer->fill_rect(
		*HpBar::position,
		glm::vec2(
			HpBar::position->x + ((float)currhp/(float)tothp)*HpBar::width,
			HpBar::position->y + HpBar::height
		),
		glm::vec4(RED)
	);
	
	Context::renderer->draw_rect(
		*HpBar::position,
		glm::vec2(
			HpBar::position->x + HpBar::width,
			HpBar::position->y + HpBar::height
		),
		glm::vec4(BLACK),
		(GLfloat) THICKNESS
	);
}

void HpBar::changeHp(int amount){
	HpBar::currhp += amount;
	if(HpBar::currhp > HpBar::tothp)
		HpBar::currhp = HpBar::tothp;
}

void HpBar::setHp(int hp){
	if(hp > HpBar::tothp)
		return;
	HpBar::currhp = hp;
}

bool HpBar::wasClicked(GGClickEvent*){
	return false;
}

void HpBar::setPosition(glm::vec2* pos){
	if(HpBar::position != NULL)
		delete HpBar::position;
	HpBar::position = pos;
}

int HpBar::getWidth(){
	return width;
}

int HpBar::getHeight(){
	return height;
}

void HpBar::setWidth(int width){
	HpBar::width = width;
}

void HpBar::setHeight(int height){
	HpBar::height = height;
}
