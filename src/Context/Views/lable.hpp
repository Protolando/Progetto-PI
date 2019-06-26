#ifndef LABLEINC
#define LABLEINC

#include "GGView.hpp"
#include <string>
#include "../context.hpp"

class Lable : public GGView{
public:
	Lable(std::string*, int, glm::vec4* , glm::vec2*);
	Lable(const char*, int, glm::vec4* , glm::vec2*);
	~Lable();
	
	void setText(std::string*);
	void setText(const char*);
	void setSize(int);
	void setColor(glm::vec4*);
	void setPosition(glm::vec2*);
	
	glm::vec2 getPosition();

	int getWidth();
	int getHeight();
	std::string* getText();
	
	void draw() override;
private:
	std::string* text;
	int size;
	glm::vec4* color;
	glm::vec2* position;
	bool wasClicked(GGClickEvent*) override;
};
#endif
