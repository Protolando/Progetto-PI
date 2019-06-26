#include "createsidepanel.hpp"
#include "../color_palette.h"
#define SIDEPANELPADDING 15

#include <dirent.h>
#include "../def.hpp"
#include <iostream>

CreateSidePanel::CreateSidePanel(int width, glm::vec4* color) :
	SidePanel(width, color){
	CreateSidePanel::currentTexSelected = NULL;
	
	CreateSidePanel::tb = new Textbox(
		new glm::vec2(
			Context::window_width - SidePanel::getWidth() + SIDEPANELPADDING/2,
			20),
		SidePanel::getWidth() - SIDEPANELPADDING, 15);
	GGView::addView(CreateSidePanel::tb);
	
	CreateSidePanel::submitButton = new Button(
			new std::string("Submit"),
			new glm::vec2(
				Context::window_width -
				SidePanel::getWidth() + 
				SIDEPANELPADDING/2,
				45
			),
			15
		);
	CreateSidePanel::submitButton->setColor(new glm::vec4(P_LIGHT_BLUE));
	CreateSidePanel::submitButton->adaptClickedColor();
	CreateSidePanel::addView(CreateSidePanel::submitButton);
	
	CreateSidePanel::clearButton = new Button(
			new std::string("Clear"),
			new glm::vec2(
				Context::window_width -
				SidePanel::getWidth() + 
				SIDEPANELPADDING +
				CreateSidePanel::submitButton->getWidth()
				,
				45
			),
			15
		);
	CreateSidePanel::clearButton->setColor(new glm::vec4(P_LIGHT_BLUE));
	CreateSidePanel::clearButton->adaptClickedColor();
	CreateSidePanel::addView(CreateSidePanel::clearButton);
	
	glm::vec2* posGallery = new glm::vec2(
		Context::window_width - SidePanel::getWidth() + 10,
		70
	);
	
	CreateSidePanel::gallery = new Gallery(
		posGallery,
		SidePanel::getWidth() - 20,
		Context::window_height - 80
	);
	GGView::addView(CreateSidePanel::gallery);
	CreateSidePanel::gallery->setScrollbarColors(
		new glm::vec4(P_DARK_BLUE),
		new glm::vec4(P_LIGHT_BLUE)
	);
	CreateSidePanel::gallery->setColor(new glm::vec4(P_DARK_BLACK));
	
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (TEXPATH)) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {
		std::string *pathadd = new std::string(std::string(TEXPATH) + "/" + ent->d_name);
		std::string *filenameadd = new std::string(ent->d_name);
		glm::vec4 *coloradd = new glm::vec4(WHITE);
		glm::vec2 *posadd = new glm::vec2(100, 100); 
	  	try{
		  	GalleryItem *itm = new GalleryItem(
		  		pathadd,
				filenameadd,
				posadd,
				100,
				100,
				coloradd
			);
			CreateSidePanel::gallery->addItem(itm);
			itm = NULL;
		} catch(const char* s){
			delete pathadd;
			delete filenameadd;
			delete coloradd;
			delete posadd;
		}
	  }
	  closedir (dir);
	}
}

CreateSidePanel::~CreateSidePanel(){
	delete CreateSidePanel::tb;
	delete CreateSidePanel::submitButton;
	delete CreateSidePanel::clearButton;
	delete CreateSidePanel::gallery;
}

void CreateSidePanel::onResize(GGResizeEvent* evt){
	SidePanel::onResize(evt);
	
	CreateSidePanel::tb->setWidth(
		SidePanel::getWidth() - SIDEPANELPADDING
	);
	CreateSidePanel::tb->setPosition(
		new glm::vec2(
			evt->getWidth() - SidePanel::getWidth() + SIDEPANELPADDING/2,
			20
		)
	);
	
	CreateSidePanel::submitButton->setPosition(
		new glm::vec2(
			Context::window_width -
			SidePanel::getWidth() + 
			SIDEPANELPADDING/2,
			45
		)
	);
	
	CreateSidePanel::clearButton->setPosition(
		new glm::vec2(
			Context::window_width -
			SidePanel::getWidth() + 
			SIDEPANELPADDING +
			CreateSidePanel::submitButton->getWidth(),
			45
		)
	);
	
	CreateSidePanel::gallery->setPosition(
		new glm::vec2(
			Context::window_width - SidePanel::getWidth() + 10,
			70
		)
	);
	
	CreateSidePanel::gallery->setWidth(SidePanel::getWidth() - 10);
	CreateSidePanel::gallery->setHeight(Context::window_height - 80);
}

void CreateSidePanel::onMouseMove(GGMouseMoveEvent* evt){
	SidePanel::onMouseMove(evt);
	
	if(SidePanel::isResizing()){
		CreateSidePanel::tb->setWidth(
			SidePanel::getWidth() - SIDEPANELPADDING
		);
		CreateSidePanel::tb->setPosition(
			new glm::vec2(
				Context::window_width - SidePanel::getWidth() + SIDEPANELPADDING/2,
				20
			)
		);
		
		CreateSidePanel::submitButton->setPosition(
			new glm::vec2(
				Context::window_width -
				SidePanel::getWidth() + 
				SIDEPANELPADDING/2,
				45
			)
		);
		
		CreateSidePanel::clearButton->setPosition(
			new glm::vec2(
				Context::window_width -
				SidePanel::getWidth() + 
				SIDEPANELPADDING +
				CreateSidePanel::submitButton->getWidth(),
				45
			)
		);
			
		CreateSidePanel::gallery->setPosition(
			new glm::vec2(
				Context::window_width - SidePanel::getWidth() + 10,
				70
			)
		);
		
		CreateSidePanel::gallery->setWidth(SidePanel::getWidth() - 20);
		CreateSidePanel::gallery->setHeight(Context::window_height - 80);
	}
}

bool CreateSidePanel::onMouseUp(GGClickEvent* evt){
	if(!SidePanel::onMouseUp(evt))
		return false;
	
	if(CreateSidePanel::clearButton->onMouseUp(evt)){
		CreateSidePanel::tb->resetText();
	} else if(CreateSidePanel::submitButton->onMouseUp(evt)){
		std::string filename = CreateSidePanel::tb->getText().substr(
			CreateSidePanel::tb->getText().find_last_of("/")+1,
			CreateSidePanel::tb->getText().size()
		);
		std::string path = std::string(TEXPATH) + "/" + filename;
		Context::copyFile(CreateSidePanel::tb->getText().c_str(), path.c_str());
		std::string *pathadd = new std::string(path);
		std::string *filenameadd = new std::string(filename);
		glm::vec4 *color = new glm::vec4(WHITE);
		glm::vec2 *posadd = new glm::vec2(100, 100); 
		try{
			GalleryItem* tmp = new GalleryItem(
				pathadd,
				filenameadd,
				posadd,
				100,
				100,
				color
			);
			CreateSidePanel::gallery->addItem(tmp);
			CreateSidePanel::tb->removeError();
		} catch(const char* s){
			CreateSidePanel::tb->displayError();
			delete pathadd;
			delete filenameadd;
			delete color;
			delete posadd;
		}
	} else if(CreateSidePanel::gallery->onMouseUp(evt)){
		GalleryItem* sel = CreateSidePanel::gallery->getSelected();
		if(sel != NULL){
			CreateSidePanel::currentTexSelected = sel->getTexture();
			}
		else
			CreateSidePanel::currentTexSelected = NULL;
	}
	
	return true;
}

std::vector<texture_t *> CreateSidePanel::getTextures(){
	return CreateSidePanel::gallery->getTextures();
}
