#include "playview.hpp"
#include "../def.hpp"
#include "../Context/colors.hpp"
#include "../mainmenu.hpp"
#include "slimeenemy.hpp"
#include "../Context/rect.hpp"
#define TILESIZE 50
#include "hpbar.hpp"
#include "../color_palette.h"
#include "../mainmenu.hpp"
#define MAINMENU "Main Menu"
#define RESTART "Restart"

PlayView::PlayView(std::string *mapname){
	/*
	 * 1) Open Map +
	 * 2) Initialize Map +
	 * 3) Initialize HUD ~
	 * 4) Initialize Character +
	 * 5) Initialize enemies +
	 * 6) Spawn character and enemies ~
	 */
	srand (time(NULL));
	glfwSetTime(0.0f);
	Context::renderer->set_background_color(glm::vec4(BLACK));
	Context::unsubscribeTextInput();
	PlayView::moving = false;
	PlayView::player = new PlayerCharacter();
	PlayView::gameMap = new GameMap(std::string(MAPSPATH) + "/" + *mapname);
	
	PlayView::view = glm::mat4(1.0f);
	
	//Spawn Enemies
	for(int i = 0; i < 1; i++){
		Character *enem = new SlimeEnemy("/home/protolando/tmp/slime.png", glm::vec2(0.0f, 0.0f));
		bool placed = false;
		while(!placed){
			enem->setPosition({
				TILESIZE * (rand() % PlayView::gameMap->getWidth()),
				TILESIZE * (rand() % PlayView::gameMap->getHeight())
			});
			
			if(PlayView::gameMap->isWalkable(enem->getHitbox()) &&
				!Rect::intersect(PlayView::player->getHitbox(), enem->getHitbox())
			){
				PlayView::enemies.push_back(enem);
				placed = true;
			}
		}
	}
	PlayView::gameOver = false;
	PlayView::hpbar = new HpBar(Character::maxHp, new glm::vec2(0.0f, 0.0f));
	PlayView::hpbar->setPosition(
		new glm::vec2(
			10.0f,
			Context::window_height-(PlayView::hpbar->getHeight() + 10)
		)
	);
	PlayView::mainMenu = new Button(
		new std::string(MAINMENU), 
		new glm::vec2(Context::window_width/4, 3*Context::window_height/6),
		Context::window_height/24
	);
	
    PlayView::mainMenu->setColor(new glm::vec4(P_GREEN));
    PlayView::mainMenu->setClickedColor(new glm::vec4(P_DARK_GREEN));
	GGView::addView(PlayView::mainMenu);
	
	PlayView::restart = new Button(
		new std::string(RESTART),
		new glm::vec2(
			2*Context::window_width/4 + PlayView::mainMenu->getWidth(),
			3*Context::window_height/6
		),
		Context::window_height/24
	);
	PlayView::restart->setColor(new glm::vec4(P_GREEN));
    PlayView::restart->setClickedColor(new glm::vec4(P_DARK_GREEN));
	GGView::addView(PlayView::restart);
	
	PlayView::mapname = mapname;
}

PlayView::~PlayView(){
	Context::subscribeTextInput();
	delete PlayView::player;
	delete PlayView::gameMap;
	for(auto i = PlayView::FriendlyBullets.begin(); i != PlayView::FriendlyBullets.end(); i++){
		delete *i;
	}
	for(auto i = PlayView::enemies.begin(); i != PlayView::enemies.end(); i++){
		delete *i;
	}
	delete PlayView::hpbar;
	delete PlayView::mapname;
}

glm::vec2 PlayView::v2w(glm::vec2 vect){
	glm::vec4 tmp = PlayView::view*glm::vec4(vect, 0.0f, 1.0f);
	return glm::vec2(tmp.x, tmp.y);
}

glm::vec2 PlayView::w2v(glm::vec2 vect){
	glm::vec4 tmp = glm::inverse(PlayView::view) * glm::vec4(vect, 0.0f, 1.0f);
	return glm::vec2(tmp.x, tmp.y);
}

Rect PlayView::w2v(Rect r){
	Rect ret = r.clone();
	ret.transform(glm::inverse(PlayView::view));
	return ret;
}
Rect PlayView::v2w(Rect r){
	Rect ret = r.clone();
	ret.transform(PlayView::view);
	return ret;
}

