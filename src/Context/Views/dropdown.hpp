#ifndef DROPDOWNINC
#define DROPDOWNINC
#include "GGView.hpp"
#include <vector>
#include "lable.hpp"

class Dropdown : public GGView{
public : 
	Dropdown(glm::vec2 *, int, int);
	~Dropdown();
	
	void draw() override;
	
	void addElement(std::string *);
	void activate();
	void deactivate();
	bool isExpanded();
	bool isActive();
	
	std::string getCurrentChoice();
	
	void setPosition(glm::vec2 *);
	void setWidth(int);
	int getWidth();
	
	bool was_clicked(GGClickEvent *);
	
	bool onMouseUp(GGClickEvent *) override;
	bool onMouseDown(GGClickEvent *) override;
private :
	int width;
	int height;
	std::vector<Lable *> lables;
	Lable *currentChoice;
	bool expanded;
	bool active;
	glm::vec2 *position;
	bool wasClicked(GGClickEvent *) override;
	void placeItems();
};

#endif
