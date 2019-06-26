#ifndef PLAYVIEWINC
#define PLAYVIEWINC
#include "../Context/context.hpp"
#include "../Context/Views/GGView.hpp"
#include "../Context/Views/button.hpp"
#include "character.hpp"
#include "gameMap.hpp"
#include "playercharacter.hpp"
#include "bullet.hpp"
#include "hpbar.hpp"

class PlayView : public GGView{
public : 
	PlayView(std::string *);
	~PlayView();
	
	void draw() override;
	void viewLogic() override;
	
	bool onKeyUp(GGKeyboardEvent *) override;
	bool onKeyDown(GGKeyboardEvent *) override;
	
	void onResize(GGResizeEvent *) override;
private : 
	bool moving = false;
	double shootingtimer = 0.0f;
	PlayerCharacter *player;
	bool wasClicked(GGClickEvent *) override;
	bool onMouseUp(GGClickEvent *) override;
	glm::vec2 w2v(glm::vec2);
	glm::vec2 v2w(glm::vec2);
	Rect w2v(Rect);
	Rect v2w(Rect);
	glm::mat4 view;
	GameMap *gameMap;
	std::vector<Bullet *> FriendlyBullets;
	std::vector<Bullet *> EnemyBullets;
	std::vector<Character *> enemies;
	HpBar *hpbar;
	bool gameOver;
	Button *mainMenu;
	Button *restart;
	std::string *mapname;
	bool debugging = false;
	bool pause = false;
	bool wonned = false; //lol xd
};

#endif