void PlayView::draw(){
	PlayView::gameMap->draw(PlayView::view);
	for(auto i = PlayView::enemies.begin(); i != PlayView::enemies.end(); i++){
		(*i)->draw(PlayView::view);
	}
	for(auto i = PlayView::FriendlyBullets.begin(); i != PlayView::FriendlyBullets.end(); i++){
		(*i)->draw(PlayView::view);
	}
	for(auto i = PlayView::EnemyBullets.begin(); i != PlayView::EnemyBullets.end(); i++){
		(*i)->draw(PlayView::view);
	}
	PlayView::player->draw(PlayView::view);
	
	if(PlayView::debugging){
		Context::renderer->draw_rect(
			PlayView::player->getHitbox(),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
		);
		
		for(auto i = PlayView::FriendlyBullets.begin(); i != PlayView::FriendlyBullets.end(); i++){
			Context::renderer->draw_rect(
				v2w((*i)->getHitbox()),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
			);
		}
		
		for(auto i = PlayView::EnemyBullets.begin(); i != PlayView::EnemyBullets.end(); i++){
			Context::renderer->draw_rect(
				v2w((*i)->getHitbox()),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
			);
		}
	
		for(auto i = PlayView::enemies.begin(); i != PlayView::enemies.end(); i++){
			Context::renderer->draw_rect(
				v2w((*i)->getHitbox()),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
			);
		}
	}
	
	//Draw hud
	PlayView::hpbar->draw();
	
	//Enemies number
	char *tmp = (char*) malloc(sizeof(char)*10);
	sprintf(tmp, "%d",(int)PlayView::enemies.size());
	std::string remainingEnemies(tmp);
	double rightpos = Context::renderer->get_string_size(
		remainingEnemies.c_str(),
		Context::window_height/10
	).first;
	
	Context::renderer->fill_rect(
		glm::vec2(Context::window_width - rightpos, Context::window_height - Context::window_height/10),
		glm::vec2(Context::window_width, Context::window_height),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.2f)
	);
	Context::renderer->draw_text(
		remainingEnemies,
		glm::vec2(Context::window_width - rightpos, Context::window_height - Context::window_height/10),
		BLACK,
		Context::window_height/10
	);
	free(tmp);
	
	if(PlayView::gameOver || PlayView::pause || PlayView::wonned){
		Context::renderer->fill_rect(
			glm::vec2(0.0f, Context::window_height/3),
			glm::vec2(Context::window_width, 2*Context::window_height/3),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.5f)
		);
		
		std::string str;
		glm::vec4 color;
		if(PlayView::gameOver){
			str = "You Died";
			color = P_RED;
		} else if(PlayView::wonned){
			str = "You Won";
			color = P_GREEN;	
		} else if(PlayView::pause){
			str = "Pause";
			color = P_GREEN;
		}
		
		glm::vec2 strpos = glm::vec2(
			Context::window_width/2-Context::renderer->get_string_size(
				str.c_str(),
				Context::window_height/6
			).first/2,
			2*Context::window_height/6
		);
		
		Context::renderer->draw_text(
			str,
			strpos,
			color,
			Context::window_height/6
		);
			
		PlayView::mainMenu->draw();
		PlayView::restart->draw();
	}
}

bool PlayView::onKeyDown(GGKeyboardEvent *evt){
	if(PlayView::gameOver)
		return true;

	if(!PlayView::moving){
		glfwSetTime(0.0f);
		PlayView::moving = true;
	}
	
	return true;
}

bool PlayView::onKeyUp(GGKeyboardEvent *evt){
	if(evt->getKey() == GLFW_KEY_ESCAPE){
		PlayView::pause = !PlayView::pause;
		glfwSetTime(0.0f);
	} else if(evt->getKey() == GLFW_KEY_F2){
		PlayView::debugging = !PlayView::debugging;
		PlayView::gameMap->toggleTileNumber();
	}
	if(PlayView::gameOver || PlayView::pause)
		return true;

	if (glfwGetKey(Context::window, GLFW_KEY_W) == GLFW_RELEASE &&
		glfwGetKey(Context::window, GLFW_KEY_A) == GLFW_RELEASE &&
		glfwGetKey(Context::window, GLFW_KEY_S) == GLFW_RELEASE &&
		glfwGetKey(Context::window, GLFW_KEY_D) == GLFW_RELEASE
	){
		PlayView::moving = false;
	}
			
	return true;
}

