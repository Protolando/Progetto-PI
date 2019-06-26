#include "rect.hpp"

#include<bits/stdc++.h> 
#include <glm/gtc/matrix_transform.hpp>

bool compareRectCornersY(glm::vec2 a, glm::vec2 b){
	if(a.y == b.y)
		return a.x < b.x;
	else 
		return a.y > b.y;
}
bool compareRectCornersX(glm::vec2 a, glm::vec2 b){
	if(a.x == b.x)
		return a.y > b.y;
	else 
		return a.x < b.x;
}

Rect::Rect(glm::vec2 *pos, float width, float height){
	Rect::width = width;
	Rect::height = height;
	Rect::transformMatrix = glm::mat4(1.0f);
	Rect::translate(*pos);
}

Rect::Rect(float x, float y, float width, float height){
	Rect::width = width;
	Rect::height = height;
	Rect::transformMatrix = glm::mat4(1.0f);
	Rect::translate(glm::vec2(x, y));
}

Rect::Rect(glm::vec2 pos, float width, float height){
	Rect::width = width;
	Rect::height = height;
	Rect::transformMatrix = glm::mat4(1.0f);
	Rect::translate(pos);
}

Rect::~Rect(){}

float Rect::getWidth(){
	return Rect::width;
}

float Rect::getHeight(){
	return Rect::height;
}

glm::vec2 Rect::getTopLeft(){
	glm::vec4 tmp = transformMatrix*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return glm::vec2(tmp.x, tmp.y);
}

glm::vec2 Rect::getTopRight(){
	glm::vec4 tmp = transformMatrix*glm::vec4(
		Rect::width,
		0.0f,
		0.0f,
		1.0f
	);
	return glm::vec2(tmp.x, tmp.y);
}

glm::vec2 Rect::getBotLeft(){
	glm::vec4 tmp = transformMatrix*glm::vec4(
		0.0f,
		Rect::height,
		0.0f,
		1.0f
	);
	return glm::vec2(tmp.x, tmp.y);
}

glm::vec2 Rect::getBotRight(){
	glm::vec4 tmp = transformMatrix*glm::vec4(
		Rect::width,
		Rect::height,
		0.0f,
		1.0f
	);
	return glm::vec2(tmp.x, tmp.y);
}

bool Rect::intersect(Rect r0, Rect r1){
	if(Rect::contains(r0, r1.getTopLeft()))
		return true;
	else if(Rect::contains(r0, r1.getTopRight()))
		return true;
	else if(Rect::contains(r0, r1.getBotLeft()))
		return true;
	else if(Rect::contains(r0, r1.getBotRight()))
		return true;
	else if(Rect::contains(r1, r0.getTopLeft()))
		return true;
	else if(Rect::contains(r1, r0.getTopRight()))
		return true;
	else if(Rect::contains(r1, r0.getBotLeft()))
		return true;
	else if(Rect::contains(r1, r0.getBotRight()))
		return true;
	else
		return false;
}

void Rect::rotate(float theta){
	Rect::transformMatrix = glm::rotate(Rect::transformMatrix, theta, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Rect::translate(glm::vec2 pos){
	Rect::transformMatrix = glm::translate(Rect::transformMatrix, glm::vec3(pos, 1.0f));
}

void Rect::transform(glm::mat4 transform){
	Rect::transformMatrix = transform*Rect::transformMatrix;
}

Rect Rect::clone(){
	Rect ret(0.0f, 0.0f, Rect::width, Rect::height);
	ret.transform(Rect::transformMatrix);

	return ret;
}

bool Rect::contains(Rect r, glm::vec2 p){

	//Sorting the corners so that they are, in order, the one on top, the one on left, bottom and right
	glm::vec2 corners[] = {r.getTopLeft(), r.getTopRight(), r.getBotLeft(), r.getBotRight()};
	std::sort(corners, corners+4, compareRectCornersY);
	std::sort(corners+1, corners+4, compareRectCornersX);
	std::sort(corners+2, corners+4, compareRectCornersY);
	glm::vec2 tmp = corners[3];
	corners[3] = corners[2];
	corners[2] = tmp;
	
	double as[4];
	double bs[4];
	double cs[4];
	
	for(int i = 0; i < 4; i++){
		as[i] = corners[i].y - corners[(i+1)%4].y;
		bs[i] = corners[(i+1)%4].x - corners[i].x;
		cs[i] = corners[i].x*corners[i].y - corners[i].y*corners[(i+1)%4].x +
			corners[(i+1)%4].y*corners[i].x - corners[i].x*corners[i].y;
	}
	
	if(
		(as[0]*p.x+bs[0]*p.y+cs[0]) >= 0 &&
		(as[1]*p.x+bs[1]*p.y+cs[1]) >= 0 &&
		(as[2]*p.x+bs[2]*p.y+cs[2]) >= 0 &&
		(as[3]*p.x+bs[3]*p.y+cs[3]) >= 0
	)
		return true;
	else
		return false;
}
