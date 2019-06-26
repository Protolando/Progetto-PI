#ifndef CONTEXTINC
#define CONTEXTINC

#include "Views/GGView.hpp"
#include "renderer.hpp"
#define default_window_height 600
#define default_window_width 800

#include <stdio.h>

//Opengl
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//handles the graphic environment
namespace Context{
	//Vars
	extern GGView* currentView;
	extern GLFWwindow* window;
	extern Renderer* renderer;
	extern int window_width;
	extern int window_height;
	extern glm::mat4 projection;
	extern bool textInput;
	
	//Functions
	extern void init(std::string);
	extern void invalidate();
	extern void onResize(GLFWwindow* window, int width, int height);
	extern void onMouseMove(GLFWwindow* window, double xpos, double ypos);
	extern void onClick(GLFWwindow* window, int button, int action, int mods);
	extern void keyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	extern void charInput(GLFWwindow* window, unsigned int codepoint);
	extern void switchView(GGView *view);
	extern void mainLoop();
	extern void setView(GGView*);
	extern void setCursorToNormal();
	extern void setCursorToResize();
	extern void setCursorToClick();
	extern std::string getClipboardContent();
	extern bool copyFile(const char*, const char*);
	extern void writeToFile(const char*, const char*);
	extern std::string readFromFile(const char*);
	extern void unsubscribeTextInput();
	extern void subscribeTextInput();
	extern void freeResources();
	extern void logic();
	extern glm::vec2 getCursorPos();
	
	extern GLint getTexureProgram();
};
#endif