bool PlayView::wasClicked(GGClickEvent *){
	return false;
}

void PlayView::viewLogic(){
	if(PlayView::gameOver || PlayView::pause || PlayView::wonned)
		return;

	double time = glfwGetTime();
	glfwSetTime(0.0f);
	
	PlayView::player->update(time);
	
	for(auto id = PlayView::enemies.begin(); id != PlayView::enemies.end(); id++){
		(*id)->update(time);
		
		if(!(*id)->isAnimating()){
			int direction = rand() % 4;

			glm::vec2 enemystepsize(0.0f);
			
			if(direction == 0){ //up
				enemystepsize.y = -(*id)->getStepSize(0.2f);
			} else if(direction == 1){ //left
				enemystepsize.x = -(*id)->getStepSize(0.2f);
			} else if(direction == 2){ //down
				enemystepsize.y = (*id)->getStepSize(0.2f);
			} else if(direction == 3){ //right
				enemystepsize.x = (*id)->getStepSize(0.2f);
			}
			
			Rect enhitbox = (*id)->getHitbox().clone();
			enhitbox.translate(glm::vec2(enemystepsize));
			
			if(PlayView::gameMap->isWalkable(enhitbox)){
				switch(direction){
					case 0: (*id)->moveUp(time); break;
					case 1: (*id)->moveLeft(time); break;
					case 2: (*id)->moveDown(time); break;
					case 3: (*id)->moveRight(time); break;
				}
			}
			
			if(rand()%10 <= 1)
				EnemyBullets.push_back((*id)->shoot(
					glm::mat4(1.0f),
					w2v(PlayView::player->getPosition())
			));
		}
	}
	
	//Move FriendlyBullets and remove dead ones
	int i = 0;
	while(i < (int) PlayView::FriendlyBullets.size()){
		bool deleteme = false;
		Bullet *b = PlayView::FriendlyBullets.at(i);
		b->move(time);
		if(!PlayView::gameMap->isWalkable(b->getHitbox())){
			deleteme = true;
		} else {
			int j = 0;
			while(!deleteme && j < (int) enemies.size()){
				Character *e = PlayView::enemies.at(j);
				if(Rect::intersect(b->getHitbox(), e->getHitbox())){
					//Enemy e was hit
					e->changeHp(-b->getDamage());
					if(e->isDead()){
						delete enemies.at(j);
						PlayView::enemies.erase(enemies.begin() + j);
						deleteme = true;
					} else{
						j++;
					}
					if((int) PlayView::enemies.size() == 0)
						PlayView::wonned = true;
				} else {
					j++;
				}
			}
		}
		
		if(deleteme){
			delete FriendlyBullets.at(i);
			FriendlyBullets.erase(FriendlyBullets.begin() + i);
		} else {
			i++;
		}	
	}
	
	//Move/delete enemy bullets
	i = 0;
	while(i < (int) PlayView::EnemyBullets.size()){
		bool deleteme = false;
		Bullet *b = PlayView::EnemyBullets.at(i);
		b->move(time);
		if(!PlayView::gameMap->isWalkable(b->getHitbox())){
			deleteme = true;
		} else {
			if(Rect::intersect(b->getHitbox(), w2v(PlayView::player->getHitbox()))){
				//Enemy e was hit
				if(!PlayView::player->isInvulnerable()){
					PlayView::player->changeHp(-b->getDamage());
					PlayView::hpbar->changeHp(-b->getDamage());
				}
				if(PlayView::player->isDead()){
					PlayView::gameOver = true;
					PlayView::mainMenu->setColor(new glm::vec4(P_RED));
					PlayView::mainMenu->setClickedColor(new glm::vec4(P_DARK_RED));
					PlayView::restart->setColor(new glm::vec4(P_RED));
					PlayView::restart->setClickedColor(new glm::vec4(P_DARK_RED));
				}
				deleteme = true;
				}
			}
		
		if(deleteme){
			delete EnemyBullets.at(i);
			EnemyBullets.erase(EnemyBullets.begin() + i);
		} else {
			i++;
		}	
	}
	
	double playerstepsize = PlayView::player->getStepSize(time);
	glm::vec2 playerdirection(0.0f, 0.0f);

	if(glfwGetKey(Context::window, GLFW_KEY_W) == GLFW_PRESS){
		playerdirection.y -= 1.0f;
	}
	if(glfwGetKey(Context::window, GLFW_KEY_A) == GLFW_PRESS){
		playerdirection.x -= 1.0f;
	}
	if(glfwGetKey(Context::window, GLFW_KEY_S) == GLFW_PRESS){
		playerdirection.y +=1.0f;
	}
	if(glfwGetKey(Context::window, GLFW_KEY_D) == GLFW_PRESS){
		playerdirection.x +=1.0f;
	}
	
	Rect playerhitbox = PlayView::player->getHitbox();
	playerhitbox.translate(glm::vec2(
				playerstepsize*playerdirection.x,
				playerstepsize*playerdirection.y
	));
	playerhitbox = w2v(playerhitbox);
	
	if(PlayView::gameMap->isWalkable(playerhitbox)){
		PlayView::view = glm::translate(
			PlayView::view, glm::vec3(
				-playerstepsize*playerdirection.x,
				-playerstepsize*playerdirection.y,
				0.0f
			)
		);
		if(playerdirection.x == 1.0f)
			PlayView::player->moveRight(time);
		if(playerdirection.x == -1.0f)
			PlayView::player->moveLeft(time);
		if(playerdirection.y == 1.0f)
			PlayView::player->moveDown(time);
		if(playerdirection.y == -1.0f)
			PlayView::player->moveUp(time);
	} 
	
	for(auto i = PlayView::enemies.begin(); i != PlayView::enemies.end(); i++){
		if(Rect::intersect(playerhitbox, (*i)->getHitbox())){
			if(!PlayView::player->isInvulnerable()){
				PlayView::player->changeHp(-20);
				PlayView::hpbar->changeHp(-20);
			}

			if(PlayView::player->isDead()){
				PlayView::gameOver = true;
				PlayView::mainMenu->setColor(new glm::vec4(P_RED));
				PlayView::mainMenu->setClickedColor(new glm::vec4(P_DARK_RED));
				PlayView::restart->setColor(new glm::vec4(P_RED));
				PlayView::restart->setClickedColor(new glm::vec4(P_DARK_RED));
			}
		}
	}
	
	//if(glfwGetKey(Context::window, GLFW_KEY_SPACE) == GLFW_PRESS){
	if(glfwGetMouseButton(Context::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		PlayView::shootingtimer += time;
		if(PlayView::shootingtimer * PlayView::player->getFireRate() >= 1){
			PlayView::FriendlyBullets.push_back(PlayView::player->shoot(glm::inverse(PlayView::view), Context::getCursorPos()));
			PlayView::shootingtimer = 0.0f;
		}
	}
	
	
	return;
}

void PlayView::onResize(GGResizeEvent *evt){
	glm::vec2 translation = 
		glm::vec2(evt->getWidth()/2, evt->getHeight()/2) - 
		glm::vec2(PlayView::player->getPosition());
	PlayView::view = glm::translate(PlayView::view, glm::vec3((translation), 0.0f));
	PlayView::player->setPosition({Context::window_width/2, Context::window_height/2});
	
	PlayView::hpbar->setPosition(
		new glm::vec2(
			10.0f,
			Context::window_height-(PlayView::hpbar->getHeight() + 10)
		)
	);
	
	PlayView::mainMenu->setPosition(
		new glm::vec2(Context::window_width/4, 3*Context::window_height/6)
	);
    PlayView::mainMenu->setHeight(Context::window_height/24);
	PlayView::mainMenu->resetWidth();
    
	PlayView::restart->setPosition(
		new glm::vec2(
			2*Context::window_width/4 + PlayView::mainMenu->getWidth(),
			3*Context::window_height/6
		)
	);
	PlayView::restart->setHeight(Context::window_height/24);
	PlayView::restart->resetWidth();
}

bool PlayView::onMouseUp(GGClickEvent *evt){
	if(PlayView::mainMenu->onMouseUp(evt)){
		Context::switchView(new MainMenu());
	} else if(PlayView::restart->onMouseUp(evt)){
		Context::switchView(new PlayView(new std::string(*mapname)));
	}
	
	return true;
}
